#include "Sentiment_SFMLGui.h"

void CreateGui(std::shared_ptr<IGui>& guiObj)
{
    guiObj.reset(new SFMLGui);
}

std::shared_ptr<IGui::IWindow> SFMLGui::CreateWindow(IGui::IWindow::INIT_DESC& desc)
{
    return std::shared_ptr<IGui::IWindow>(new SFMLWindow(desc));
}

bool SFMLGui::SFMLWindow::isOpen() {return m_window.isOpen();}

void SFMLGui::SFMLWindow::close() {m_window.close();}

void SFMLGui::SFMLWindow::execute()
{
    sf::Event event;
    m_window.pollEvent(event);

    if(event.type == sf::Event::Closed)
    {
        m_window.close();
    }
}

void* SFMLGui::SFMLWindow::get_handle()
{
    return m_window.getSystemHandle();
}
