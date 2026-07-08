#include "Emulator.hpp"
#include <iostream>

Emulator::Emulator() : m_context(), m_window(m_context)
{
    //Check that everything is set up correctly
    if (!m_context.isCreated() || !m_window.isCreated())
    {
        std::cerr << "[Emulator] -> Critical Error SDL initialization failed!" << std::endl;
        return;
    }

    m_created = true;
}

void Emulator::mainLoop()
{
    if (!m_created)
    {
        return;
    }

    while (m_window.eventHandling())
    {
        //CHIP-8

        //Draw Present
        m_window.render();
    }
}