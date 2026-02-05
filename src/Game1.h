#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"

#include <SFML/Graphics.hpp>

class Game1 : public Scene
{
	EntityManager       m_entities;
	sf::Text			m_text;
	sf::Text			m_DiffText;
	int					m_score = 0;
	int					m_HighScore = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	int					m_lastSpecialTime = 0;
	bool				m_paused = false;

	std::shared_ptr<Entity> m_player;

	void onEnd();
	void init();

	void sMovement();
	void sDoAction(const Action& action);
	void sLifeSpan();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sUserInput();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

	Game1(GameEngine* gameEngine = nullptr);

	void update() override;
	void run();
	
};