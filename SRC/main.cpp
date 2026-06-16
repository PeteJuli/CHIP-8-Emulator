#include "SDL/Context/Context.hpp"
#include "Emulator/Emulator.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
   try 
    {
        //Create SDL Context first cause of RAII
        SDL::Context sdlContext;

        //Start Emulator
        Emulator emulator;
        emulator.mainLoop();
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Exception -> " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
