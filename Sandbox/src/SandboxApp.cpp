#include <Viper.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		////////////////
		// First shader
		////////////////
		m_VertexArray.reset(Viper::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Viper::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Viper::VertexBuffer::Create(vertices, sizeof(vertices)));
		Viper::BufferLayout layout = {
			{ Viper::ShaderDataType::Float3, "a_Position" },
			{ Viper::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBufffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Viper::IndexBuffer> indexBuffer;
		indexBuffer.reset(Viper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(Viper::Shader::Create(vertexSource, fragmentSource));

		////////////////
		// Second shader
		////////////////
		m_SquareVertexArray.reset(Viper::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<Viper::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Viper::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({ { Viper::ShaderDataType::Float3, "a_Position" } });
		m_SquareVertexArray->AddVertexBufffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Viper::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Viper::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_SquareShader.reset(Viper::Shader::Create(squareVertexSource, squareFragmentSource));
	}

	virtual void OnImGuiRender() override
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
		ImGui::ColorPicker4("Color picker", m_Color);
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
		
		Viper::Renderer::Submit(m_Shader, m_VertexArray);

		//Viper::MaterialRef material = new Viper::Material(m_SquareShader);
		//material->Set("u_Color", red);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
		glm::vec4 red(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		glm::vec4 blue(0.0f, 0.0f, 255.0f, 1.0f);
		for (uint32_t i = 0; i < 5; i++)
		{
			for (uint32_t j = 0; j < 5; j++)
			{
				glm::vec3 pos(i * 0.21f, j * 0.21f, 0.0f);
				pos += m_SquarePosition;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (i % 2 == 0) { m_SquareShader->UploadUniform("u_Color", red); }
				else { m_SquareShader->UploadUniform("u_Color", blue); }
				Viper::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
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
	std::shared_ptr<Viper::VertexArray> m_VertexArray;
	std::shared_ptr<Viper::Shader> m_Shader;

	std::shared_ptr<Viper::VertexArray> m_SquareVertexArray;
	std::shared_ptr<Viper::Shader> m_SquareShader;

	Viper::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(0.0f);
	float m_CameraRotation = 0.0f;
	float m_CameraPositionSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquarePosition = glm::vec3(0.0f);
	float m_SquarePositionSpeed = 1.0f;

	float m_Color[4] = { 0.0f, 0.0f, 255.0f, 0.0f };
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