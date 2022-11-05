#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_renderer.hpp"
#include "lve_game_object.hpp"

#include "triangles.h"

#include <memory>
#include <vector>

namespace lve {
    class FirstApp {

        public:
        static constexpr unsigned WIDTH = 800;
        static constexpr unsigned HEIGHT = 600;
        static constexpr float MAX_FRAME_TIME = 10.f;

        FirstApp(std::vector<Triangles::triangle_info_t> triangles);
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;

        void run();

        private:
        void loadGameObjects(std::vector<Triangles::triangle_info_t> triangles);

        LveWindow lveWindow{WIDTH, HEIGHT, "Hello_Vulkan"};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        std::vector<LveGameObject> gameObjects;
    };
} // lve