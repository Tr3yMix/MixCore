#pragma once
#include "RectanglePrimitive.h"
#include "Text.h"
#include "core/AppWindow.h"

namespace ui {

    class AppUI {

    public:

        explicit AppUI(AppWindow* window);
        ~AppUI();

        void draw() const;

        static void clear(util::Color color);

    private:
        AppWindow* m_window;

        std::unique_ptr<Text> m_text;
        std::unique_ptr<RectanglePrimitive> m_topBar;
        std::unique_ptr<RectanglePrimitive> m_exitButton;




    };

} // ui
