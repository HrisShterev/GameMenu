#pragma once
#include <SFML/Network.hpp>
#include "Assets.h"
#include "Scene.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include <memory>
#include <map>

class Game2 : public Scene {
    std::map<int, std::shared_ptr<Entity>> bullets;

    sf::UdpSocket socket;
    sf::IpAddress serverIp = "192.168.100.107";
    unsigned short serverPort = 54000;

    int playerId = -1;
    bool joinRequest = false;
    bool startGame = false;

    std::map<int, std::shared_ptr<Entity>> enemies;
    std::map<int, sf::Text> enemiesHealth;

    EntityManager    m_entities;
    sf::Text         m_gameInfo;
    sf::Clock        m_fpsClock;
    int              m_currentFrame = 0;
    int              m_fps = 0;
    float            currentTime = 0;

    sf::Clock        pingClock;
    sf::Time         lastPongTime;
    sf::Time         lastPingTime;
    int              m_pingMs = -1;

    std::shared_ptr<Entity> m_player;

    void sPing();
    void spawnPlayer();
    void sMovement();
    void sCollision();
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target);
    void spawnEnemyBullet(std::shared_ptr<Entity> entity, const Vec2& target);
    void spawnEnemy(const int& id, const Vec2& pos);
    void sLifeSpan();

    void sReceive();
    void sSend();
    void sSend(const Vec2& target);

    void sUserInput();

public:
    Game2(GameEngine* gameEngine = nullptr);
    ~Game2();

    void init();
    void update() override;

    void sDoAction(const Action& action) override;

    void sRender() override;

    void onEnd() override;
};
