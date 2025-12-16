#include "Game2.h"
#include"GameEngine.h"
#include"Action.h"
#include "Scene_menu.h"
#include<iostream>

Game2::Game2(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}


Game2::~Game2() {}

void Game2::init() {
    keyRegisterAction(sf::Keyboard::Escape, "QUIT");

}

void Game2::sRender()
{
    
}

void Game2::update()
{
    m_currentFrame++;
}

void Game2::onEnd()
{

}

void Game2::sDoAction(const Action& action)
{
    if (action.name() == "QUIT")
    {
        m_hasEnded = true;
        m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
    }
}
