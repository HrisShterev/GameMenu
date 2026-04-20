#include "Server.h"
#include "Scene_menu.h"

void Server::init()
{
    keyRegisterAction(sf::Keyboard::Escape, "QUIT");

    socket.bind(54000);
    socket.setBlocking(false);

    titleText.setFont(m_game->assets().getFont("Tech"));
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setString("Server - port: 54000, max players: " + std::to_string(maxPpl));
    titleText.setOrigin(0.f, 0.f);
    titleText.setPosition(50.f, 50.f);
}

Server::Server(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    maxPpl = m_game->getMaxPlayersServer();
    init();
}

Server::~Server() {}

void Server::update()
{
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;

    // =========================
    // RECEIVE PACKETS
    // =========================
    while (socket.receive(packet, sender, port) == sf::Socket::Done)
    {
        int packetType;
        packet >> packetType;

        auto key = std::make_pair(sender, port);

        // -------- JOIN REQUEST --------
        if (packetType == PacketType::JoinRequest)
        {
            float x, y;
            int clientTempId;
            packet >> clientTempId >> x >> y;

            if (curPpl >= maxPpl)
            {
                sf::Packet reject;
                reject << PacketType::MaxPlayers;
                socket.send(reject, sender, port);
                continue;
            }

            if (players.find(key) == players.end())
            {
                Player p;
                p.id = nextId++;
                p.position = { x, y };
                p.lastActive.restart();

                players[key] = p;
                curPpl++;

                std::cout << "Player connected! ID = " << p.id << "\n";

                if (curPpl == maxPpl && !startGame)
                    startGamePending = true;

                sf::Packet idPacket;
                idPacket << PacketType::AssignId << p.id;

                if(curPpl == maxPpl && startGameSent)
                    idPacket << PacketType::StartGame;;

                socket.send(idPacket, sender, port);
            }
        }

        // -------- PING --------
        else if (packetType == PacketType::Ping)
        {
            sf::Int32 timestamp;
            packet >> timestamp;

            sf::Packet pong;
            pong << PacketType::Pong << timestamp;
            socket.send(pong, sender, port);
        }

        // -------- PLAYER UPDATE --------
        else if (packetType == PacketType::PlayerUpdate)
        {
            float x, y;
            int tempId;
            packet >> tempId >> x >> y;

            auto it = players.find(key);
            if (it != players.end())
            {
                it->second.position = { x, y };
                it->second.lastActive.restart();
            }
        }

        // -------- PLAYER KILLED --------
        else if (packetType == PacketType::PlayerKilled)
        {
            int id;
            packet >> id;

            for (auto it = players.begin(); it != players.end(); ++it)
            {
                if (it->second.id == id)
                {
                    std::cout << "Player " << id << " killed\n";
                    players.erase(it);
                    curPpl--;
                    break;
                }
            }
        }

        // -------- BULLET --------
        else if (packetType == PacketType::BulletSpawn)
        {
            Bullet b;
            packet >> b.owner.id >> b.position.x >> b.position.y;
            bullets.push_back(b);
        }
    }

    // =========================
    // REMOVE TIMED-OUT PLAYERS
    // =========================
    sf::Packet disconnectPacket;

    for (auto it = players.begin(); it != players.end(); )
    {
        if (it->second.lastActive.getElapsedTime().asSeconds() > 10.f)
        {
            disconnectPacket << PacketType::DisconectedUser << it->second.id;
            std::cout << "Player " << it->second.id << " timed out\n";
            it = players.erase(it);
            curPpl--;
        }
        else
        {
            ++it;
        }
    }

    if (disconnectPacket.getDataSize() > 0)
    {
        for (auto& p : players)
            socket.send(disconnectPacket, p.first.first, p.first.second);
    }

    // =========================
    // START GAME DECISION
    // =========================
    if (startGamePending)
    {
        startGame = true;
        startGamePending = false;
    }

    bool sendStartGame = startGame && !startGameSent;

    // =========================
    // BROADCAST WORLD STATE
    // =========================
    for (auto& p : players)
    {
        sf::Packet world;

        // All players
        for (auto& pl : players)
        {
            world << PacketType::PlayerUpdate
                << pl.second.id
                << pl.second.position.x
                << pl.second.position.y;
        }

        // Start game ONCE for ALL clients
        if (sendStartGame)
            world << PacketType::StartGame;

        // Bullets
        for (auto& b : bullets)
        {
            world << PacketType::BulletSpawn
                << b.owner.id
                << b.position.x
                << b.position.y;
        }

        socket.send(world, p.first.first, p.first.second);
    }

    if (sendStartGame)
        startGameSent = true;

    bullets.clear();
}

void Server::sDoAction(const Action& action)
{
    if (action.name() == "QUIT")
    {
        m_hasEnded = true;
        m_game->changeScene("Game", std::make_shared<Scene_Menu>(m_game), true);
    }
}

void Server::sRender() 
{
    m_game->window().draw(titleText);

    for(int i = 0; i < players.size(); i++)
    {
        sf::Text playerText;
        playerText.setFont(m_game->assets().getFont("Tech"));
        playerText.setCharacterSize(36);
        playerText.setFillColor(sf::Color::White);
        playerText.setPosition(100, 150 + i * 50);
        auto it = players.begin();
        std::advance(it, i);
        playerText.setString("Player ID: " + std::to_string(it->second.id) + 
                             " Position: (" + std::to_string(static_cast<int>(it->second.position.x)) + 
                             ", " + std::to_string(static_cast<int>(it->second.position.y)) + ")");
        m_game->window().draw(playerText);
	}
}

void Server::onEnd() {}
