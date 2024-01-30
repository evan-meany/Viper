#include "vppch.h"
#include "Application.h"
#include "Input.h"
#include "Events/Event.h"
#include "Renderer/Renderer.h"

namespace Viper
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		VP_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(VP_BIND_EVENT_FUNC(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		////////////////
		// First shader
		////////////////
		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBufffer(vertexBuffer);
	
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

		m_Shader.reset(Shader::Create(vertexSource, fragmentSource));

		////////////////
		// Second shader
		////////////////
		m_SquareVertexArray.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({ { ShaderDataType::Float3, "a_Position" } });
		m_SquareVertexArray->AddVertexBufffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_SquareShader.reset(Shader::Create(squareVertexSource, squareFragmentSource));
	}

	Application::~Application()
	{
		// Layers are deleted in LayerStack destructor
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
			RenderCommand::Clear();

			Renderer::BeginScene();
			
			m_SquareShader->Bind();
			m_SquareShader->UploadUniform("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			Renderer::Submit(m_SquareVertexArray);

			m_Shader->Bind();
			m_Shader->UploadUniform("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			Renderer::Submit(m_VertexArray);
			
			Renderer::EndScene();

			// Render layers from back to front
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			// Render the ImGui layers
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			// Poll for and handle events
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VP_BIND_EVENT_FUNC(Application::OnWindowClose));

		// Handle layer events from front to back
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled()) { break; }
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& evnt)
	{
		m_Running = false;
		return true;
	}

}