#pragma once

#include "Action.h"
#include"EntityManager.h"

#include<memory>

class GameEngine;

typedef std::map<int, std::string> KeyActionMap;
typedef std::map<int, std::string> MouseActionMap;

class Scene
{
protected:

	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	KeyActionMap   m_keyActionMap;
	MouseActionMap m_mouseActionMap;
	bool          m_hasEnded = false;
	size_t        m_currentFrame = 0;

public:

	Scene();
	Scene(GameEngine* gameEngine);
	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	virtual void doAction(const Action& action);
	void simulate(const size_t frames);
	void keyRegisterAction(int inputKey, const std::string& actionName);
	void mouseRegisterAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	virtual void onEnd() = 0;
	bool hasEnded() const;
	const KeyActionMap& getKeyActionMap() const;
	const MouseActionMap& getMouseActionMap() const;
	void drawLine(const Vec2& p1, const Vec2& p2);
};