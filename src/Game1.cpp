#include "Game1.h"
#include "GameEngine.h"

#define M_PI 3.14159265358979323846

#include<iostream>
#include<fstream>
#include <random>
#include "Scene_menu.h"

const std::string HIGH_SCORE_FILE = "assets/highscore.txt";

Game1::Game1(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Game1::update() 
{
	m_entities.Update();
	if (!m_paused)
	{
		sUserInput();
		sEnemySpawner();
		sMovement();
		sCollision();
		sLifeSpan();
		m_currentFrame++;
		m_lastSpecialTime++;
	}
}

int loadHighScore() {
	std::ifstream inFile(HIGH_SCORE_FILE);
	int highScore = 0;

	if (inFile.is_open()) {
		inFile >> highScore;
		inFile.close();
	}

	return highScore;
}

void saveHighScore(int highScore) {
	std::ofstream outFile(HIGH_SCORE_FILE);
	if (outFile.is_open()) {
		outFile << highScore;
		outFile.close();
	}
	else {
		std::cerr << "Error: Unable to save high score!" << std::endl;
	}
}

void Game1::run()
{

}

void Game1::init()
{
	m_HighScore = loadHighScore();

	m_hasEnded = false;

	keyRegisterAction(sf::Keyboard::S, "DOWN");
	keyRegisterAction(sf::Keyboard::W, "UP");
	keyRegisterAction(sf::Keyboard::D, "RIGHT");
	keyRegisterAction(sf::Keyboard::A, "LEFT");
	keyRegisterAction(sf::Keyboard::Escape, "ESCAPE");
	mouseRegisterAction(sf::Mouse::Button::Right, "M_RIGHT");
	mouseRegisterAction(sf::Mouse::Button::Left, "M_LEFT");

	spawnPlayer();
	m_text.setFont(m_game->assets().getFont("Tech"));
	m_DiffText.setFont(m_game->assets().getFont("Tech"));
}

void Game1::onEnd()
{
	std::cout << "Game Over!" << std::endl;
}

void Game1::spawnPlayer()
{
	//todo: finish all properties of the player with the correct values from the config

	//we create every entity by calling EntityManager.addEntity(tag)
	//This returns a std::shared_ptr<Entity> so e use auto to save typing
	auto entity = m_entities.addEntity("player");

	float mx = m_game->window().getSize().x / 2.0f;
	float my = m_game->window().getSize().y / 2.0f;
	//Give this entity a Transform so it spawns at (200,200) with velocity(0,0) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);
	
	//radius 32, 8 sides, dark grey fill, red outline, 
	entity->cShape = std::make_shared<CShape>(32, 8, sf::Color(10,10,10), sf::Color(255,0,0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(entity->cShape->circle.getRadius()-2);
	m_player = entity;
}

void Game1::spawnEnemy()
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
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(sx, sy), 0.0f);
	entity->cShape = std::make_shared<CShape>(radius,ver, sf::Color(0, 0, 0), sf::Color(0, 0, 255), 4);
	entity->cCollision = std::make_shared<CCollision>(radius - 2);
	entity->cScore = std::make_shared<CScore>(10);

	// Record the spawn time of the enemy
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game1::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	int numPoints = e->cShape->circle.getPointCount();
	float radius = 30.0f; // Distance from the center for spawning
	for (int i = 0; i < 360; i += (360 / numPoints))
	{
		// Convert degrees to radians
		float rad = i * (M_PI / 180.0f);

		// Calculate the offset position for the small enemy
		float offsetX = cos(rad) * radius;
		float offsetY = sin(rad) * radius;

		// Create the small enemy
		auto SEnemy = m_entities.addEntity("SmallEnemy");
		SEnemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius()/2, e->cShape->circle.getPointCount(), sf::Color(e->cShape->circle.getFillColor()), sf::Color(e->cShape->circle.getOutlineColor()), e->cShape->circle.getOutlineThickness()/2);
		SEnemy->cCollision = std::make_shared<CCollision>(SEnemy->cShape->circle.getRadius()-2);
		SEnemy->cLifespan = std::make_shared<CLifespan>(100);
		SEnemy->cScore = std::make_shared<CScore>(100);

		// Offset the position from the center of the original enemy
		Vec2 spawnPos = Vec2(e->cTransform->pos.x + offsetX, e->cTransform->pos.y + offsetY);
		Vec2 direction = Vec2(cos(rad), sin(rad)); // Move in the direction of the offset

		SEnemy->cTransform = std::make_shared<CTransform>(spawnPos, direction, 0);
	}
}

