#ifndef SENTIMENT_SFMLGUI_H
#define SENTIMENT_SFMLGUI_H

#ifdef BUILD_DLL
#define LIBLINK extern "C" __declspec(dllexport)
#else
#define LIBLINK extern "C" __declspec(dllimport)
#endif

#include "SFML\Window.hpp"
#include ".\Root\Engine\GUI\IGui.h"

class SFMLGui :
    public IGui
{
public:
    class SFMLWindow :
        public IWindow
    {
    public:
        SFMLWindow(INIT_DESC& desc) :
            m_window(sf::VideoMode(desc.width, desc.height), desc.windowName)
            {
                m_window.setActive(false);
            }

        bool isOpen();

        void close();

        void execute();

        void* get_handle();

    private:
        sf::Window m_window;
    };

public:
    std::shared_ptr<IWindow> CreateWindow(IWindow::INIT_DESC& desc);

};

LIBLINK void CreateGui(std::shared_ptr<IGui>& guiObj);


#endif
