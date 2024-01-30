#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Viper {

	class Shader
	{
	public:
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat4& uniform) = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
	};

}