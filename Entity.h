#pragma once

#include "Components.h"
#include<memory>
#include<string>
#include <unordered_map>
#include <typeindex>

class Entity
{
	friend class EntityManager;

    std::unordered_map<std::type_index, std::shared_ptr<void>> components;

	bool		m_active = true;
	size_t      m_id	 = 0;
	std::string m_tag = "deafault";

	Entity(const size_t id, const std::string& tag);

public:


    // Add a component of type T by copying 'component'.
    template<typename T>
    void addComponent(const T& component);

    // Get a shared_ptr to component T, or nullptr if absent.
    template<typename T>
    std::shared_ptr<T> getComponent();

    // Check for presence of component type T.
    template<typename T>
    bool hasComponent() const;

    // Remove (erase) the component of type T.
    template<typename T>
    void removeComponent();

	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape>		cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput>		cInput;
	std::shared_ptr<CScore>		cScore;
	std::shared_ptr<CLifespan>	cLifespan;

	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();

};

template<typename T>
void Entity::addComponent(const T& component) {
    // Create a shared_ptr<T> to store the component copy.
    std::shared_ptr<T> compPtr = std::make_shared<T>(component);
    // Store as shared_ptr<void> with aliasing constructor.
    components[std::type_index(typeid(T))] =
        std::shared_ptr<void>(compPtr, compPtr.get());
}

template<typename T>
std::shared_ptr<T> Entity::getComponent() {
    auto it = components.find(std::type_index(typeid(T)));
    if (it != components.end()) {
        // Cast shared_ptr<void> back to shared_ptr<T>.
        return std::static_pointer_cast<T>(it->second);
    }
    return nullptr; // Not found.
}

template<typename T>
bool Entity::hasComponent() const {
    return (components.find(std::type_index(typeid(T))) != components.end());
}

template<typename T>
void Entity::removeComponent() {
    components.erase(std::type_index(typeid(T)));
}