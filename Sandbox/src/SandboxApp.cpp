#include <Viper.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Viper::Layer
{
public:
	ExampleLayer(float aspectRatio) 
		: Layer("Example"), m_CameraController(aspectRatio)
	{
		//Viper::Input::HideCursor();

		//////////////////
		// Triangle shader
		//////////////////
		m_TriangleVertexArray = Viper::VertexArray::Create();

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
		m_TriangleVertexArray->AddVertexBufffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		auto indexBuffer = Viper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_TriangleVertexArray->SetIndexBufffer(indexBuffer);
		m_TriangleShader = Viper::Shader::Create("assets/shaders/Triangle.glsl");

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
		auto squareIndexBuffer = Viper::IndexBuffer::Create(squareIndices, 
															sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBufffer(squareIndexBuffer);
		m_SquareShader = Viper::Shader::Create("assets/shaders/Square.glsl");

		////////////////
		// Texture shader
		////////////////
		m_TextureShader = Viper::Shader::Create("assets/shaders/Texture.glsl");
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
		if (m_CameraOn)
		{
			m_CameraController.OnUpdate(timestep);
		}
		if (Viper::Input::IsKeyPressed(VP_KEY_LEFT)) { m_SquarePosition.x -= m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_RIGHT)) { m_SquarePosition.x += m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_DOWN)) { m_SquarePosition.y -= m_SquarePositionSpeed * timestep; }
		if (Viper::Input::IsKeyPressed(VP_KEY_UP)) { m_SquarePosition.y += m_SquarePositionSpeed * timestep; }

		Viper::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Viper::RenderCommand::Clear();

		Viper::Renderer::BeginScene(m_CameraController.GetCamera());
		
		// Triangle rendering
		Viper::Renderer::Submit(m_TriangleShader, m_TriangleVertexArray);

		//Viper::MaterialRef material = new Viper::Material(m_SquareShader);
		//material->Set("u_Color", red);

		// Textures
		m_Texture->Bind();
		Viper::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		m_LogoTexture->Bind();
		Viper::Renderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

		// Little squares
		m_SquareShader->Bind();
		m_SquareShader->UploadUniform("u_Color", m_Color);
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (uint32_t i = 0; i < 5; i++)
		{
			for (uint32_t j = 0; j < 5; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				pos += m_SquarePosition;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Viper::Renderer::Submit(m_SquareShader, m_SquareVertexArray, transform);
			}
		}

		Viper::Renderer::EndScene();
	}

	void OnEvent(Viper::Event& evnt) override
	{
		m_CameraController.OnEvent(evnt);

		Viper::EventDispatcher dispatcher(evnt);
		dispatcher.Dispatch<Viper::KeyPressedEvent>(VP_BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Viper::KeyPressedEvent& evnt)
	{
		auto keycode = evnt.GetKeyCode();
		if (keycode == VP_KEY_ESCAPE)
		{
			m_CameraOn = false;
			Viper::Input::HideCursor(false);
		}
		return false;
	}
private:
	Viper::Shared<Viper::VertexArray> m_TriangleVertexArray;
	Viper::Shared<Viper::Shader> m_TriangleShader;

	Viper::Shared<Viper::VertexArray> m_SquareVertexArray;
	Viper::Shared<Viper::Shader> m_SquareShader;

	Viper::Shared<Viper::Shader> m_TextureShader;

	Viper::Shared<Viper::Texture2D> m_Texture;
	Viper::Shared<Viper::Texture2D> m_LogoTexture;

	bool m_CameraOn = true;
	//Viper::CameraController m_CameraController;
	Viper::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition = glm::vec3(0.5f);
	float m_SquarePositionSpeed = 1.0f;

	glm::vec4 m_Color = { 0.8, 0.3f, 0.1f, 1.0f };
};

class Sandbox : public Viper::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer(GetWindow().AspectRatio()));
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