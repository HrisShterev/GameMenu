#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "Scene.h"
#include "GameEngine.h"

enum PacketType
{
    JoinRequest = 0,
    PlayerUpdate = 1,
    BulletSpawn = 2,
    AssignId = 3,
    MaxPlayers = 4,
    StartGame = 5,
    DisconectedUser = 6,
    Ping = 7,
    Pong = 8,
    PlayerKilled = 9,
};


class Server : public Scene
{
private:


    struct Player {
        int id;
        sf::Vector2f position;
        sf::Color color;
        sf::Clock lastActive;
    };

    struct Bullet {
        Player owner;
        sf::Vector2f position;
    };

    std::map<std::pair<sf::IpAddress, unsigned short>, Player> players;
    std::vector<Bullet> bullets;

    sf::UdpSocket socket;
    int nextId = 1;
    int maxPpl;
    int curPpl = 0;
    bool isRunning = false;
    bool startGame = false;
    bool startGamePending = false;
    bool startGameSent = false;

public:
    Server(GameEngine* gameEngine = nullptr);
    ~Server();

    void init();
    void update() override;

    void sDoAction(const Action& action) override;

    void sRender() override;

    void onEnd() override;
};
