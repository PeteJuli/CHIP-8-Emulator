#include "Emulator/Emulator.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    //Start Emulator
    Emulator emulator;

    if (!emulator.loadGame("Pong.ch8"))
    {
        std::cerr << "Can not load Rom!" << std::endl;
        return -1;
    }

    emulator.mainLoop();

    return 0;
}
