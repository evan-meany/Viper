#pragma once

#include "Viper/Renderer/Shader.h"

namespace Viper {
	class OpenGLShader : public Shader 
	{
	public:
		OpenGLShader(const std::string& vertexSource,
					 const std::string& fragmentSource);
		~OpenGLShader();
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_RendererID;
	};
}