#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Aryl 
{
	static GLenum ShaderDataTypeToOpenGLBaseType(Aryl::ShaderDataType type)
	{
		switch (type)
		{
			case Aryl::ShaderDataType::Float:    return GL_FLOAT;
			case Aryl::ShaderDataType::Float2:   return GL_FLOAT;
			case Aryl::ShaderDataType::Float3:   return GL_FLOAT;
			case Aryl::ShaderDataType::Float4:   return GL_FLOAT;
			case Aryl::ShaderDataType::Mat3:     return GL_FLOAT;
			case Aryl::ShaderDataType::Mat4:     return GL_FLOAT;
			case Aryl::ShaderDataType::Int:      return GL_INT;
			case Aryl::ShaderDataType::Int2:     return GL_INT;
			case Aryl::ShaderDataType::Int3:     return GL_INT;
			case Aryl::ShaderDataType::Int4:     return GL_INT;
			case Aryl::ShaderDataType::Bool:     return GL_BOOL;
		}

		YL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &myRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &myRendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(myRendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer)
	{
		YL_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(myRendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		myVertexBuffers.emplace_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
	{
		glBindVertexArray(myRendererID);
		indexBuffer->Bind();

		myIndexBuffer = indexBuffer;
	}
}