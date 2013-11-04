#include "Game.h"

Game::Game(Engine* engine) :
    IGame(engine)
{

}

Game::~Game()
{

}

void Game::Initialize()
{
    std::shared_ptr<IGui::IWindow> m_MainWindow;
    std::shared_ptr<IRenderUtility::IRenderContext> m_Context;
    // BIG OL' TEST

    IGui::IWindow::INIT_DESC desc;

    desc.windowName = "My App";
    desc.width = 800;
    desc.height = 600;

    m_MainWindow = m_Engine->Gui()->CreateWindow(desc);

    IRenderUtility::IRenderContext::INIT_DESC desc1;
    desc1.antialiasLevel = 4;
    desc1.depthBits = 24;
    desc1.stencilBits = 8;
    desc1.window = m_MainWindow;

    m_Context = m_Engine->RenderUtility()->CreateRenderContext(desc1);

    m_Engine->Renderer()->SetRenderContext(m_Context);

    m_Engine->Renderer()->Begin(0.5f, 0.0f, 0.5f, 1.0f);
    m_Engine->Renderer()->End();
}

void Game::Activate()
{

}

void Game::PreDraw()
{
}

void Game::Frame()
{

}

void Game::PostDraw()
{

}

void CreateGame(std::shared_ptr<IGame>& game, Engine* engine)
{
    game.reset(new Game(engine));
}
