#pragma once

namespace Aryl 
{
	enum class PrimitiveType
	{
		None = 0, Triangles, Lines
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL,
			Vulkan
		};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		static API GetAPI() { return s_CurrentRendererAPI; }
		static void SetAPI(API api) { s_CurrentRendererAPI = api; }

	private:
		inline static API s_CurrentRendererAPI = API::OpenGL;
	};

}