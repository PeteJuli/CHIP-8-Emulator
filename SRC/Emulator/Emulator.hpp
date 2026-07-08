#pragma once

#include "../SDL/Context/Context.hpp"
#include "../SDL/Window/Window.hpp"

class Emulator
{
  public:
    Emulator();
    void mainLoop();

  private:
    //Create SDL Context first cause of deletion order for RAII
    SDL::Context m_context;
    SDL::Window m_window;
    bool m_created{false};
};
