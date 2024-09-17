#include "engine.h"
#include "../external/glad.h"
#include "../rendering/font.h"
#include <iostream>

std::string demoText =
R"(Font rendering is such a pain in the ass -_-
That said, big thanks to https://github.com/shreyaspranav/stb-truetype-example)";

Engine::Engine() {
    size_t width = 512;
    size_t height = 512;
    application = Application(width, height, "OpenGL Font Rendering");
    screen.setWindowSize(width, height);
}

void Engine::run() {
    if(application.getWindow()) {
        std::cout << "Window is already created!\n";
        return;
    }
    
    application.initialize += [this] () {
        glEnable(GL_DEPTH_TEST);
        fontRenderer.initialize();
        font = Font("../data/BarlowCondensed-Regular.ttf", 32);
    };

    application.close += [this] () {
        fontRenderer.deinitialize();
        font.destroy();
    };

    application.newFrame += [this] () {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fontRenderer.addText(&font, demoText, glm::vec2(10.0f, 20.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), 22.0f);

        fontRenderer.addText(&font, "This works", glm::vec2(10.0f, 60.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 22.0f);
        fontRenderer.addText(&font, "Noice", glm::vec2(10.0f, 90.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 22.0f);
        fontRenderer.newFrame();
    };

    application.resize += [this] (int32_t width, int32_t height) {
        glViewport(0, 0, width, height);
        screen.setWindowSize(width, height);
    };

    application.keypress += [this] (int32_t key, int32_t scancode, int32_t action, int32_t mods) {

    };

    application.buttonPress += [this] (int32_t button, int32_t action, int32_t mods) {

    };

    application.mouseMove += [this] (double x, double y) {

    };

    application.mouseScroll += [this] (double x, double y) {

    };
    
    application.run();
}