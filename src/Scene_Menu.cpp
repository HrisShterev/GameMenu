#include "Scene_menu.h"
#include "Assets.h"
#include"GameEngine.h"
#include <iostream>
#include "Game1.h"
#include "Game2.h"
#include "Server.h"

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{
    const auto& font = m_game->assets().getFont("Tech");
    m_title = "Main Menu";

    keyRegisterAction(sf::Keyboard::Down, "DOWN");
    keyRegisterAction(sf::Keyboard::Up, "UP");
    keyRegisterAction(sf::Keyboard::Right, "RIGHT");
    keyRegisterAction(sf::Keyboard::Left, "LEFT");
    keyRegisterAction(sf::Keyboard::Enter, "ENTER");
    keyRegisterAction(sf::Keyboard::Escape, "ESCAPE");
    // Example menu options
    m_menuStrings.push_back("Start Game1");
    m_menuStrings.push_back("Start Game2");
    m_menuStrings.push_back("Exit");

    m_difficultyText.resize(4);


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

    m_serverOrClientText.resize(2);

    m_serverOrClientText[0].setString("Server");
    m_serverOrClientText[1].setString("Client");

    for (auto& text : m_serverOrClientText)
    {
        text.setFont(font);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
    }

    m_titleText.setFont(font);
    m_titleText.setString(m_title);
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setPosition(100, 50);

    m_inputText.setFont(m_game->assets().getFont("Tech"));
    m_inputText.setCharacterSize(36);
    m_inputText.setFillColor(sf::Color::White);
    m_inputText.setPosition(100, 150);
}

void Scene_Menu::update()
{

}

void Scene_Menu::sRender()
{
    m_game->window().draw(m_titleText);

    if (!m_selectedServer && !m_selectedClient)
    {
        m_menuText.setFont(m_game->assets().getFont("Tech"));
        m_menuText.setCharacterSize(36);
        m_menuText.setFillColor(sf::Color::White);

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
        if (m_selectedMenuIndex == m_GameIndex)
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
        if (m_selectedMenuIndex == m_multiplayerIndex)
        {
            for (auto i = 0; i < m_serverOrClientText.size(); i++)
            {
                m_serverOrClientText.at(i).setFont(m_game->assets().getFont("Tech"));
                m_serverOrClientText.at(i).setCharacterSize(36);
                m_serverOrClientText.at(i).setPosition(320, 200 + i * 50);


                if (i == m_selectedMultiplayerIndex && m_onMul)
                {
                    m_serverOrClientText.at(i).setFillColor(sf::Color::Red);
                }
                else
                {
                    m_serverOrClientText.at(i).setFillColor(sf::Color::White);
                }

                m_game->window().draw(m_serverOrClientText.at(i));
            }
        }
    }
    else
    {
        if (m_selectedServer)
        {
            m_inputText.setString("Max players: " + m_inputBuffer);
            m_game->window().draw(m_inputText);

        }
        else if (m_selectedClient)
        {
            m_inputText.setString("Server IP: " + m_inputBuffer);
            m_game->window().draw(m_inputText);
		}
    }
}

void Scene_Menu::sDoAction(const Action& action)
{
    if (!m_selectedServer && !m_selectedClient)
    {
        if (action.type() == "START")
        {
            if (action.name() == "UP")
            {
                if (m_onDif)
                {
                    if (m_selectedDifficultyIndex > 0) m_selectedDifficultyIndex--;
                }
                else if (m_onMul)
                {
                    if (m_selectedMultiplayerIndex > 0) m_selectedMultiplayerIndex--;
                }
                else if (m_selectedMenuIndex > m_GameIndex) m_selectedMenuIndex--;

            }
            else if (action.name() == "DOWN")
            {
                if (m_onDif)
                {
                    if (m_selectedDifficultyIndex < m_difficultyText.size() - 1) m_selectedDifficultyIndex++;
                }
                else if (m_onMul)
                {
                    if (m_selectedMultiplayerIndex < m_serverOrClientText.size() - 1) m_selectedMultiplayerIndex++;
                }
                else if (m_selectedMenuIndex < m_menuStrings.size() - 1) m_selectedMenuIndex++;
            }
            else if (action.name() == "RIGHT")
            {
                if (m_selectedMenuIndex == m_GameIndex)
                {
                    m_onDif = true;
                    m_onMul = false;
                }
                else if (m_selectedMenuIndex == m_multiplayerIndex)
                {
                    m_onMul = true;
                    m_onDif = false;
                }
            }

            else if (action.name() == "LEFT")
            {
                m_onDif = false;
                m_onMul = false;
            }
            else if (action.name() == "ENTER")
            {
                if (m_onDif)
                {
                    m_game->setDifficulty(m_selectedDifficultyIndex + 1);
                    m_hasEnded = true;
                }
                else if (m_onMul)
                {
                    if (m_selectedMultiplayerIndex == 0)
                    {
                        m_selectedServer = true;
                    }
                    else if (m_selectedMultiplayerIndex == 1)
                    {
                        m_selectedClient = true;
                    }
                }
                else if (m_selectedMenuIndex == 2)
                {
                    m_game->quit();
                }
            }
        }
    }
    else if (!m_hasEnded && action.name() == "TEXT" && (m_selectedClient || m_selectedServer))
    {
        char c = action.character();

        if (c == '\b') // Backspace
        {
            if (!m_inputBuffer.empty())
                m_inputBuffer.pop_back();
        }
        else if ((c == '\n' || c == '\r') && !m_inputBuffer.empty())
        {
            if (m_selectedClient)
            {
                m_game->setServerIp(m_inputBuffer);
                m_hasEnded = true;
            }
            else if (m_selectedServer)
            {
                if (!m_inputBuffer.empty())
                {
                    try
                    {
                        int max = std::stoi(m_inputBuffer);
                        m_game->setMaxPlayersServer(max);
                        m_hasEnded = true;
                    }
                    catch (...)
                    {
                        m_inputBuffer.clear();
                        std::cout << "Invalid number format for max players." << std::endl;
                    }
                }
            }
        }
        else if (c >= 32 && c < 127) // Printable
        {
            m_inputBuffer += c;
        }
    }
    else if (action.type() == "START" && action.name() == "ESCAPE"
        && (m_selectedClient || m_selectedServer))
    {
        m_selectedClient = false;
        m_selectedServer = false;
		m_inputBuffer.clear();
    }
}

void Scene_Menu::onEnd()
{
    if (m_selectedMenuIndex == 0)
    {
        m_game->changeScene("Game", std::make_shared<Game1>(m_game), true);
    }
    else if (m_selectedMenuIndex == 1)
    {
        if (m_selectedMultiplayerIndex == 0)
        {
            m_game->changeScene("Server", std::make_shared<Server>(m_game), true);
        }
        else if (m_selectedMultiplayerIndex == 1)
        {
            m_game->changeScene("Game", std::make_shared<Game2>(m_game), true);
        }
    }
}


