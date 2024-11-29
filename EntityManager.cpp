#include "EntityManager.h"
#include <algorithm>
#include <iostream>

EntityManager::EntityManager()
{
    m_totalEntities = 0;
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	for (size_t i = vec.size(); i-- > 0;){
        if (!vec[i]->isActive()){
            vec.erase(vec.begin() + i);
        }
    }
}

void EntityManager::update()
{
    for (auto e : m_toAdd){
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    
    removeDeadEntities(m_entities);
    
	for (auto& [tag, entityVec] : m_entityMap)
        removeDeadEntities(entityVec);

    m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_toAdd.push_back(e);
    return e;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}