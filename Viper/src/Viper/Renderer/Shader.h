#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Viper {

	class Shader
	{
	public:
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;

		// TODO: instead of uniform upload a material
		// TODO: change UploadUniform to Set() or Upload() because DX doesn't use uniforms
		virtual void UploadUniform(const std::string& name, int uniform) = 0;
		virtual void UploadUniform(const std::string& name, float uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec2& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec3& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::vec4& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat3& uniform) = 0;
		virtual void UploadUniform(const std::string& name, const glm::mat4& uniform) = 0;

		static Shared<Shader> Create(const std::string& path);
		static Shared<Shader> Create(const std::string& name, 
									 const std::string& vertexSource,
									 const std::string& fragmentSource);
	};

	// ShaderLibrary assumes that m_Name exists 
	// for a Shader as it uses Shader::GetName()
	class ShaderLibrary
	{
	public:
		void Add(const Shared<Shader>& shader);
		void Load(const std::string& path);
		Shared<Shader> Get(const std::string& name);
	private:
		std::unordered_map<std::string, Shared<Shader>> m_Shaders;
	};

}