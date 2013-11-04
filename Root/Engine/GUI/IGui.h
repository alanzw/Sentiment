#ifndef SENTIMENT_IGUI_H
#define SENTIMENT_IGUI_H

#include <memory>
#include <string>

class IGui
{
public:
    class IWindow
    {
    public:
        struct INIT_DESC
        {
            std::string windowName;
            int height;
            int width;
        };

    public:
        virtual bool isOpen() = 0;

        virtual void close() = 0;

        virtual void execute() = 0;

        virtual void* get_handle() = 0;
    };


public:
    virtual std::shared_ptr<IWindow> CreateWindow(IWindow::INIT_DESC& desc) = 0;
};

typedef void (*GUI_CONSTRUCTOR)(std::shared_ptr<IGui> & guiObj);

#endif
