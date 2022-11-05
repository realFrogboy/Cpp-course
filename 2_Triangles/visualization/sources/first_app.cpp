#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
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

FirstApp::FirstApp(std::vector<Triangles::triangle_info_t> triangles) {
    loadGameObjects(triangles);
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
    LveCamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = LveGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!lveWindow.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = glm::min(frameTime, MAX_FRAME_TIME);

        cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
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

std::unique_ptr<LveModel> createTriangleModel(LveDevice& device, Triangles::triangle_info_t& triag) {
    glm::vec3 firstPt  = {triag.triangle.trianglePt.first.coord.x, triag.triangle.trianglePt.first.coord.y, triag.triangle.trianglePt.first.coord.z};
    glm::vec3 secondPt = {triag.triangle.trianglePt.second.coord.x, triag.triangle.trianglePt.second.coord.y, triag.triangle.trianglePt.second.coord.z};
    glm::vec3 thirdPt  = {triag.triangle.trianglePt.third.coord.x, triag.triangle.trianglePt.third.coord.y, triag.triangle.trianglePt.third.coord.z};

    glm::vec3 color{};
    (triag.is_intersect == true) ? color = {1.f, 0.f, 0.f} : color = {0.f, 0.f, 1.f};

    std::vector<LveModel::Vertex> vertices{
        {firstPt, color},
        {secondPt, color},
        {thirdPt, color},
    };

    return std::make_unique<LveModel>(device, vertices);
}

void FirstApp::loadGameObjects(std::vector<Triangles::triangle_info_t> triangles) {
    for (auto triag : triangles) {
        std::shared_ptr<LveModel> LveModel = createTriangleModel(lveDevice, triag);
        auto tr = LveGameObject::createGameObject();
        tr.model = LveModel;
        tr.transform.translation = {0, 0, 0};
        tr.transform.scale = {1, 1, 1};
        gameObjects.push_back(std::move(tr));
    }
}
} // lve