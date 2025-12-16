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
	std::vector <sf::Text>	 m_difficultyText;
	size_t					 m_selectedMenuIndex = 0;
	size_t					 m_selectedDifficultyIndex = 0;
	bool					 m_onDif = false;
	size_t					 m_GameIndex = 0;

	void init();
	void onEnd();

public:

	Scene_Menu(GameEngine * gameEngine = nullptr);
	void update() override;
	void sRender();
	void sDoAction(const Action& action);
};
