#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class Game2 : public Scene {
public:
    Game2(GameEngine* gameEngine = nullptr);
    ~Game2();

    void init();

    // Implement all pure virtual functions
    void update() override;

    void sDoAction(const Action& action) override;

    void sRender() override;

    void onEnd() override;
};
