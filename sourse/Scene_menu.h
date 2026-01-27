#pragma once

#include"Scene.h"
#include<map>
#include<memory>
#include<deque>
#include<vector>

#include "EntityManager.h"

class Scene_Menu : public Scene
{
protected:

	std::string				 m_title;
	std::vector<std::string> m_menuStrings;
	sf::Text				 m_menuText;
	sf::Text				 m_titleText;
	std::vector <sf::Text>	 m_difficultyText;
	std::vector <sf::Text>	 m_serverOrClientText;
	size_t					 m_selectedMenuIndex = 0;
	size_t					 m_selectedDifficultyIndex = 0;
	size_t					 m_selectedMultiplayerIndex = 0;
	bool					 m_onDif = false;
	bool					 m_onMul = false;
	bool                     m_selectedServer = false;
	bool                     m_selectedClient = false;
	std::string				 m_inputBuffer;
	sf::Text				 m_inputText;
	const size_t			 m_GameIndex = 0;
	const size_t			 m_multiplayerIndex = 1;

	void init();
	void onEnd();

public:

	Scene_Menu(GameEngine * gameEngine = nullptr);
	void update() override;
	void sRender();
	void sDoAction(const Action& action);
};
