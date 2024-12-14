#ifndef OPENGL_SETUP_H
#define OPENGL_SETUP_H

#include <GLFW/glfw3.h>

class OpenGLSetup {
public:
    static bool initialize();
    static GLFWwindow* createWindow(int width, int height, const char* title);
    static void processInput(GLFWwindow* window);
    static void cleanup();

private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif 

