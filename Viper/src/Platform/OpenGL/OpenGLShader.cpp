#include "vppch.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Viper {

	static std::string ExtractNameFromPath(const std::string& path)
	{
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		return path.substr(lastSlash, count);
	}

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") { return GL_VERTEX_SHADER; }
		if (type == "fragment" || type == "pixel") { return GL_FRAGMENT_SHADER; }

		VP_CORE_ASSERT(false, "Uknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
		m_Name = ExtractNameFromPath(path);
	}

	OpenGLShader::OpenGLShader(const std::string& name, 
							   const std::string& vertexSource,
							   const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniform(const std::string& name, int uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, uniform);
	}

	void OpenGLShader::UploadUniform(const std::string& name, float uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, uniform);
	}	
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec2& uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, uniform.x, uniform.y);
	}	
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec3& uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, uniform.x, uniform.y, uniform.z);
	}	
	
	void OpenGLShader::UploadUniform(const std::string& name, const glm::vec4& uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, uniform.x, uniform.y, uniform.z, uniform.w);
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat3& uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
	}

	void OpenGLShader::UploadUniform(const std::string& name, const glm::mat4& uniform)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform));
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.is_open()) { VP_CORE_ASSERT(false, "Unable to open '{0}'", path); return result; }

		file.seekg(0, std::ios::end);
		auto filePosition = file.tellg();
		if (filePosition < 0) { VP_CORE_ASSERT(false, "Unable to read '{0}'", path); return result; }
		result.resize(filePosition);
		file.seekg(0, std::ios::beg);
		file.read(&result[0], result.size());
		file.close();

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		if (source.size() == 0) 
		{ 
			VP_CORE_ASSERT(false, "Shader source is empty");
		}

		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			VP_CORE_ASSERT(eol != std::string::npos, "Syntax error in shader");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GLenum glType = ShaderTypeFromString(type);
			VP_CORE_ASSERT(glType, "Invalid type specifier in shader");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[glType] = source.substr(nextLinePos, 
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		if (shaderSources.size() == 0) { VP_CORE_ASSERT(false, "No shader sources"); }

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());
		for (const auto& [shaderType, shaderSource] : shaderSources)
		{
			GLuint shader = glCreateShader(shaderType);
			const GLchar* source = (const GLchar*)shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);
				VP_CORE_ERROR("{0}", infoLog.data());
				VP_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (const auto& shader : glShaderIDs)
			{
				glDeleteShader(shader);
			}

			VP_CORE_ERROR("{0}", infoLog.data());
			VP_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		for (const auto& shader : glShaderIDs)
		{
			glDetachShader(program, shader);
		}

		m_RendererID = program;
	}
}