// Programmer (s): Rook
// Date: 5/13/2013
// File: main.cpp

#include "Root\Engine\Engine.h"
#include <stdexcept>
#include <iostream>

int main(int argc, char * argv[])
{

    try
    {
        Engine engine(argv[0]);

        engine.run();
    }
    catch (std::runtime_error& error)
    {
        std::cout << error.what();
    }

    return 0;
}

