#pragma once

#include "lve_game_object.hpp"
#include "lve_window.hpp"

namespace lve {

class KeyboardMovementController {
    public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_E;
        int moveDown = GLFW_KEY_Q;
        int turn_on_camera = GLFW_KEY_C;
        int turn_off_camera = GLFW_KEY_B;
        int minimize_window = GLFW_KEY_V;
    };

    void moveInPlaneXZ(LveWindow& lveWindow, float dt, LveGameObject& gameObject);

    KeyMappings keys{};
    float moveSpeed{3.f};
    float lookSpeed{1.5f};
    bool cursor_enabled = false;
};

} // lve