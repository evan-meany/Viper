#pragma once

#include "Viper/Renderer/Shader.h"

#include <glad/glad.h>

#include <unordered_map>

namespace Viper {
	class OpenGLShader : public Shader 
	{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name,
					 const std::string& vertexSource,
					 const std::string& fragmentSource);
		~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string& GetName() const override { return m_Name; }
		virtual void UploadUniform(const std::string& name, int uniform) override;
		virtual void UploadUniform(const std::string& name, float uniform) override;
		virtual void UploadUniform(const std::string& name, const glm::vec2& uniform) override;
		virtual void UploadUniform(const std::string& name, const glm::vec3& uniform) override;
		virtual void UploadUniform(const std::string& name, const glm::vec4& uniform) override;
		virtual void UploadUniform(const std::string& name, const glm::mat3& uniform) override;
		virtual void UploadUniform(const std::string& name, const glm::mat4& uniform) override;
	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}