#pragma once

#include "Entity.h"
#include<vector>
#include<map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	std::map<std::string, EntityVec> m_entityMap;
	size_t    m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);
public:
	EntityManager();

	void Update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);
	std::shared_ptr<Entity> addEntity(const size_t id, const std::string& tag);

	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
};
