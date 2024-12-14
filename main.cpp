#include "stb_image.h"
#include "shader.h"
#include "sphere.h"
#include "opengl_setup.h"
#include "texture.h"
#include "camera.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

struct Planet {
    Sphere sphere;
    Texture texture;
    float distanceFromSun;
    float size;
    float orbitSpeed;      
    float rotationSpeed;    
    float axisTilt;         
    std::vector<Planet> moons;
};

Camera camera(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = 400, lastY = 300; 
bool firstMouse = true;
float deltaTime = 0.0f; 
float lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

int main() {
    // Ініціалізація OpenGL
    if (!OpenGLSetup::initialize())
        return -1;

    GLFWwindow* window = OpenGLSetup::createWindow(800, 600, "Solar System Emulator");
    if (!window)
        return -1;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

    Texture sunTexture("../textures/sun.jpg");
    Texture mercuryTexture("../textures/mercury.jpg");
    Texture venusTexture("../textures/venus.jpg");
    Texture earthTexture("../textures/earth.jpg");
    Texture moonTexture("../textures/moon.jpg");
    Texture marsTexture("../textures/mars.jpg");
    Texture jupiterTexture("../textures/jupiter.jpg");
    Texture saturnTexture("../textures/saturn.jpg");
    Texture uranusTexture("../textures/uranus.jpg");
    Texture neptuneTexture("../textures/neptune.jpg");
    Texture starsTexture("../textures/stars.jpg");

    Sphere sun(3.0f, 36, 18);
    Sphere skybox(50.0f, 36, 18);

    std::vector<Planet> planets = {
        {Sphere(0.6f, 36, 18), mercuryTexture, 4.0f, 0.2f,  16.7f, 10.0f, 90.0f, {}},  
        {Sphere(1.0f, 36, 18), venusTexture,   5.0f, 0.2f,  14.5f, -9.0f, 50.4f, {}}, 
        {Sphere(1.2f, 36, 18), earthTexture,   6.5f, 0.3f,  12.0f, 8.0f,  70.0f, {{Sphere(5.4f, 36, 18), moonTexture, 2.6f, 0.05f, 6.0f, 1.0f, 0.0f, {}}}}, 
        {Sphere(0.8f, 36, 18), marsTexture,    8.0f, 0.2f,  8.4f, 7.0f,  80.0f, {}}, 
        {Sphere(1.7f, 36, 18), jupiterTexture, 11.0f, 0.7f,  4.3f, 6.0f,  -80.1f,  {}},  
        {Sphere(1.4f, 36, 18), saturnTexture, 15.0f, 0.6f,  3.9f, 5.0f,  26.7f, {}},  
        {Sphere(1.0f, 36, 18), uranusTexture, 18.0f, 0.5f,  3.2f, 4.0f,  97.8f,  {}}, 
        {Sphere(0.9f, 36, 18), neptuneTexture,20.0f, 0.4f,  3.0f, 3.0f,  28.3f,  {}}  
    };

    glEnable(GL_DEPTH_TEST);

    float time = 0.0f;
    float timeScale = 60.0f; 
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        time += deltaTime;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.use();

        // Матриця проекції
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Матриця виду
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Рендеринг зіркового фону
        starsTexture.bind(0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "planetTexture"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(skybox.getVAO());
        glDrawElements(GL_TRIANGLES, skybox.getVertexCount(), GL_UNSIGNED_INT, 0);

        // Рендеринг Сонця
        sunTexture.bind(0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "planetTexture"), 0);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(sun.getVAO());
        glDrawElements(GL_TRIANGLES, sun.getVertexCount(), GL_UNSIGNED_INT, 0);

        for (const auto& planet : planets) {
            planet.texture.bind(0);
            glUniform1i(glGetUniformLocation(shaderProgram.ID, "planetTexture"), 0);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, time * glm::radians(planet.orbitSpeed * 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(planet.distanceFromSun, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(planet.axisTilt), glm::vec3(5.0f, 1.0f, 1.0f)); 
            model = glm::rotate(model, time * glm::radians(planet.rotationSpeed * 8.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
            model = glm::scale(model, glm::vec3(planet.size));

            // Передача моделі в шейдер
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // Рендеринг планети
            glBindVertexArray(planet.sphere.getVAO());
            glDrawElements(GL_TRIANGLES, planet.sphere.getVertexCount(), GL_UNSIGNED_INT, 0);

            // Рендеринг супутників
            for (const auto& moon : planet.moons) {
                moon.texture.bind(0);
                glUniform1i(glGetUniformLocation(shaderProgram.ID, "planetTexture"), 0);

                glm::mat4 moonModel = model;

                moonModel = glm::rotate(moonModel, time * glm::radians(moon.orbitSpeed * 8.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                moonModel = glm::translate(moonModel, glm::vec3(moon.distanceFromSun, 0.0f, 0.0f));
                moonModel = glm::rotate(moonModel, time * glm::radians(moon.rotationSpeed * timeScale), glm::vec3(0.0f, 1.0f, 0.0f));
                moonModel = glm::scale(moonModel, glm::vec3(moon.size));

                glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
                glBindVertexArray(moon.sphere.getVAO());
                glDrawElements(GL_TRIANGLES, moon.sphere.getVertexCount(), GL_UNSIGNED_INT, 0);
            }
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    OpenGLSetup::cleanup();
    return 0;
}
