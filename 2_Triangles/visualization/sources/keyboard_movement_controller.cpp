#include "keyboard_movement_controller.hpp"

#include <limits>

namespace lve {



void KeyboardMovementController::moveInPlaneXZ(LveWindow& lveWindow, float dt, LveGameObject& gameObject) {

    GLFWwindow *window = lveWindow.getGLFWwindow();
    auto extent = lveWindow.getExtent();

    double xpos, ypos, xcenter = extent.width / 2, ycenter = extent.height / 2;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, xcenter, ycenter);

    glm::vec3 rotate{0.f};
    rotate.y += -xpos + xcenter;
    rotate.x += ypos - ycenter;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
    }
    
    // limits pitch values between about +/- 85idh degrees
    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = gameObject.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 UpDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += UpDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= UpDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}

} // lve