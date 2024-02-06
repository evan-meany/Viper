#include <Viper.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		//////////////////
		// Triangle shader
		//////////////////
		m_VertexArray = Viper::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		auto vertexBuffer = Viper::VertexBuffer::Create(vertices, sizeof(vertices));
		Viper::BufferLayout layout = {
			{ Viper::ShaderDataType::Float3, "a_Position" },
			{ Viper::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBufffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		auto indexBuffer = Viper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBufffer(indexBuffer);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			in vec3 v_Position;
			in vec4 v_Color;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader = Viper::Shader::Create(vertexSource, fragmentSource);

		////////////////
		// Square shader
		////////////////
		m_SquareVertexArray = Viper::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		auto squareVertexBuffer = Viper::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVertexBuffer->SetLayout({ { Viper::ShaderDataType::Float3, "a_Position" }, 
										{ Viper::ShaderDataType::Float2, "a_TextureCoord" } });
		m_SquareVertexArray->AddVertexBufffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto squareIndexBuffer = Viper::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBufffer(squareIndexBuffer);

		std::string squareVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

		m_FlatColorShader = Viper::Shader::Create(squareVertexSource, squareFragmentSource);

		////////////////
		// Texture shader
		////////////////
		std::string textureVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TextureCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TextureCoord;

			void main()
			{
				v_TextureCoord = a_TextureCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform sampler2D u_Texture;

			in vec2 v_TextureCoord;

			void main()
			{
				color = texture(u_Texture, v_TextureCoord);
				//color = vec4(v_TextureCoord, 0.0f, 1.0f);
			}
		)";
		m_TextureShader = Viper::Shader::Create(textureVertexSource, textureFragmentSource);

		m_Texture = Viper::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = Viper::Texture2D::Create("assets/textures/ChernoLogo.png");

		m_TextureShader->Bind();
		m_TextureShader->UploadUniform("u_Texture", (int)0);
	}

	virtual void OnImGuiRender() override
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
		ImGui::Begin("Settings");
		ImGui::ColorPicker4("", glm::value_ptr(m_Color));
		ImGui::End();
	}

	void OnUpdate(Viper::Timestep timestep) override
	{
		//VP_TRACE("Delta time: {0}s, {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());

		if (Viper::Input::IsKeyPressed(VP_KEY_A)) { m_CameraPosition.x -= m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_D)) { m_CameraPosition.x += m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_W)) { m_CameraPosition.y += m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_S)) { m_CameraPosition.y -= m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_R)) { m_CameraRotation -= m_CameraRotationSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_T)) { m_CameraPosition = glm::vec3(0.0f); m_CameraRotation = 0.0f; }

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		if (Viper::Input::IsKeyPressed(VP_KEY_LEFT)) { m_SquarePosition.x -= m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_RIGHT)) { m_SquarePosition.x += m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_DOWN)) { m_SquarePosition.y -= m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_UP)) { m_SquarePosition.y += m_SquarePositionSpeed * timestep; }

		Viper::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Viper::RenderCommand::Clear();

		Viper::Renderer::BeginScene(m_Camera);
		
		// Triangle rendering
		//Viper::Renderer::Submit(m_Shader, m_VertexArray);

		//Viper::MaterialRef material = new Viper::Material(m_FlatColorShader);
		//material->Set("u_Color", red);

		// Big square
		m_Texture->Bind();
		Viper::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		m_LogoTexture->Bind();
		Viper::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

		// Little squares
		std::dynamic_pointer_cast<Viper::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Viper::OpenGLShader>(m_FlatColorShader)->UploadUniform("u_Color", m_Color);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (uint32_t i = 0; i < 5; i++)
		{
			for (uint32_t j = 0; j < 5; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				pos += m_SquarePosition;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Viper::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		Viper::Renderer::EndScene();
	}

	void OnEvent(Viper::Event& evnt) override
	{
		Viper::EventDispatcher dispatcher(evnt);
		dispatcher.Dispatch<Viper::KeyPressedEvent>(VP_BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Viper::KeyPressedEvent& evnt)
	{
		return false;
	}
private:
	Viper::Shared<Viper::VertexArray> m_VertexArray;
	Viper::Shared<Viper::Shader> m_Shader;

	Viper::Shared<Viper::VertexArray> m_SquareVertexArray;
	Viper::Shared<Viper::Shader> m_FlatColorShader;

	Viper::Shared<Viper::Shader> m_TextureShader;

	Viper::Shared<Viper::Texture2D> m_Texture;
	Viper::Shared<Viper::Texture2D> m_LogoTexture;

	Viper::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float m_CameraRotation = 0.0f;
	float m_CameraPositionSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquarePosition = glm::vec3(0.5f);
	float m_SquarePositionSpeed = 1.0f;

	glm::vec4 m_Color = { 0.8, 0.3f, 0.1f, 1.0f };
};

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		// ImGuiLayer automatically added
	}

	~Sandbox()
	{

	}
};

Viper::Application* Viper::CreateApplication()
{
	return new Sandbox();
}