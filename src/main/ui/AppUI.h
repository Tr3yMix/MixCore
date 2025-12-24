#pragma once
#include "RectanglePrimitive.h"
#include "core/AppWindow.h"

namespace MixCore::ui {

    class AppUI {

    public:

        explicit AppUI(AppWindow* window);

        void draw() const;

        static void clear(util::Color color);

    private:
        AppWindow* m_window;

        std::unique_ptr<RectanglePrimitive> m_topBar;
        std::unique_ptr<RectanglePrimitive> m_exitButton;




    };

} // ui
