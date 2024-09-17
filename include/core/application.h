#ifndef APPLICATION_H
#define APPLICATION_H

#include "eventhandler.h"
#include <cstdlib>
#include <string>

using InitializeEvent = std::function<void()>;
using CloseEvent = std::function<void()>;
using NewFrameEvent = std::function<void()>;
using ResizeEvent = std::function<void(int32_t width, int32_t height)>;
using KeypressEvent = std::function<void(int32_t key, int32_t scancode, int32_t action, int32_t mods)>;
using ButtonPressEvent = std::function<void(int32_t button, int32_t action, int32_t mods)>;
using MouseMoveEvent = std::function<void(double x, double y)>;
using MouseScrollEvent = std::function<void(double x, double y)>;

struct GLFWwindow;

class Application {
public:
    EventHandler<InitializeEvent> initialize;
    EventHandler<CloseEvent> close;
    EventHandler<NewFrameEvent> newFrame;
    EventHandler<ResizeEvent> resize;
    EventHandler<KeypressEvent> keypress;
    EventHandler<ButtonPressEvent> buttonPress;
    EventHandler<MouseMoveEvent> mouseMove;
    EventHandler<MouseScrollEvent> mouseScroll;
    Application();
    Application(uint32_t width, uint32_t height, const std::string &title);
    void run();
    void quit();
    inline GLFWwindow *getWindow() const { return window; }
private:
    GLFWwindow *window;
    uint32_t width;
    uint32_t height;
    std::string title;
    void dispose();
    static Application *getApplicationFromUserData(GLFWwindow *window);
    static void onWindowResizeCallback(GLFWwindow *window, int32_t width, int32_t height);
    static void onKeyPressCallback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    static void onMouseButtonPressCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
    static void onMouseMoveCallback(GLFWwindow *window, double x, double y);
    static void onMouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif