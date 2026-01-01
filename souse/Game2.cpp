#include "Game2.h"
#include"GameEngine.h"
#include"Action.h"
#include "Scene_menu.h"
#include<iostream>
#include "Server.h"
#include <fstream>

Game2::Game2(GameEngine* gameEngine)
    : Scene(gameEngine)
{
	serverIp = m_game->getServerIp();
    init();
}


Game2::~Game2() {}

void Game2::init() {
    keyRegisterAction(sf::Keyboard::Escape, "QUIT");
    keyRegisterAction(sf::Keyboard::S, "DOWN");
    keyRegisterAction(sf::Keyboard::W, "UP");
    keyRegisterAction(sf::Keyboard::D, "RIGHT");
    keyRegisterAction(sf::Keyboard::A, "LEFT");
    mouseRegisterAction(sf::Mouse::Button::Left, "M_LEFT");

    socket.setBlocking(false);

    m_gameInfo.setCharacterSize(24);
    m_gameInfo.setFillColor(sf::Color::White);
    m_gameInfo.setOutlineColor(sf::Color::Black);
    m_gameInfo.setFont(m_game->assets().getFont("Tech"));

    spawnPlayer();
}

void Game2::spawnPlayer()
{
    auto e = m_entities.addEntity("player");

    float mx = 640, my = 360;
    e->cTransform = std::make_shared<CTransform>(Vec2(0, 0), Vec2(mx,my), Vec2(0, 0), 0.f);
    e->cShape = std::make_shared<CShape>(32, 8, sf::Color(10, 10, 10), sf::Color::Red, 4.f);
    e->cInput = std::make_shared<CInput>();
    e->cCollision = std::make_shared<CCollision>(30);
    e->cLifespan = std::make_shared<CLifespan>(10);

    m_player = e;
}

void Game2::sPing()
{
    lastPingTime = pingClock.getElapsedTime();
    sf::Packet packet;
    packet << PacketType::Ping << pingClock.getElapsedTime().asMilliseconds();

    socket.send(packet, serverIp, serverPort);

}

void Game2::sReceive()
{
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;

    if (pingClock.getElapsedTime() - lastPongTime > sf::seconds(5) && m_pingMs != -1)
    {
        std::cout << "Disconected from server!" << std::endl;
        m_pingMs = -1;
    }

    while (socket.receive(packet, sender, port) == sf::Socket::Done)
    {
        while (!packet.endOfPacket())
        {
            int type;
            packet >> type;

            if (type == PacketType::Pong)
            {
                lastPongTime = pingClock.getElapsedTime();
                sf::Int32 sentTime;
                packet >> sentTime;

                sf::Int32 now = pingClock.getElapsedTime().asMilliseconds();
                m_pingMs = now - sentTime;
            }

            if (type == PacketType::DisconectedUser)
            {
                int id;
                packet >> id;
                for (auto& e : m_entities.getEntities("enemy"))
                {
                    if (e->id() == id)
                    {
                        e->destroy();
                        break;
                    }
                }

                enemies.erase(id);
            }

            if (type == PacketType::StartGame)
            {
                startGame = true;
            }

            // MAX PLAYERS
            if (type == PacketType::MaxPlayers && playerId == -1)
            {
                std::cout << "Max players in server!" << std::endl;
                m_hasEnded = true;
                m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
            }

            // ID ASSIGN
            if (type == PacketType::AssignId)
            {
                packet >> playerId;
                std::cout << "My ID = " << playerId << "\n";
                continue;
            }

            // PLAYER UPDATE
            if (type == PacketType::PlayerUpdate)
            {
                int id;
                float x, y;
                packet >> id >> x >> y;

                if (id == playerId)
                    continue;

                Vec2 newPos(x, y);

                auto it = enemies.find(id);
                if (it == enemies.end())
                {
                    spawnEnemy(id, newPos);
                }
                else if (it->second->cTransform)
                {
                    it->second->cTransform->pos = newPos;
                }
            }


            // BULLET
            if (type == PacketType::BulletSpawn && startGame)
            {
                int ownerId;
                float bx, by;
                packet >> ownerId >> bx >> by;

                if (ownerId == playerId)
                    continue;

                if (!enemies.count(ownerId))
                    continue;

                spawnEnemyBullet(enemies[ownerId], Vec2(bx, by));
            }
        }
    }
}



