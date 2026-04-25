#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"

#include <SFML/Graphics.hpp>

class Game3 : public Scene
{
	EntityManager       m_entities;
	int					m_currentFrame = 0;
	int                 m_players_count = 0;
	bool				m_paused = false;

	std::shared_ptr<Entity> m_players [2];

	void onEnd();
	void init();

	void sMovement();
	void sDoAction(const Action& action);
	void sRender();
	void sCollision();
	void sUserInput();

	void spawnPlayers();

public:

	Game3(GameEngine* gameEngine = nullptr);

	void update() override;
	void run();
	
};