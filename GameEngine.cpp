#include "GameEngine.h"
#include "Scene_menu.h"
#include "Game1.h"

#include <iostream>
#include<fstream>
#include "Game2.h"

// Constructor - Initializes the game engine with assets
GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::LoadAssets(const std::string& path)
{
    std::cout << "Initializing GameEngine with assets from: " << path << std::endl;
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "Failed to open config file: " << path << std::endl;
        return;
    }

    std::string type, name, filePath;
    while (fin >> type >> name >> filePath) {
        if (type == "Font") {
            m_assets.addFont(name, filePath);
        }
        else if (type == "Texture") {
            m_assets.addTexture(name, filePath);
        }
        else if (type == "Sound") {
            m_assets.addSound(name, filePath);
        }
    }

    fin.close();
}

// Initializes assets and any starting scenes
void GameEngine::init(const std::string& path)
{
    LoadAssets(path);

    m_window.create(sf::VideoMode(1280, 720), "Game");
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(false);

    // Register both scenes
    auto menuScene = std::make_shared<Scene_Menu>(this);
    m_currentScene = "Scene_Menu";
    m_sceneMap[m_currentScene] = menuScene;
    changeScene("Scene_Menu", menuScene);
}


// Returns the current active scene
std::shared_ptr<Scene> GameEngine::currentScene()
{
    if (m_sceneMap.find(m_currentScene) != m_sceneMap.end())
    {
        return m_sceneMap[m_currentScene];
    }
    return nullptr;
}

// Main game loop
void GameEngine::run()
{
    while (m_running && m_window.isOpen())
    {
        sUserInput();  // Handle user input
        update();      // Update game logic
        m_window.clear();
        currentScene()->sRender();  // Render the current scene
        m_window.display();
    }
}

// Updates the game logic
void GameEngine::update()
{
    if (currentScene())
    {
        currentScene()->update();

        if (currentScene()->hasEnded())
        {
            currentScene()->onEnd();
        }

    }
}

// Handles user input
void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getKeyActionMap().find(event.key.code) == currentScene()->getKeyActionMap().end()) { continue; }
            const std::string actionType = (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) ? "START" : "END";
            currentScene()->doAction(Action(currentScene()->getKeyActionMap().at(event.key.code), actionType));
        }else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            auto button = event.mouseButton.button;
            if (currentScene()->getMouseActionMap().find(button) == currentScene()->getMouseActionMap().end()) { continue; }
            std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";
            currentScene()->doAction(Action(currentScene()->getMouseActionMap().at(button), actionType));
        }

    }
}

// Changes to a different scene
void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (endCurrentScene)
    {
        m_sceneMap.erase(m_currentScene);
    }

    m_sceneMap[sceneName] = scene;
    m_currentScene = sceneName;
}

// Quits the game
void GameEngine::quit()
{
    m_running = false;
    m_window.close();
}

// Returns a reference to the SFML window
sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

int GameEngine::getDifficulty()
{
    return m_GameDifficulty;
}
void GameEngine::setDifficulty(int value)
{
    m_GameDifficulty = value;
}

// Returns a reference to the game assets
const Assets& GameEngine::assets() const
{
    return m_assets;
}

// Returns whether the game is still running
bool GameEngine::isRunning()
{
    return m_running;
}
