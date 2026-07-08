#pragma once

namespace SDL
{
    class Context
    {
      public:
        Context();
        ~Context();
        [[nodiscard]] bool isCreated() const;

      private:
        bool m_created{false};
    };
} //namespace SDL
