#include "EntityManager.h"

#include <iostream>
#include <algorithm>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	//todo add entitnes from entitiestoadd vector to the proper location
	// add them to the vector of all entitites]
	// add them to the vecot inside the mao with the tafg as a key

	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
	}

	m_entitiesToAdd.clear();

	//remove dead entities from vector of all entities
	removeDeadEntities(m_entities);

	//remove dead entities from each vector in the entity map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}

}


void EntityManager::removeDeadEntities(EntityVec& vec)
{
	const auto new_end = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& entityPtr) {
		return !entityPtr->m_active; // Remove entities that are NOT active (i.e., dead)
		});

	vec.erase(new_end, vec.end());
}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntitites++, tag));

	m_entitiesToAdd.push_back(entity);

	m_entityMap[tag].push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: this is incorrect
	return m_entityMap[tag];
}
