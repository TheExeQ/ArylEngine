#pragma once
#include "Aryl/Core/Base.h"

namespace Aryl
{
	class Scene;
	class SceneManager
	{
	public:
		static void Reset();
		inline static void SetActiveScene(Ref<Scene> scene) { myActiveScene = scene; }
		inline static Ref<Scene> GetActiveScene() { return myActiveScene; }

	private:
		SceneManager() = delete;
		inline static Ref<Scene> myActiveScene;
	};
}