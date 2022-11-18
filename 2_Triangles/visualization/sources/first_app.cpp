#include "first_app.hpp"
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANTS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>
#include <array>

namespace lve {

constexpr double accurasy = 0.0001;
constexpr glm::vec3 lightDirection = {1.0, -3.0, -1.0};

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

KeyMappings keys{};
VkExtent2D extent{};
TransformComponent transform{};
float moveSpeed{0.05f};
float lookSpeed{0.01f};
bool cursor_enabled = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float yaw = transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 UpDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (key == keys.moveForward) moveDir += forwardDir;
    if (key == keys.moveBackward) moveDir -= forwardDir;
    if (key == keys.moveRight) moveDir += rightDir;
    if (key == keys.moveLeft) moveDir -= rightDir;
    if (key == keys.moveUp) moveDir += UpDir;
    if (key == keys.moveDown) moveDir -= UpDir;
    if (key == keys.turn_on_camera) {
        cursor_enabled = 1;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (key == keys.turn_off_camera) {
        cursor_enabled = 0;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (key == keys.minimize_window) glfwIconifyWindow(window);

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        transform.translation += moveSpeed * glm::normalize(moveDir);
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (cursor_enabled) {
        double xcenter = extent.width / 2, ycenter = extent.height / 2;
        glfwSetCursorPos(window, xcenter, ycenter);

        glm::vec3 rotate{0.f};
        rotate.y -= -xpos + xcenter;
        rotate.x -= ypos - ycenter;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            transform.rotation += lookSpeed * glm::normalize(rotate);
        }
        
        // limits pitch values between about +/- 85idh degrees
        transform.rotation.x = glm::clamp(transform.rotation.x, -1.5f, 1.5f);
        transform.rotation.y = glm::mod(transform.rotation.y, glm::two_pi<float>());
    }
}

FirstApp::FirstApp(std::vector<Triangles::triangle_info_t> triangles) {
    loadGameObjects(triangles);
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
    LveCamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = LveGameObject::createGameObject();

    while (!lveWindow.shouldClose()) {
        glfwPollEvents();

        extent = lveWindow.getExtent();
        glfwSetCursorPosCallback(lveWindow.getGLFWwindow(), cursor_position_callback);
        glfwSetKeyCallback(lveWindow.getGLFWwindow(), key_callback);
        viewerObject.transform = transform;

        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = lveRenderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

        if (auto commandBuffer = lveRenderer.beginFrame()) {
            lveRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }
    }

    vkDeviceWaitIdle(lveDevice.device());
}

std::unique_ptr<LveModel> createTriangleModel(LveDevice& device, std::vector<Triangles::triangle_info_t>& triangles) {
    std::vector<LveModel::Vertex> vertices{};
    for (auto triag : triangles) {
        glm::vec3 firstPt  = {triag.triangle.trianglePt.first.coord.x, triag.triangle.trianglePt.first.coord.y, triag.triangle.trianglePt.first.coord.z};
        glm::vec3 secondPt = {triag.triangle.trianglePt.second.coord.x, triag.triangle.trianglePt.second.coord.y, triag.triangle.trianglePt.second.coord.z};
        glm::vec3 thirdPt  = {triag.triangle.trianglePt.third.coord.x, triag.triangle.trianglePt.third.coord.y, triag.triangle.trianglePt.third.coord.z};

        glm::vec3 color{};
        (triag.is_intersect == true) ? color = {1.f, 0.f, 0.f} : color = {0.f, 0.f, 1.f};

        glm::vec3 normal = glm::normalize(glm::cross((secondPt - firstPt), (thirdPt - firstPt)));

        if (glm::dot(normal, lightDirection) < 0) {
            normal = -normal;
        }

        //glm::vec3 side1 = firstPt - glm::vec3(0.004f, 0.004f, 0.004f);
        //glm::vec3 side2 = secondPt - glm::vec3(0.004f, 0.004f, 0.004f);
        //glm::vec3 side3 = thirdPt - glm::vec3(0.004f, 0.004f, 0.004f);


        vertices.push_back({firstPt, color, normal});
        vertices.push_back({secondPt, color, normal});
        vertices.push_back({thirdPt, color, normal});
    }

    return std::make_unique<LveModel>(device, vertices);
}

void FirstApp::loadGameObjects(std::vector<Triangles::triangle_info_t> triangles) {
    std::shared_ptr<LveModel> LveModel = createTriangleModel(lveDevice, triangles);
    auto tr = LveGameObject::createGameObject();
    tr.model = LveModel;
    tr.transform.translation = {0, 0, 0};
    tr.transform.scale = {1, 1, 1};
    gameObjects.push_back(std::move(tr));
}
} // lve