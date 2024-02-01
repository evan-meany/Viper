#pragma once

#include <memory>
#include "Buffer.h"

namespace Viper {
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddVertexBufffer(const Shared<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBufffer(const Shared<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Shared<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Shared<IndexBuffer>& GetIndexBuffer() const = 0;
		static VertexArray* Create();
	};
}