void Game2::sSend(const Vec2& target)
{
    if (playerId == -1) return;

    sf::Packet packet;
    packet << PacketType::BulletSpawn << playerId << target.x << target.y;

    socket.send(packet, serverIp, serverPort);
}


void Game2::sSend()
{
    if (playerId == -1 && !joinRequest)
    {
        joinRequest = true;
        sf::Packet p;
        p << PacketType::JoinRequest << -1
            << m_player->cTransform->pos.x
            << m_player->cTransform->pos.y;

        socket.send(p, serverIp, serverPort);
        return;
    }

    sf::Packet p;
    p << PacketType::PlayerUpdate << playerId
        << m_player->cTransform->pos.x
        << m_player->cTransform->pos.y;

    socket.send(p, serverIp, serverPort);
}


void Game2::sCollision()
{
    auto& players = m_entities.getEntities("player");
    auto& enemyBullets = m_entities.getEntities("enemyBullet");
    auto& bullets = m_entities.getEntities("bullet");
    auto& enemy = m_entities.getEntities("enemy");

    for (auto& p : players)
    {
        if (!p->cTransform || !p->cCollision) continue;


        for (auto& e : enemyBullets)
        {
            if (!e->cTransform || !e->cCollision) continue;

            float dx = p->cTransform->pos.x - e->cTransform->pos.x;
            float dy = p->cTransform->pos.y - e->cTransform->pos.y;
            float distSq = dx * dx + dy * dy;
            float r = p->cCollision->radius + e->cCollision->radius;

            if (distSq < r * r)
            {
                std::cout << "Agartha user has been shot!" << std::endl;

                p->cLifespan->remaining--;
                if (p->cLifespan->remaining <= 0)
                {
                    sf::Packet p;
                    p << PacketType::PlayerKilled << playerId;
                    socket.send(p, serverIp, serverPort);

                    std::cout << "You got killed!" << std::endl;

                    m_hasEnded = true;
                    m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
                }

                e->destroy();
                break;
            }
        }
    }


    for (auto& p : enemy)
    {
        if (!p->cTransform || !p->cCollision) continue;


        for (auto& e : bullets)
        {
            if (!e->cTransform || !e->cCollision) continue;

            float dx = p->cTransform->pos.x - e->cTransform->pos.x;
            float dy = p->cTransform->pos.y - e->cTransform->pos.y;
            float distSq = dx * dx + dy * dy;
            float r = p->cCollision->radius + e->cCollision->radius;

            if (distSq < r * r)
            {
                std::cout << "One less to get to Agartha!" << std::endl;

                p->cLifespan->remaining--;
                if (p->cLifespan->remaining <= 0) p->destroy();

                e->destroy();
                break;
            }
        }

        for (auto& e : enemyBullets)
        {
            if (!e->cTransform || !e->cCollision) continue;

            if (p->id() == e->id()) continue;

            float dx = p->cTransform->pos.x - e->cTransform->pos.x;
            float dy = p->cTransform->pos.y - e->cTransform->pos.y;
            float distSq = dx * dx + dy * dy;
            float r = p->cCollision->radius + e->cCollision->radius;

            if (distSq < r * r)
            {
                std::cout << "They killing each other!" << std::endl;

                p->cLifespan->remaining--;
                if (p->cLifespan->remaining <= 0) p->destroy();

                e->destroy();
                break;
            }
        }
    }
}

void Game2::sRender()
{
    for (auto& e : m_entities.getEntities())
    {
        if (!e->cTransform || !e->cShape) continue;

        e->cShape->circle.setPosition(
            e->cTransform->pos.x,
            e->cTransform->pos.y
        );

        e->cTransform->angle += 1;
        e->cShape->circle.setRotation(e->cTransform->angle);
        m_game->window().draw(e->cShape->circle);
    }

    if (m_player && m_player->cLifespan)
    {
        m_gameInfo.setString(
            "Fps: " + std::to_string(m_fps) +
            "  Ping: " + std::to_string(m_pingMs) +
            " ms  Health: " +
            std::to_string(m_player->cLifespan->remaining)
        );
        m_game->window().draw(m_gameInfo);
    }
}


