#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Viper {

	class Shader
	{
	public:
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// TODO: instead of uniform upload a material
		virtual void UploadUniform(const std::string& name, int uniform) = 0;
		virtual void UploadUniform(const std::string& name, float uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec2& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec3& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec4& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat3& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat4& uniform) = 0;

		static Shared<Shader> Create(const std::string& path);
		static Shared<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);
	};

}