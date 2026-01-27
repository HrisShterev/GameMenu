#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::Update()
{
	//todo: add entities from m_entetiesToAdd to the proper location (s)
	//add them to the vector insede the map with the tag as the key

	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();
	//remove dead entities from the vector of all the entities

	removeDeadEntities(m_entities);

	//remove dead entities form each vector in the entity map
	for (auto& pair : m_entityMap)
	{
		auto& entityVec = pair.second;
		removeDeadEntities(entityVec);
	}
}


void EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto it = vec.begin();
	while (it != vec.end())
	{
		if (!(*it)->isActive())
		{
			it = vec.erase(it);
		}
		else
		{
			++it;
		}
	}
}
std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

std::shared_ptr<Entity> EntityManager::addEntity(const size_t id, const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(id, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}


const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	static EntityVec empty; // Avoid returning reference to a temporary object
	if (m_entityMap.find(tag) != m_entityMap.end())
	{
		return m_entityMap[tag];
	}
	return empty;
}
