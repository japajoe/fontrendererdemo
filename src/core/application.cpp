#include "application.h"
#include "../external/glad.h"
#include "../external/glfw3.h"
#include <iostream>

Application::Application() {
    this->width = 512;
    this->height = 512;
    this->title = "OpenGL Font Rendering";
    window = nullptr;    
}

Application::Application(uint32_t width, uint32_t height, const std::string &title) {
    this->width = width;
    this->height = height;
    this->title = title;
    window = nullptr;
}

void Application::run() {
    if(window) {
        std::cerr << "Window is already created\n";
        return;
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if(!window) {
        dispose();
        std::cerr << "Failed to create window\n";
        return;
    }

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, onWindowResizeCallback);
    glfwSetKeyCallback(window, onKeyPressCallback);
    glfwSetMouseButtonCallback(window, onMouseButtonPressCallback);
    glfwSetCursorPosCallback(window, onMouseMoveCallback);
    glfwSetScrollCallback(window, onMouseScrollCallback); 

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        dispose();
        std::cerr << "Failed to initialize OpenGL\n";
        return;
    }

    const GLubyte* version = glGetString(GL_VERSION);

    if (version != nullptr) 
        std::cout << "OpenGL Version: " << version << '\n';

    glfwSwapInterval(1);

    initialize();

    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window)) {
        newFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    close();

    dispose();
}

void Application::quit() {
    if(window)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Application::dispose() {
    if(!window) {
        glfwTerminate();
    } else {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    window = nullptr;
}

Application *Application::getApplicationFromUserData(GLFWwindow *window) {
    void *userData = glfwGetWindowUserPointer(window);
    if(userData)
        return reinterpret_cast<Application*>(userData);
    return nullptr;
}

void Application::onWindowResizeCallback(GLFWwindow *window, int32_t width, int32_t height) {
    Application *application = getApplicationFromUserData(window);
    if(application) {
        application->resize(width, height);
    }
}

void Application::onKeyPressCallback(GLFWwindow *window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    Application *application = getApplicationFromUserData(window);
    if(application) {
        application->keypress(key, scancode, action, mods);
    }
}

void Application::onMouseButtonPressCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
    Application *application = getApplicationFromUserData(window);
    if(application) {
        application->buttonPress(button, action, mods);
    }
}

void Application::onMouseMoveCallback(GLFWwindow *window, double x, double y) {
    Application *application = getApplicationFromUserData(window);
    if(application) {
        application->mouseMove(x, y);
    }
}

void Application::onMouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    Application *application = getApplicationFromUserData(window);
    if(application) {
        application->mouseScroll(xoffset, yoffset);
    }
}