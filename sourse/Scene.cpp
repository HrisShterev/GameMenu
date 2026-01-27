#include "Scene.h"
#include "GameEngine.h"
#include <iostream>

Scene::Scene() {}

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{
}

// Handles actions performed in the scene
void Scene::doAction(const Action& action)
{
    sDoAction(action);
}

// Runs multiple frames of simulation
void Scene::simulate(const size_t frames)
{
    for (size_t i = 0; i < frames; ++i)
    {
        update();
    }
}

void Scene::mouseRegisterAction(int inputKey, const std::string& actionName)
{
    m_mouseActionMap[inputKey] = actionName;
}

// Registers an action to a specific key input
void Scene::keyRegisterAction(int key, const std::string& name)
{
    m_keyActionMap[key] = name;
}

// Returns the current scene width (if stored in GameEngine)
size_t Scene::width() const
{
    return m_game ? m_game->window().getSize().x : 0;
}

// Returns the current scene height (if stored in GameEngine)
size_t Scene::height() const
{
    return m_game ? m_game->window().getSize().y : 0;
}

// Returns the current frame count
size_t Scene::currentFrame() const
{
    return m_currentFrame;
}

// Checks if the scene has ended
bool Scene::hasEnded() const
{
    return m_hasEnded;
}

// Retrieves the action map
const KeyActionMap& Scene::getKeyActionMap() const
{
    return m_keyActionMap;
}
const MouseActionMap& Scene::getMouseActionMap() const
{
    return m_mouseActionMap;
}

// Draws a line between two points (assuming SFML is used)
void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
    if (!m_game) return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x, p1.y), sf::Color::White),
        sf::Vertex(sf::Vector2f(p2.x, p2.y), sf::Color::White)
    };

    m_game->window().draw(line, 2, sf::Lines);  // Assuming `GameEngine` has `getWindow()`
}
