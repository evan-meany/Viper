#include "vppch.h"
#include "Shader.h"
#include "Viper/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Viper {

	Shared<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);
		}

		VP_CORE_ASSERT(false, "OpenGLShader should not be nullptr");
		return nullptr;
	}

	Shared<Shader> Shader::Create(const std::string& name, 
								  const std::string& vertexSource,
			     				  const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VP_CORE_ASSERT(false, "RendererAPI cannot be None");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}
		
		VP_CORE_ASSERT(false, "OpenGLShader should not be nullptr");
		return nullptr;
	}

	void ShaderLibrary::Add(const Shared<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		if (m_Shaders.find(name) != m_Shaders.end()) 
		{ 
			VP_CORE_WARN("ShaderLibrary already contains {0}", name);
		}
		m_Shaders.insert({shader->GetName(), shader});
	}
	
	void ShaderLibrary::Load(const std::string& path)
	{
		auto shader = Shader::Create(path);
		Add(shader);
	}

	Shared<Shader> ShaderLibrary::Get(const std::string& name)
	{
		auto lookup = m_Shaders.find(name);
		if (lookup == m_Shaders.end()) 
		{ 
			VP_CORE_ASSERT(false, "ShaderLibrary does not contain {0}", name);
			return nullptr;
		}
		return lookup->second;
	}

} // end Viper namespace