#pragma once

#include"Scene.h"
#include"Assets.h"

#include<memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:
	int				 m_GameDifficulty = 0;
	int				 m_maxPlayersServer = 0;
	std::string      m_serverIp = "";
	sf::RenderWindow m_window;
	Assets			 m_assets;
	std::string      m_currentScene;
	SceneMap		 m_sceneMap;
	size_t			 m_simulationSpeed = 1;
	bool			 m_running = true;

	void LoadAssets(const std::string& path);
	void init(const std::string& path);
	void update();

	void sUserInput();

	std::shared_ptr<Scene> currentScene();

public:

	GameEngine(const std::string & path);

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	sf::RenderWindow& window();
	const Assets& assets() const;
	int getDifficulty();
	void setDifficulty(int value);
	int getMaxPlayersServer();
	void setMaxPlayersServer(int value);
	std::string getServerIp();
	void setServerIp(const std::string& ip);
	bool isRunning();
};
