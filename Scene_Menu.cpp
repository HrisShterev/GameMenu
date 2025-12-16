#include "Scene_Menu.h"
#include "Assets.h"
#include"GameEngine.h"
#include <iostream>
#include "Game1.h"
#include "Game2.h"

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{
    m_title = "Main Menu";

    m_GameIndex = 0;

    keyRegisterAction(sf::Keyboard::Down, "DOWN");
    keyRegisterAction(sf::Keyboard::Up, "UP");
    keyRegisterAction(sf::Keyboard::Right, "RIGHT");
    keyRegisterAction(sf::Keyboard::Left, "LEFT");
    keyRegisterAction(sf::Keyboard::Enter, "ENTER");
    // Example menu options
    m_menuStrings.push_back("Start Game1");
    m_menuStrings.push_back("Start Game2");
    m_menuStrings.push_back("Exit");

    m_difficultyText.resize(4);
    const auto& font = m_game->assets().getFont("Tech");

    m_difficultyText[0].setString("Easy");
    m_difficultyText[1].setString("Medium");
    m_difficultyText[2].setString("Hard");
    m_difficultyText[3].setString("Impossible");

    for (auto& text : m_difficultyText)
    {
        text.setFont(font);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
    }

    m_menuText.setFont(m_game->assets().getFont("Tech"));  // Make sure font is loaded in Assets
    m_menuText.setCharacterSize(36);    
    m_menuText.setFillColor(sf::Color::White);
}

void Scene_Menu::update()
{

}

void Scene_Menu::sRender()
{
    sf::Text titleText;
    titleText.setFont(m_game->assets().getFont("Tech"));
    titleText.setString(m_title);
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(100, 50);

    m_game->window().draw(titleText);

    for (auto i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition(100, 150 + i * 50);

        // Highlight selected menu option
        if (i == m_selectedMenuIndex)
        {
            m_menuText.setFillColor(sf::Color::Red);
        }
        else
        {
            m_menuText.setFillColor(sf::Color::White);
        }

        m_game->window().draw(m_menuText);
    }
    if (m_selectedMenuIndex == m_GameIndex || m_selectedMenuIndex == -1)
    {
        for (auto i = 0; i < m_difficultyText.size(); i++)
        {
            m_difficultyText.at(i).setFont(m_game->assets().getFont("Tech"));
            m_difficultyText.at(i).setCharacterSize(36);
            m_difficultyText.at(i).setPosition(320, 150 + i * 50);


            if (i == m_selectedDifficultyIndex && m_onDif)
            {
                 m_difficultyText.at(i).setFillColor(sf::Color::Red);
            }
            else
            {
                 m_difficultyText.at(i).setFillColor(sf::Color::White);
            }

            m_game->window().draw(m_difficultyText.at(i));
        }
    }
}

void Scene_Menu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_onDif)
            {
                if (m_selectedDifficultyIndex > 0) m_selectedDifficultyIndex--;
            }
            else if (m_selectedMenuIndex > m_GameIndex) m_selectedMenuIndex--;

        }
        else if (action.name() == "DOWN")
        {
            if (m_onDif)
            {
                if (m_selectedDifficultyIndex < m_difficultyText.size() - 1) m_selectedDifficultyIndex++;
            }
            else if (m_selectedMenuIndex < m_menuStrings.size() - 1) m_selectedMenuIndex++;
        }
        else if (action.name() == "RIGHT")
        {
            if (m_selectedMenuIndex == m_GameIndex)
            {
                m_onDif = true;
            }
        }
        else if (action.name() == "LEFT")
        {
            if (m_onDif)
            {
                m_onDif = false;
            }
        }
        else if (action.name() == "ENTER")
        {
            if (m_onDif)
            {
                m_game->setDifficulty(m_selectedDifficultyIndex + 1);
                m_hasEnded = true;
            }
            else if (m_selectedMenuIndex == 1)
            {
                m_hasEnded = true;
            }
            else if (m_selectedMenuIndex == 2)
            {
                m_game->quit();
            }
        }
    }
}

void Scene_Menu::onEnd()
{
    if(m_selectedMenuIndex == 0) m_game->changeScene("Game", std::make_shared<Game>(m_game), true);
    if(m_selectedMenuIndex == 1) m_game->changeScene("Game", std::make_shared<Game2>(m_game), true);
}

