#include "engine.h"
#include "../external/glad.h"
#include "../rendering/font.h"
#include <iostream>

std::string demoText1 =
R"(Font rendering is such a pain in the ass.
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
        fontRenderer.initialize();
        font1 = Font("../data/BarlowCondensed-Regular.ttf", 64);
        font2 = Font("../data/sui.ttf", 64);
        font3 = Font("../data/Handjet-Regular.ttf", 64);
    };

    application.close += [this] () {
        fontRenderer.deinitialize();
        font1.destroy();
        font2.destroy();
        font3.destroy();
    };

    application.newFrame += [this] () {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        fontRenderer.addText(&font1, demoText1, glm::vec2(5.0f, 5.0f), glm::vec4(0.90f, 0.58f, 0.09f, 1.0f), 18.0f);
        fontRenderer.addText(&font2, "This uses a different font and color", glm::vec2(5.0f, 35.0f), glm::vec4(0.78, 0.08f, 0.90f, 1.0f), 22.0f);
        fontRenderer.addText(&font3, "Noice", glm::vec2(5.0f, 55.0f), glm::vec4(0.21f, 0.94f, 0.21f, 1.0f), 64.0f);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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