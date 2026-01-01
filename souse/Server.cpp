#include "Server.h"
#include "Scene_menu.h"

void Server::init()
{
	keyRegisterAction(sf::Keyboard::Escape, "QUIT");

    socket.bind(54000);
    socket.setBlocking(false);
    std::cout << "Server running on port 54000\n";
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
    static sf::Clock tick;
    if (tick.getElapsedTime().asMilliseconds() < 16)
        return;
    tick.restart();

    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short port;

    // PROCESS ALL INCOMING PACKETS THIS FRAME
    while (socket.receive(packet, sender, port) == sf::Socket::Done)
    {
        int packetType;
        packet >> packetType;

        if (packetType == PacketType::JoinRequest)
        {
            float x, y;
            int clientTempId;
            packet >> clientTempId >> x >> y;

            auto key = std::make_pair(sender, port);

            if (curPpl >= maxPpl)
            {
                std::cout << "Max players reached! Rejecting client.\n";
                sf::Packet rejectPacket;
                rejectPacket << PacketType::MaxPlayers;
                socket.send(rejectPacket, sender, port);
                continue;
            }

            // New player
            if (players.find(key) == players.end())
            {
                curPpl++;

                Player p;
                p.id = nextId++;
                p.position = { x, y };
                p.lastActive.restart();

                players[key] = p;

                std::cout << "Player connected! ID = " << p.id << "\n";

                if (startGamePending)
                {
                    startGame = true;
                    startGamePending = false;
                }


                if (curPpl == maxPpl && !startGamePending && !startGame)
                {
                    startGamePending = true;
                }

                // Send ID to client
                sf::Packet idPacket;
                idPacket << PacketType::AssignId << p.id;
                socket.send(idPacket, sender, port);
            }
        }

        if (packetType == PacketType::Ping)
        {

            sf::Packet pong;
            pong << PacketType::Pong;

            sf::Int32 timestamp;
            packet >> timestamp;
            pong << timestamp;

            socket.send(pong, sender, port);
        }
        if (packetType == PacketType::PlayerKilled)
        {

            int id;
            packet >> id;
            std::cout << "Player " << id << " killed" << std::endl;
            for (auto it = players.begin(); it != players.end(); )
            {
                if (it->second.id == id)
                {
                    it = players.erase(it);
                    curPpl--;
                    break;
                }
                else
                {
                    ++it;
                }
            }
        }
        if (packetType == PacketType::PlayerUpdate)
        {
            float x, y;
            int tempId;

            packet >> tempId >> x >> y;

            auto key = std::make_pair(sender, port);

            auto it = players.find(key);
            if (it != players.end())
            {
                it->second.position = { x, y };
                it->second.lastActive.restart();
            }

        }
        if (packetType == PacketType::BulletSpawn)
        {
            Bullet b;
            packet >> b.owner.id >> b.position.x >> b.position.y;

            bullets.push_back(b);
        }
    }
    sf::Packet disconnectPacket;

    // REMOVE INACTIVE PLAYERS
    for (auto it = players.begin(); it != players.end(); )
    {
        if (it->second.lastActive.getElapsedTime().asSeconds() > 10.f)
        {
            disconnectPacket << PacketType::DisconectedUser << it->second.id;

            std::cout << "Player " << it->second.id << " timed out.\n";
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


    // BROADCAST P STATE — ONE PACKET PER CLIENT
    for (auto& p : players)
    {
        sf::Packet world;

        for (auto& pl : players)
        {
            world << PacketType::PlayerUpdate
                << pl.second.id
                << pl.second.position.x
                << pl.second.position.y;
        }

        if (startGame)
        {
            world << PacketType::StartGame;
        }

        for (auto& b : bullets)
        {
            world << PacketType::BulletSpawn
                << b.owner.id
                << b.position.x
                << b.position.y;
        }

        socket.send(world, p.first.first, p.first.second);
    }

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

}

void Server::onEnd()
{

}

