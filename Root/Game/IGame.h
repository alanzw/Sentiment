#ifndef IGAME_H
#define IGAME_H

#include <memory>

class Engine;

class IGame
{
public:
    IGame(Engine* engine) :
        m_Engine(engine)
        {}

    virtual ~IGame() {}

    virtual void Initialize() = 0;

    virtual void Activate() = 0;

    virtual void PreDraw() = 0;

    virtual void Frame() = 0;

    virtual void PostDraw() = 0;

protected:
    Engine* m_Engine;
};

typedef void (*GAME_CONSTRUCTOR)(std::shared_ptr<IGame> & game, Engine* engine);

#endif
