#include "Entity.h"
#include "Aryl/Components/Components.h"

namespace Aryl
{
	Entity::Entity()
	{

	}

	Entity::Entity(entt::entity id, Scene* scene)
		: myId(id), myScene(scene)
	{

	}

	Entity::Entity(const Entity& entity)
	{
		myId = entity.myId;
		myScene = entity.myScene;
	}

	Entity::~Entity()
	{

	}

	const std::string Entity::GetTag()
	{
		return GetComponent<TagComponent>().tag;
	}

	void Entity::SetTag(const std::string& tag)
	{
		GetComponent<TagComponent>().tag = tag;
	}

	const std::vector<Entity> Entity::GetChilden() const
	{
		return std::vector<Entity>();
	}

	const Entity Entity::GetParent() const
	{
		return Entity();
	}

	void Entity::ResetParent()
	{

	}

	void Entity::RemoveChild(Entity entity)
	{

	}

	void Entity::SetPosition(const glm::vec3& position, bool updatePhysics /*= true*/)
	{

	}

	void Entity::SetRotation(const glm::quat& rotation, bool updatePhysics /*= true*/)
	{

	}

	void Entity::SetScale(const glm::vec3& scale)
	{

	}

	void Entity::SetLocalPosition(const glm::vec3& position, bool updatePhysics /*= true*/)
	{

	}

	void Entity::SetLocalRotation(const glm::quat& rotation, bool updatePhysics /*= true*/)
	{

	}

	void Entity::SetLocalScale(const glm::vec3& scale)
	{

	}
}
