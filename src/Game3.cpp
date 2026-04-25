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
	
	spawnPlayers();
}

void Game3::onEnd()
{
	std::cout << "Game Over!" << std::endl;
}

void Game3::spawnPlayers()
{
	auto entity = m_entities.addEntity("player1");

	float mx = m_game->window().getSize().x - 32.0f;
	float my = m_game->window().getSize().y - 32.0f;
	//Give thsi entity a Transform so it spawns at (200,200) with velocity(1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);
	
	//radius 32, 8 sides, dark grey fill, red outline, 
	entity->cShape = std::make_shared<CShape>(32, 8, sf::Color(10,10,10), sf::Color(255,0,0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(entity->cShape->circle.getRadius()-2);
	m_players[0] = entity;

	entity = m_entities.addEntity("player2");

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
	for(int i = 0; i< m_players_count; i++)
	{
			for (auto& p : m_entities.getEntities("player1"))
			{
				for (auto& e : m_entities.getEntities("player2"))
				{
					float pex = p->cTransform->pos.x - e->cTransform->pos.x;
					float pey = p->cTransform->pos.y - e->cTransform->pos.y;
					float PEdistanceSquared = pex * pex + pey * pey;
					float PEcombinedRadius = p->cCollision->radius + e->cCollision->radius;

					if (PEdistanceSquared < PEcombinedRadius * PEcombinedRadius)
					{
						p->cTransform->pos.x = m_game->window().getSize().x - 32.0f;
						p->cTransform->pos.y = m_game->window().getSize().x - 32.0f;
						p->cTransform->velocity.x = 0;
						p->cTransform->velocity.y = 0;
						e->cTransform->pos.x = 32.0f;
						e->cTransform->pos.y = 32.0f;
						e->cTransform->velocity.x = 0;
						e->cTransform->velocity.y = 0;
						std::cout << "Player Collision!" << std::endl;
						break;
					}
				}
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
