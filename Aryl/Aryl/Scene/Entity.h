#pragma once
#include "Scene.h"

#include <entt/entt.hpp>
#include <unordered_map>

namespace Aryl
{
	class Entity
	{
	public:
		Entity();
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& entity);

		~Entity();

		template<typename T>
		T& GetComponent();

		template<typename T>
		T& GetComponent() const;

		const std::string GetTag();
		void SetTag(const std::string& tag);

		//const glm::mat4 GetTransform() const;
		//const glm::mat4 GetLocalTransform() const;

		//const glm::vec3 GetForward() const;
		//const glm::vec3 GetRight() const;
		//const glm::vec3 GetUp() const;

		//const glm::vec3 GetLocalForward() const;
		//const glm::vec3 GetLocalRight() const;
		//const glm::vec3 GetLocalUp() const;

		//const glm::vec3 GetLocalPosition() const;
		//const glm::quat GetLocalRotation() const;
		//const glm::vec3 GetLocalScale() const;

		//const glm::vec3 GetPosition() const;
		//const glm::quat GetRotation() const;
		//const glm::vec3 GetScale() const;

		const std::vector<Aryl::Entity> GetChilden() const;
		const Aryl::Entity GetParent() const;
		void ResetParent();
		void RemoveChild(Aryl::Entity entity);

		void SetPosition(const glm::vec3& position, bool updatePhysics = true);
		void SetRotation(const glm::quat& rotation, bool updatePhysics = true);
		void SetScale(const glm::vec3& scale);

		void SetLocalPosition(const glm::vec3& position, bool updatePhysics = true);
		void SetLocalRotation(const glm::quat& rotation, bool updatePhysics = true);
		void SetLocalScale(const glm::vec3& scale);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		bool HasComponent() const;

		template<typename T>
		void RemoveComponent();

		inline const entt::entity GetId() const { return myId; }
		inline bool IsNull() const
		{
			if (myId == entt::null)
			{
				return true;
			}

			if (!myScene || !myScene->GetRegistry().valid(myId))
			{
				return true;
			}

			return false;
		}
		inline Scene* GetScene() const { return myScene; };

		inline bool operator==(const Entity& entity) const { return myId == entity.myId; }
		inline bool operator!() const { return IsNull(); }
		inline explicit operator bool() const { return !IsNull(); }

		static Aryl::Entity Null() { return { entt::null, nullptr }; }

	private:
		friend class Scene;

		Scene* myScene = nullptr;
		entt::entity myId = entt::null;
	};

	template<typename T>
	inline T& Entity::GetComponent()
	{
		return myScene->myRegistry.get<T>(myId);
	}

	template<typename T>
	inline T& Entity::GetComponent() const
	{
		return myScene->myRegistry.get<T>(myId);
	}

	template<typename T, typename... Args>
	inline T& Entity::AddComponent(Args&&... args)
	{
		return myScene->myRegistry.emplace<T>(myId, std::forward<Args>(args)...);
	}

	template<typename T>
	inline bool Entity::HasComponent() const
	{
		return myScene->myRegistry.get<T>(myId);
	}

	template<typename T>
	inline void Entity::RemoveComponent()
	{
		myScene->myRegistry.remove<T>(myId);
	}
}
