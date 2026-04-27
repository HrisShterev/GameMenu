#include "Game3.h"
#include "GameEngine.h"

#include<iostream>
#include<fstream>
#include <random>
#include "Scene_menu.h"

Game3::Game3(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Game3::update() 
{
	m_entities.Update();
	if (!m_paused)
	{
		sUserInput();
		sMovement();
		sLifeSpan();
		sEnemySpawner();
		sCollision();
		m_currentFrame++;
	}
}

void Game3::run()
{

}

void Game3::init()
{

	m_hasEnded = false;
	m_players_count = 2;

	keyRegisterAction(sf::Keyboard::Up, "1UP");
	keyRegisterAction(sf::Keyboard::Down, "1DOWN");
	keyRegisterAction(sf::Keyboard::Right, "1RIGHT");
	keyRegisterAction(sf::Keyboard::Left, "1LEFT");
	
	keyRegisterAction(sf::Keyboard::S, "2DOWN");
	keyRegisterAction(sf::Keyboard::W, "2UP");
	keyRegisterAction(sf::Keyboard::D, "2RIGHT");
	keyRegisterAction(sf::Keyboard::A, "2LEFT");
	
	keyRegisterAction(sf::Keyboard::Escape, "ESCAPE");
	
	m_scoreText.setFont(m_game->assets().getFont("Tech"));
	m_scoreText.setPosition(m_game->window().getSize().x / 2 - 160, 0);

	spawnPlayers();
}

void Game3::onEnd()
{
	std::cout << "Game Over!" << std::endl;
}

void Game3::spawnPlayers()
{
	auto entity = m_entities.addEntity("players");

	float mx = m_game->window().getSize().x - 32.0f;
	float my = m_game->window().getSize().y - 32.0f;
	//Give thsi entity a Transform so it spawns at (200,200) with velocity(1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);
	
	//radius 32, 8 sides, dark grey fill, red outline, 
	entity->cShape = std::make_shared<CShape>(32, 8, sf::Color(10,10,10), sf::Color(255,0,0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(entity->cShape->circle.getRadius()-2);
	m_players[0] = entity;

	entity = m_entities.addEntity("players");

	mx = 32.0f;
	my = 32.0f;
	//Give thsi entity a Transform so it spawns at (200,200) with velocity(1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);
	
	//radius 32, 8 sides, dark grey fill, red outline, 
	entity->cShape = std::make_shared<CShape>(32, 8, sf::Color(10,10,10), sf::Color(0,255, 0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(entity->cShape->circle.getRadius()-2);
	m_players[1] = entity;
}

void Game3::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	// Set the enemy's radius
	float radius = 32.0f;

	// Ensure the enemy spawns fully within screen bounds
	int windowWidth = m_game->window().getSize().x;
	int windowHeight = m_game->window().getSize().y;
	float ex = radius + rand() % (windowWidth - 2 * static_cast<int>(radius));
	float ey = radius + rand() % (windowHeight - 2 * static_cast<int>(radius));

	// Setup a random number generator
	static std::random_device rd;
	static std::mt19937 gen(rd());

	// Generate a random velocity between -5 and 5, excluding 0
	std::uniform_int_distribution<int> velocityDist(-3, 3);
	float sx = 0, sy = 0;
	while (sx == 0) sx = velocityDist(gen);
	while (sy == 0) sy = velocityDist(gen);

	// Generate a random number of vertices for the enemy polygon, e.g., between 3 and 10
	std::uniform_int_distribution<int> vertexDist(3, 6);
	sf::Uint8 ver = static_cast<sf::Uint8>(vertexDist(gen));

	// Assign components to the enemy entity
	entity->cLifespan = std::make_shared<CLifespan>(2000);
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(sx, sy), 0.0f);
	entity->cShape = std::make_shared<CShape>(radius,ver, sf::Color(0, 0, 0), sf::Color(0, 0, 255), 4);
	entity->cCollision = std::make_shared<CCollision>(radius - 2);
	entity->cScore = std::make_shared<CScore>(10);

	// Record the spawn time of the enemy
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game3::sLifeSpan()
{
	for (auto& e : m_entities.getEntities("enemy"))
	{
		e->cLifespan->remaining--;
		if (e->cLifespan->remaining <= 0)
		{
			e->destroy();
		}
	}
}

void Game3::sEnemySpawner()
{
	if(m_currentFrame % 100 == 0) 
	{
		m_score[0]++;
		m_score[1]++;
	}

	if (m_currentFrame - m_lastEnemySpawnTime == 200)
	{
		spawnEnemy();
	}
}

void Game3::sMovement()
{
	for(int i = 0; i < m_players_count; i++)
	{
		if (m_players[i]->cTransform->pos.x <= m_players[i]->cShape->circle.getRadius())
		{
			m_players[i]->cTransform->pos.x = m_players[i]->cShape->circle.getRadius();
		}
		if (m_players[i]->cTransform->pos.x >= m_game->window().getSize().x - m_players[i]->cShape->circle.getRadius())
		{
			m_players[i]->cTransform->pos.x = m_game->window().getSize().x - m_players[i]->cShape->circle.getRadius();
		}
		if (m_players[i]->cTransform->pos.y <= m_players[i]->cShape->circle.getRadius())
		{
			m_players[i]->cTransform->pos.y = m_players[i]->cShape->circle.getRadius();
		}
		if (m_players[i]->cTransform->pos.y >= m_game->window().getSize().y - m_players[i]->cShape->circle.getRadius())
		{
			m_players[i]->cTransform->pos.y = m_game->window().getSize().y - m_players[i]->cShape->circle.getRadius();
		}
	}
		
	for (auto & e : m_entities.getEntities())
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}
}

void Game3::sCollision()
{
	float pex = m_players[0]->cTransform->pos.x - m_players[1]->cTransform->pos.x;
	float pey = m_players[0]->cTransform->pos.y - m_players[1]->cTransform->pos.y;
	float PEdistanceSquared = pex * pex + pey * pey;
	float PEcombinedRadius = m_players[0]->cCollision->radius + m_players[1]->cCollision->radius;

	if (PEdistanceSquared < PEcombinedRadius * PEcombinedRadius)
	{
		m_players[0]->cTransform->pos.x = m_game->window().getSize().x - 32.0f;
		m_players[0]->cTransform->pos.y = m_game->window().getSize().x - 32.0f;
		m_players[0]->cTransform->velocity.x = 0;
		m_players[0]->cTransform->velocity.y = 0;
		m_players[1]->cTransform->pos.x = 32.0f;
		m_players[1]->cTransform->pos.y = 32.0f;
		m_players[1]->cTransform->velocity.x = 0;
		m_players[1]->cTransform->velocity.y = 0;
		m_score[0] = 0;
		m_score[1] = 0;
		std::cout << "Player Collision!" << std::endl;
	}

	for (auto& e : m_entities.getEntities("enemy"))
	{
		float pex = m_players[0]->cTransform->pos.x - e->cTransform->pos.x;
		float pey = m_players[0]->cTransform->pos.y - e->cTransform->pos.y;
		float PEdistanceSquared = pex * pex + pey * pey;
		float PEcombinedRadius = m_players[0]->cCollision->radius + e->cCollision->radius;

		float pex1 = m_players[1]->cTransform->pos.x - e->cTransform->pos.x;
		float pey1 = m_players[1]->cTransform->pos.y - e->cTransform->pos.y;
		float PEdistanceSquared1 = pex1 * pex1 + pey1 * pey1;
		float PEcombinedRadius1 = m_players[1]->cCollision->radius + e->cCollision->radius;

		if (PEdistanceSquared < PEcombinedRadius * PEcombinedRadius)
		{
			e->destroy();
			float startX = m_game->window().getSize().x - 32.0f;
			float startY = m_game->window().getSize().x - 32.0f;
			m_players[0]->cTransform->pos.x = startX;
			m_players[0]->cTransform->pos.y = startY;
			m_players[0]->cTransform->velocity.x = 0;
			m_players[0]->cTransform->velocity.y = 0;
			m_score[0] = 0;
			break;
		}

		if (PEdistanceSquared1 < PEcombinedRadius1 * PEcombinedRadius1)
		{
			e->destroy();
			float startX = 32.0f;
			float startY = 32.0f;
			m_players[1]->cTransform->pos.x = startX;
			m_players[1]->cTransform->pos.y = startY;
			m_players[1]->cTransform->velocity.x = 0;
			m_players[1]->cTransform->velocity.y = 0;
			m_score[1] = 0; 
			break;
		}
	}

	for (auto& e : m_entities.getEntities("enemy"))
	{
		float radius = e->cShape->circle.getRadius();
		float winWidth = m_game->window().getSize().x;
		float winHeight = m_game->window().getSize().y;

		if (e->cTransform->pos.x <= radius || e->cTransform->pos.x >= winWidth - radius)
		{
			e->cTransform->velocity.x = -e->cTransform->velocity.x; 
		}

		if (e->cTransform->pos.y <= radius || e->cTransform->pos.y >= winHeight - radius)
		{
			e->cTransform->velocity.y = -e->cTransform->velocity.y;
		}
	}
}

void Game3::sRender()
{
	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_game->window().draw(e->cShape->circle);
	}
	m_scoreText.setString("Score P1: " + std::to_string(m_score[0]) + "   Score P2: " + std::to_string(m_score[1]));
	m_game->window().draw(m_scoreText);
}

void Game3::sUserInput()
{
	for(int i = 0; i< m_players_count; i++)
	{
		Vec2 velocity(0, 0);
		float speed = 3.0f;

		if (m_players[i]->cInput->up)    velocity.y -= speed;
		if (m_players[i]->cInput->down)  velocity.y += speed;
		if (m_players[i]->cInput->left)  velocity.x -= speed;
		if (m_players[i]->cInput->right) velocity.x += speed;

		m_players[i]->cTransform->velocity = velocity;
	}
}

void Game3::sDoAction(const Action& action)
{
	if (action.name() == "1UP") m_players[0]->cInput->up = (action.type() == "START");
	if (action.name() == "1DOWN") m_players[0]->cInput->down = (action.type() == "START");
	if (action.name() == "1LEFT") m_players[0]->cInput->left = (action.type() == "START");
	if (action.name() == "1RIGHT") m_players[0]->cInput->right = (action.type() == "START");
	if (action.name() == "2UP") m_players[1]->cInput->up = (action.type() == "START");
	if (action.name() == "2DOWN") m_players[1]->cInput->down = (action.type() == "START");
	if (action.name() == "2LEFT") m_players[1]->cInput->left = (action.type() == "START");
	if (action.name() == "2RIGHT") m_players[1]->cInput->right = (action.type() == "START");
	if (action.name() == "ESCAPE")
	{
		m_hasEnded = true;
		m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
	}
}
