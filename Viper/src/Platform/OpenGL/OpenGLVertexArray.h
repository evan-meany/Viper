#pragma once

#include "Viper/Renderer/VertexArray.h"

namespace Viper {
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBufffer(const Shared<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBufffer(const Shared<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const override
		{
			return m_VertexBuffers;
		}
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const override
		{
			return m_IndexBuffer;
		}
	private:
		std::vector<Shared<VertexBuffer>> m_VertexBuffers;
		Shared<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}