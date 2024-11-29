#pragma once

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>>	EntityVec;
typedef std::map<std::string, EntityVec>		EntityMap;

class EntityManager
{
private:
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_toAdd;
	size_t    m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);

public:

	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
};