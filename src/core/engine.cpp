#include "engine.h"
#include "../external/glad.h"
#include "../rendering/font.h"
#include <iostream>

std::string demoText1 =
R"(Font rendering is such a pain in the ass.
That said, big thanks to https://github.com/shreyaspranav/stb-truetype-example)";

std::string demoText2 =
R"(Font Rendering with the FontRenderer Class

The FontRenderer class is a versatile component designed to facilitate text rendering in OpenGL applications. It abstracts the complexities of font rendering by managing font data, shaders, and rendering buffers. Here’s an overview of how it works:

1. Class Structure and Initialization

The FontRenderer class is responsible for setting up and managing the rendering of text. Upon initialization, it prepares the necessary OpenGL resources and shaders. This setup involves creating vertex buffers and configuring the shader for rendering text.

2. Adding Text

To render text, the FontRenderer class provides a method to add text to the rendering queue. This method takes a font object, the text to render, its position on the screen, color, and size. The class handles the conversion of text into vertex data that OpenGL can use to render the text as textured quads.

3. Buffers and Shaders

The FontRenderer class utilizes a buffer system to manage vertex data for different fonts. Each font has its own buffer, which includes a Vertex Array Object (VAO) and Vertex Buffer Object (VBO). This setup ensures that the vertex data for each font is efficiently managed and can be rendered quickly.

Shaders play a crucial role in the rendering process. The FontRenderer class uses a dedicated shader to handle text rendering, with uniform variables to control the projection matrix and texture properties.

4. Projection Matrix

The projection matrix is essential for mapping 2D text onto the screen correctly. The FontRenderer class calculates this matrix based on the screen dimensions, ensuring that text appears correctly sized and positioned regardless of screen resolution.

5. Frame Management

At the start of each frame, the FontRenderer class resets its state to prepare for rendering. This involves clearing previous frame data and updating the rendering buffers with new text if needed.

6. Cleanup

When the rendering is done or when the application shuts down, the FontRenderer class deinitializes its resources. This cleanup process involves releasing OpenGL buffers and shaders to prevent memory leaks and ensure efficient resource management.

Conclusion

The FontRenderer class provides a robust and flexible solution for rendering text in OpenGL applications. By managing font data, shaders, and buffers, it simplifies the process of displaying text, allowing developers to focus on other aspects of their applications. Whether for UI elements, in-game text, or debugging information, the FontRenderer class ensures high-quality text rendering with ease.)";

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

        fontRenderer.addText(&font1, demoText2, glm::vec2(5.0f, 5.0f), glm::vec4(0.90f, 0.58f, 0.09f, 1.0f), 18.0f);
        //fontRenderer.addText(&font2, "This uses a different font and color", glm::vec2(5.0f, 35.0f), glm::vec4(0.78, 0.08f, 0.90f, 1.0f), 22.0f);
        //fontRenderer.addText(&font3, "Noice", glm::vec2(5.0f, 55.0f), glm::vec4(0.21f, 0.94f, 0.21f, 1.0f), 64.0f);

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