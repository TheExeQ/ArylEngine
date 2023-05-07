#pragma once

#include "Aryl/Core/Base.h"

#include "Aryl/Renderer/VertexArray.h"

namespace Aryl 
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override { return myVertexBuffers; };
		virtual const Ref<IndexBuffer>& GetIndexBuffer() override { return myIndexBuffer; };

	private:
		std::vector<Ref<VertexBuffer>> myVertexBuffers;
		Ref<IndexBuffer> myIndexBuffer;
		uint32_t myRendererID;
	};
}