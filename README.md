# OpenGL Font Rendering
A font rendering implementation based on https://github.com/shreyaspranav/stb-truetype-example

![](example.png)

# Differences with original repository:
- Handles new line characters
- No crash on unknown glyphs
- Renders multiple fonts at once
- Well defined abstraction, easy to integrate into existing projects

# Dependencies
Everything is included:
- GLFW
- Glad
- GLM
- stb_truetype