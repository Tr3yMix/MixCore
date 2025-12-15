
#include "NativeEventHandler.h"

#include <GL/gl.h>

#include "core/EventDispatcher.h"

extern EventDispatcher g_eventDispatcher;

namespace win {
    LRESULT NativeEventHandler::handleMessage(const WMC& ctx) {

        switch (ctx.msg) {
            case WM_PAINT: {
                PAINTSTRUCT ps;
                BeginPaint(ctx.hwnd, &ps);
                EndPaint(ctx.hwnd, &ps);
                break;
            }
            case WM_SIZE: {
                //handleResize(ctx);
                EventContext eCtx;
                eCtx.width = LOWORD(ctx.lParam);
                eCtx.height = HIWORD(ctx.lParam);
                const Event event(EventType::WindowResized, eCtx);

                g_eventDispatcher.pushEvent(event);
                break;
            }
            case WM_LBUTTONDOWN: {
                //handleLeftMouseDown(ctx);
                EventContext eCtx;
                eCtx.mouseX = LOWORD(ctx.lParam);
                eCtx.mouseY = HIWORD(ctx.lParam);
                const Event event(EventType::MouseButtonPressed, eCtx);

                g_eventDispatcher.pushEvent(event);
                break;
            }
            case WM_CLOSE: {
                //handleClose(ctx);
                const Event event(EventType::WindowClosed);

                g_eventDispatcher.pushEvent(event);
                break;
            }
            case WM_DESTROY: break;
            default: return DefWindowProc(ctx.hwnd, ctx.msg, ctx.wParam, ctx.lParam);
        }
        return 0;
    }

    void NativeEventHandler::handleResize(const WMC& ctx){
        ctx.window->m_width = LOWORD(ctx.lParam);
        ctx.window->m_height = HIWORD(ctx.lParam);
        glViewport(0, 0, ctx.window->m_width.signedInt(), ctx.window->m_height.signedInt());
    }

    void NativeEventHandler::handleLeftMouseDown(const WMC& ctx){
        if (const int yPos = HIWORD(ctx.lParam); yPos <= 40) {
            ReleaseCapture();
            SendMessage(ctx.hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        }
    }

    void NativeEventHandler::handleClose(const WMC& ctx) {
        ctx.window->m_isRunning = false;
        PostQuitMessage(0);
    }
}


