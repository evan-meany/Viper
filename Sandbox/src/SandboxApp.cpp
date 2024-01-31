#include <Viper.h>
#include <imgui.h>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPosition(0.0f)
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

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
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
			out vec3 v_Position;
			uniform mat4 u_ViewProjection;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(Viper::Shader::Create(squareVertexSource, squareFragmentSource));
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnUpdate(Viper::Timestep timestep) override
	{
		VP_TRACE("Delta time: {0}s, {1}ms", timestep.GetSeconds(), timestep.GetMilliseconds());

		if (Viper::Input::IsKeyPressed(VP_KEY_A)) { m_CameraPosition.x -= m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_D)) { m_CameraPosition.x += m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_W)) { m_CameraPosition.y += m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_S)) { m_CameraPosition.y -= m_CameraPositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_R)) { m_CameraRotation -= m_CameraRotationSpeed * timestep; }

		Viper::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Viper::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Viper::Renderer::BeginScene();

		m_SquareShader->UploadUniform("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
		Viper::Renderer::Submit(m_SquareShader, m_SquareVertexArray);

		m_Shader->UploadUniform("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
		Viper::Renderer::Submit(m_Shader, m_VertexArray);

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
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraPositionSpeed = 0.5f;
	float m_CameraRotationSpeed = glm::radians(35.0f);
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