void Game1::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target)
{

	auto bullet = m_entities.addEntity("bullet");
	float dx = target.x - entity->cTransform->pos.x;
	float dy = target.y - entity->cTransform->pos.y;
	float l = sqrt(dx* dx + dy * dy);
	float s = 10;
	bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(s * dx / l,s * dy / l), 0);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 2);
	bullet->cCollision = std::make_shared<CCollision>(10);
	bullet->cLifespan = std::make_shared<CLifespan>(100);
}

void Game1::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	m_lastSpecialTime = 0;
	int numPoints = m_player->cShape->circle.getPointCount() + 10;
	float radius = 30.0f; // Distance from the center for spawning
	for (int i = 0; i < 360; i += (360 / numPoints))
	{
		// Convert degrees to radians
		float rad = i * (M_PI / 180.0f);

		// Calculate the offset position for the small enemy
		float offsetX = cos(rad) * radius;
		float offsetY = sin(rad) * radius;

		// Create the small enemy
		auto SEnemy = m_entities.addEntity("bullet");
		SEnemy->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius() / 2, entity->cShape->circle.getPointCount(), sf::Color(entity->cShape->circle.getFillColor()), sf::Color(entity->cShape->circle.getOutlineColor()), entity->cShape->circle.getOutlineThickness() / 2);
		SEnemy->cCollision = std::make_shared<CCollision>(SEnemy->cShape->circle.getRadius() - 2);
		SEnemy->cLifespan = std::make_shared<CLifespan>(100);
		SEnemy->cScore = std::make_shared<CScore>(100);

		float s = 10;

		// Offset the position from the center of the original enemy
		Vec2 spawnPos = Vec2(entity->cTransform->pos.x + offsetX, entity->cTransform->pos.y + offsetY);
		Vec2 direction = Vec2(cos(rad)*s, sin(rad)*s); // Move in the direction of the offset

		SEnemy->cTransform = std::make_shared<CTransform>(spawnPos, direction, 0);
	}
}

void Game1::sMovement()
{
	if (m_player->cTransform->pos.x <= m_player->cShape->circle.getRadius())
	{
		m_player->cTransform->pos.x = m_player->cShape->circle.getRadius();
	}
	if (m_player->cTransform->pos.x >= m_game->window().getSize().x - m_player->cShape->circle.getRadius())
	{
		m_player->cTransform->pos.x = m_game->window().getSize().x - m_player->cShape->circle.getRadius();
	}
	if (m_player->cTransform->pos.y <= m_player->cShape->circle.getRadius())
	{
		m_player->cTransform->pos.y = m_player->cShape->circle.getRadius();
	}
	if (m_player->cTransform->pos.y >= m_game->window().getSize().y - m_player->cShape->circle.getRadius())
	{
		m_player->cTransform->pos.y = m_game->window().getSize().y - m_player->cShape->circle.getRadius();
	}
		
	for (auto & e : m_entities.getEntities())
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}
}

void Game1::sLifeSpan()
{
	for (auto& e : m_entities.getEntities())
	{
		// Only process SmallEnemy and bullet
		if (e->tag() == "SmallEnemy" || e->tag() == "bullet")
		{
			if (e->cLifespan->remaining > 0)
			{
				float alpha = static_cast<float>(e->cLifespan->remaining) / e->cLifespan->total;
				sf::Uint8 alphaValue = static_cast<sf::Uint8>(255 * alpha);

				// Get current colors
				sf::Color fillColor = e->cShape->circle.getFillColor();
				sf::Color outlineColor = e->cShape->circle.getOutlineColor();

				// Update alpha for both fill and outline
				fillColor.a = alphaValue;
				outlineColor.a = alphaValue;

				e->cShape->circle.setFillColor(fillColor);
				e->cShape->circle.setOutlineColor(outlineColor);
				e->cLifespan->remaining--;
			}

			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
			}
		}
	}
	
}

