#include "Entity.h"
#include <iostream>
#pragma once

Entity::Entity(const size_t id, const std::string& tag)
    : m_id(id), m_tag(tag)
{
    // Initialize other members if needed
}

const std::string& Entity::tag() const
{
    // TODO: insert return statement here
    std::cout << m_tag << std::endl;

    return m_tag;
}

bool Entity::isActive() const
{
    return false;
}


const size_t Entity::id() const
{
    return size_t();
}

void Entity::destroy()
{
    m_active = false;
}
