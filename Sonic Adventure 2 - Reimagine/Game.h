
#ifndef GAME_H
#define GAME_H

#include "Root/Engine/Engine.h"
#include "Root/Utility/LoadLib/LoadLib.h"
#include "Root/Game/IGame.h"

class Game :
    public IGame
{
public:
    Game(Engine* engine);

    ~Game();

    void Initialize();

    void Activate();

    void PreDraw();

    void Frame();

    void PostDraw();
};

LIBLINK void CreateGame(std::shared_ptr<IGame>& game, Engine* engine);

#endif