void Game1::sCollision()
{
	// Check bullet collisions with enemies and small enemies
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			float bex = b->cTransform->pos.x - e->cTransform->pos.x;
			float bey = b->cTransform->pos.y - e->cTransform->pos.y;
			float BEdistanceSquared = bex * bex + bey * bey;
			float BEcombinedRadius = b->cCollision->radius + e->cCollision->radius;

			if (BEdistanceSquared < BEcombinedRadius * BEcombinedRadius)
			{
				e->destroy();
				b->destroy();
				spawnSmallEnemies(e);
				m_score += e->cScore->score;
				if (m_score > m_HighScore) m_HighScore = m_score;
				saveHighScore(m_HighScore);
				break;
			}
		}

		for (auto& s : m_entities.getEntities("SmallEnemy"))
		{
			float bsx = b->cTransform->pos.x - s->cTransform->pos.x;
			float bsy = b->cTransform->pos.y - s->cTransform->pos.y;
			float BSdistanceSquared = bsx * bsx + bsy * bsy;
			float BScombinedRadius = b->cCollision->radius + s->cCollision->radius;

			if (BSdistanceSquared < BScombinedRadius * BScombinedRadius)
			{
				s->destroy();
				b->destroy();
				m_score += s->cScore->score;
				if (m_score > m_HighScore) m_HighScore = m_score;
				saveHighScore(m_HighScore);
				break;
			}
		}
	}

	// Check player collisions with enemies
	for (auto& p : m_entities.getEntities("player"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			float pex = p->cTransform->pos.x - e->cTransform->pos.x;
			float pey = p->cTransform->pos.y - e->cTransform->pos.y;
			float PEdistanceSquared = pex * pex + pey * pey;
			float PEcombinedRadius = p->cCollision->radius + e->cCollision->radius;

			if (PEdistanceSquared < PEcombinedRadius * PEcombinedRadius)
			{
				e->destroy();
				float startX = m_game->window().getSize().x / 2.0f;
				float startY = m_game->window().getSize().y / 2.0f;
				p->cTransform->pos.x = startX;
				p->cTransform->pos.y = startY;
				p->cTransform->velocity.x = 0;
				p->cTransform->velocity.y = 0;
				m_score = 0;
				break;
			}
		}

		// Check player collisions with small enemies
		for (auto& s : m_entities.getEntities("SmallEnemy"))
		{
			float psx = p->cTransform->pos.x - s->cTransform->pos.x;
			float psy = p->cTransform->pos.y - s->cTransform->pos.y;
			float PSdistanceSquared = psx * psx + psy * psy;
			float PScombinedRadius = p->cCollision->radius + s->cCollision->radius;
			if (PSdistanceSquared < PScombinedRadius * PScombinedRadius)
			{
				s->destroy();

				float startX = m_game->window().getSize().x / 2.0f;
				float startY = m_game->window().getSize().y / 2.0f;
				p->cTransform->pos.x = startX;
				p->cTransform->pos.y = startY;
				p->cTransform->velocity.x = 0;
				p->cTransform->velocity.y = 0;
				m_score = 0;
				break;
			}
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

void Game1::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime == 240 - m_game->getDifficulty() * 15)
	{
		spawnEnemy();
	}
}

void Game1::sRender()
{
	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_game->window().draw(e->cShape->circle);
	}
	m_text.setString(" High score: " + std::to_string(m_HighScore) + " Score: " + std::to_string(m_score));
	m_DiffText.setPosition(0, 40);
	m_DiffText.setString(" Difficulty: " + std::to_string(m_game->getDifficulty()));
	m_game->window().draw(m_text);
	m_game->window().draw(m_DiffText);
}

void Game1::sUserInput()
{
	Vec2 velocity(0, 0);
	float speed = 3.5f;

	if (m_player->cInput->up)    velocity.y -= speed;
	if (m_player->cInput->down)  velocity.y += speed;
	if (m_player->cInput->left)  velocity.x -= speed;
	if (m_player->cInput->right) velocity.x += speed;

	m_player->cTransform->velocity = velocity;
}

void Game1::sDoAction(const Action& action)
{
	if (action.name() == "UP") m_player->cInput->up = (action.type() == "START");
	if (action.name() == "DOWN") m_player->cInput->down = (action.type() == "START");
	if (action.name() == "LEFT") m_player->cInput->left = (action.type() == "START");
	if (action.name() == "RIGHT") m_player->cInput->right = (action.type() == "START");
	if (action.name() == "ESCAPE")
	{
		m_hasEnded = true;
		m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
	}
	if (action.name() == "M_LEFT" && action.type() == "START")
	{
		Vec2 mousePos(sf::Mouse::getPosition(m_game->window()).x, sf::Mouse::getPosition(m_game->window()).y);
		spawnBullet(m_player, mousePos);
	}

	if (action.name() == "M_RIGHT" && action.type() == "START")
	{
		if (m_lastSpecialTime > 200 + m_game->getDifficulty() * 200)
		{
			spawnSpecialWeapon(m_player);
		}
	}
}
