#include "Scene.h"
#include "Entity.h"
#include "Aryl/Components/Components.h"

namespace Aryl
{
	Scene::Scene(const std::string& name)
		: myName(name)
	{

	}

	Scene::Scene()
	{

	}

	void Scene::OnRuntimeStart()
	{
		myTimeSinceStart = 0;
	}

	void Scene::OnRuntimeEnd()
	{

	}

	void Scene::Update(float aDeltaTime)
	{
		myTimeSinceStart += aDeltaTime;
	}

	void Scene::UpdateEditor(float aDeltaTime)
	{

	}

	void Scene::OnEvent(Event& e)
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity e(myRegistry.create(), this);
		e.AddComponent<TagComponent>().tag = tag;
		return e;
	}

	void Scene::RemoveEntity(Entity entity)
	{
		myRegistry.destroy(entity.myId);
	}

	void Scene::ParentEntity(Entity parent, Entity child)
	{

	}

	void Scene::UnparentEntity(Entity entity)
	{

	}

	const Entity Scene::GetEntityWithName(std::string name)
	{
		for (const auto& e : myRegistry.view<TagComponent>())
		{
			if (myRegistry.get<TagComponent>(e).tag == name)
			{
				return Entity(e, this);
			}
		}
		return Entity();
	}

	void Scene::IsRecursiveChildOf(Entity mainParent, Entity currentEntity, bool& outChild)
	{

	}

	void Scene::ConvertToWorldSpace(Entity entity)
	{

	}

	void Scene::ConvertToLocalSpace(Entity entity)
	{

	}
}
