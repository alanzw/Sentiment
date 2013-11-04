#include "Engine.h"
#include <iostream>

Engine::Engine(void* moduleHandle)
{
    m_End = false;
    m_GuiLib = LoadLib(".\\bin\\Debug\\Sentiment_SFMLGui");

    if(m_GuiLib != nullptr)
    {
        GUI_CONSTRUCTOR GuiConstructor = (GUI_CONSTRUCTOR) GetFunction(m_GuiLib, "CreateGui");
        if(GuiConstructor != nullptr)
            GuiConstructor(m_GuiManager);
        else
            throw std::runtime_error("Could not find token 'CreateGui' in Sentiment_SFMLGui.dll");
    }
    else
        throw std::runtime_error("Could not open Sentiment_SFMLGui.dll");

    m_RenderLib = LoadLib(".\\bin\\Debug\\Sentiment_OGL4-3Renderer");

    if(m_RenderLib != nullptr)
    {
        RENDERER_CONSTRUCTOR renderConstructor = (RENDERER_CONSTRUCTOR) GetFunction(m_RenderLib, "CreateRendererAndUtility");
        if(renderConstructor != nullptr)
            renderConstructor(m_Renderer, m_RenderUtility);
        else
            throw std::runtime_error("Could not find token 'CreateRendererAndUtility' in Sentiment_OGL4-3Renderer.dll");
    }
    else
        throw std::runtime_error("could not open Sentiment_OGL4-3Renderer");

    m_GameLib = LoadLib(".\\bin\\Debug\\game");
    if(m_GameLib != nullptr)
    {
        GAME_CONSTRUCTOR gameConstructor = (GAME_CONSTRUCTOR) GetFunction(m_GameLib, "CreateGame");
        if(gameConstructor != nullptr)
            gameConstructor(m_Game, this);
        else
            throw std::runtime_error("Could not find token 'CreateGame' in game.dll");
    }
    else
        throw std::runtime_error("Could not open game.dll");

    m_Game->Initialize();
    m_Game->Activate();
}

Engine::~Engine()
{
    m_Game.reset();
    m_GuiManager.reset();
    m_Renderer.reset();
    m_RenderUtility.reset();

    if(m_GameLib != nullptr)
    {
        UnloadLib(m_GameLib);
        m_GameLib = nullptr;
    }

    if(m_GuiLib != nullptr)
    {
        UnloadLib(m_GuiLib);
        m_GuiLib = nullptr;
    }

    if(m_RenderLib != nullptr)
    {
        UnloadLib(m_RenderLib);
        m_RenderLib = nullptr;
    }
}

void Engine::run()
{
    while(!m_End)
    {

    }
}

