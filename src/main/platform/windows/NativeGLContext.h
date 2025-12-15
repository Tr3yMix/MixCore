#pragma once

#include <windows.h>

namespace win {
    class NativeGLContext {

    public:

        NativeGLContext();
        ~NativeGLContext();

        bool initialize(HDC hdc);
        void makeCurrent() const;
        void cleanup();
        void swapBuffers() const;

    private:

        HDC m_hdc{};
        HGLRC m_hglrc{};

    };
}


