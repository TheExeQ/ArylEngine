#pragma once

#include "Aryl/Renderer/Buffer.h"

namespace Aryl 
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return myLayout; }
		virtual void SetLayout(const BufferLayout& layout) override { myLayout = layout; }

	private:
		uint32_t myRendererID;
		BufferLayout myLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return myCount; }
	private:
		uint32_t myRendererID;
		uint32_t myCount;
	};
}