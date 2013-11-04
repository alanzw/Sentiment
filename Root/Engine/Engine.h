#ifndef SENTIMENT_ENGINE_H
#define SENTIMENT_ENGINE_H

#include <stdexcept>

#include "Root\Engine\GUI\IGui.h"
#include "Root\Engine\Graphics\IRenderer.h"
#include "Root\Game\IGame.h"

#include "Root\Utility\LoadLib\LoadLib.h"

class Engine
{
public:
    Engine(void* moduleHandle);

    ~Engine();

    void run();

    void stop() {m_End = true;}

    const std::shared_ptr<IGui> Gui() {return m_GuiManager;}

    const std::shared_ptr<IRenderer> Renderer() {return m_Renderer;}

    const std::shared_ptr<IRenderUtility> RenderUtility() {return m_RenderUtility;}

private:
    void * m_GuiLib;
    void * m_RenderLib;
    void * m_GameLib;

    std::shared_ptr<IGui> m_GuiManager;
    std::shared_ptr<IRenderer> m_Renderer;
    std::shared_ptr<IRenderUtility> m_RenderUtility;
    std::shared_ptr<IGame> m_Game;

    bool m_End;
};

#endif
