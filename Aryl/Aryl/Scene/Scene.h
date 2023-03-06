#pragma once

#include "Aryl/Events/Event.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <map>
#include <set>

namespace Aryl
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);

		inline entt::registry& GetRegistry() { return myRegistry; }
		inline const std::string& GetName() const { return myName; }
		inline const bool IsPlaying() const { return myIsPlaying; }

		void OnRuntimeStart();
		void OnRuntimeEnd();

		void Update(float aDeltaTime);
		void UpdateEditor(float aDeltaTime);
		void OnEvent(Event& e);

		Entity CreateEntity(const std::string& tag = "");
		void RemoveEntity(Entity entity);

		void ParentEntity(Entity parent, Entity child);
		void UnparentEntity(Entity entity);

		//glm::mat4 GetWorldSpaceTransform(Entity entity);

		//glm::vec3 GetWorldForward(Entity entity);
		//glm::vec3 GetWorldRight(Entity entity);
		//glm::vec3 GetWorldUp(Entity entity);

		template<typename T>
		const std::vector<entt::entity> GetAllEntitiesWith() const;

		const Entity GetEntityWithName(std::string name);

	private:
		friend class Entity;

		void IsRecursiveChildOf(Entity mainParent, Entity currentEntity, bool& outChild);
		void ConvertToWorldSpace(Entity entity);
		void ConvertToLocalSpace(Entity entity);

		bool myIsPlaying = false;
		float myTimeSinceStart = 0.f;

		std::string myName = "New Scene";
		entt::registry myRegistry;
	};

	template<typename T>
	inline const std::vector<entt::entity> Scene::GetAllEntitiesWith() const
	{
		return myRegistry.view<T>();
	}
}
