#pragma once


namespace MixCore {

    enum class EventType {
        None,
        WindowClosed,
        WindowResized,
        MouseButtonPressed,
    };

    struct EventContext {
        int width = 0, height = 0;
        int mouseX = 0, mouseY = 0;
    };

    struct Event {
        EventType type = EventType::None;
        EventContext context;

        Event(const EventType type, const int width, const int height, const int mouseX, const int mouseY) {
            this->type = type;
            this->context.width = width;
            this->context.height = height;
            this->context.mouseX = mouseX;
            this->context.mouseY = mouseY;
        }
        Event(const EventType type, const EventContext context) {
            this->type = type;
            this->context = context;
        }

        explicit Event(const EventType type) {
            this->type = type;
        }

    };
}



