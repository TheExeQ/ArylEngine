#pragma once

#include "Aryl/Core/Base.h"
#include "Buffer.h"

namespace Aryl 
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() = 0;

		static Ref<VertexArray> Create();
	};
}