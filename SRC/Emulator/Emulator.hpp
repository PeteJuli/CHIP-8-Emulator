#pragma once

#include "../SDL/Window/Window.hpp"

class Emulator
{
  public:
    void mainLoop();

  private:
    SDL::Window window;
};