void Game2::update()
{
    m_entities.Update();
    m_currentFrame++;

    currentTime = m_fpsClock.restart().asSeconds();
    m_fps = 1 / currentTime;

    sPing();
    sReceive();
    sUserInput();
    sMovement();
    sCollision();
    sLifeSpan();

    sSend();
}

void Game2::onEnd()
{
	std::cout << "Game over!" << std::endl;
}

void Game2::sMovement()
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

    for (auto& e : m_entities.getEntities())
    {
        if (!e->cTransform) continue;
        e->cTransform->pos.x += e->cTransform->velocity.x;
        e->cTransform->pos.y += e->cTransform->velocity.y;
    }
}

void Game2::sUserInput()
{
    Vec2 velocity(0, 0);
    float speed = 6.0f;

    if (m_player->cInput->up)    velocity.y -= speed;
    if (m_player->cInput->down)  velocity.y += speed;
    if (m_player->cInput->left)  velocity.x -= speed;
    if (m_player->cInput->right) velocity.x += speed;

    m_player->cTransform->velocity = velocity;
}

void Game2::sDoAction(const Action& action)
{
    if (action.name() == "QUIT")
    {
        m_hasEnded = true;
        m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
    }
    if (action.name() == "UP") m_player->cInput->up = (action.type() == "START");
    if (action.name() == "DOWN") m_player->cInput->down = (action.type() == "START");
    if (action.name() == "LEFT") m_player->cInput->left = (action.type() == "START");
    if (action.name() == "RIGHT") m_player->cInput->right = (action.type() == "START");
    if (action.name() == "M_LEFT" && action.type() == "START" && startGame)
    {
        Vec2 mousePos(sf::Mouse::getPosition(m_game->window()).x, sf::Mouse::getPosition(m_game->window()).y);
        spawnBullet(m_player, mousePos);
        sSend(mousePos);
    }
}

void Game2::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    auto bullet = m_entities.addEntity("bullet");
    float dx = target.x - entity->cTransform->pos.x;
    float dy = target.y - entity->cTransform->pos.y;
    float l = sqrt(dx * dx + dy * dy);
    if (l == 0) return;
    float s = 10;
    bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(s * dx / l, s * dy / l), 0);
    bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 2);
    bullet->cCollision = std::make_shared<CCollision>(10);
    bullet->cLifespan = std::make_shared<CLifespan>(100);
}

void Game2::spawnEnemyBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    auto bullet = m_entities.addEntity(entity->id(), "enemyBullet");
    float dx = target.x - entity->cTransform->pos.x;
    float dy = target.y - entity->cTransform->pos.y;
    float l = sqrt(dx * dx + dy * dy);
    if (l == 0) return;
    float s = 10;
    bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(s * dx / l, s * dy / l), 0);
    bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(0, 0, 0), sf::Color::Yellow, 2);
    bullet->cCollision = std::make_shared<CCollision>(10);
    bullet->cLifespan = std::make_shared<CLifespan>(100);
}

void Game2::sLifeSpan()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->tag() == "bullet" || e->tag() == "enemyBullet")
        {
            if (!e->cLifespan || !e->cShape) continue;

            if (e->cLifespan->remaining > 0)
            {
                float alpha =
                    static_cast<float>(e->cLifespan->remaining) /
                    e->cLifespan->total;

                sf::Uint8 a = static_cast<sf::Uint8>(255 * alpha);

                auto fill = e->cShape->circle.getFillColor();
                auto out = e->cShape->circle.getOutlineColor();
                fill.a = out.a = a;

                e->cShape->circle.setFillColor(fill);
                e->cShape->circle.setOutlineColor(out);
                e->cLifespan->remaining--;
            }

            if (e->cLifespan->remaining <= 0)
            {
                e->destroy();
            }
        }
    }
}


void Game2::spawnEnemy(const int& id, const Vec2& pos)
{
    auto e = m_entities.addEntity(id, "enemy");
    e->cShape = std::make_shared<CShape>(
        32, 8, sf::Color(0, 0, 0), sf::Color::Yellow, 4.f);
    e->cTransform =
        std::make_shared<CTransform>(pos, Vec2(0,0), Vec2(0, 0), 0);
    e->cCollision = std::make_shared<CCollision>(30);
    e->cLifespan = std::make_shared<CLifespan>(10);
    enemies[id] = e;
}