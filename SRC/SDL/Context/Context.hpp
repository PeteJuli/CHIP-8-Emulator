#pragma once

#include <SDL3/SDL.h>

namespace SDL
{
    class Context
    {
        bool created{false};

      public:
        Context();
        ~Context();
    };
} // namespace SDL
