#include "Emulator.hpp"

void Emulator::mainLoop()
{
    bool isRunning = true;

    while (isRunning)
    {
        //Check if window got closed
        isRunning = window.eventHandling();

        //CHIP-8
        

        //Draw Present
        window.render();
    }
}