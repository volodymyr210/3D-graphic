#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Конструктор
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    // Отримання матриці виду
    glm::mat4 getViewMatrix();

    // Обробка вводу з клавіатури
    void processKeyboard(CameraMovement direction, float deltaTime);

    // Обробка вводу з миші
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Обробка колеса миші
    void processMouseScroll(float yoffset);

private:
    // Оновлення векторів напрямку камери
    void updateCameraVectors();
};

#endif
