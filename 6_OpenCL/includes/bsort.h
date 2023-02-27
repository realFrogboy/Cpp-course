#pragma once

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 210
#define CL_HPP_MINIMUM_OPENCL_VERSION 210
#endif

#define CL_HPP_ENABLE_EXCEPTIONS

#include <CL/opencl.hpp>

namespace OpenCL {

const std::string bitonic_path = "../bsort.cl";
constexpr unsigned wi_size = 8;

enum class Order {
    ASCENDING = 0, 
    DESCENDING = -1,
};

class OpenCL_app final {
    cl::Platform platform;
    cl::Device device;
    cl::Context context;
    cl::Program program;

    static cl::Platform choose_platform();
    static cl::Device choose_device(const cl::Platform &platform);
    unsigned long command_duration(const cl::Event &evt) const {
        unsigned long start = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
        unsigned long end = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
        return (end - start);
    }

    public:
    static constexpr int direction = static_cast<int>(Order::ASCENDING); /* Ascending: 0, Descending: -1 */

    OpenCL_app() : platform{choose_platform()}, device{choose_device(platform)}, context{device} {}
    OpenCL_app(const OpenCL_app&) = delete;
    OpenCL_app &operator=(const OpenCL_app&) = delete;
    OpenCL_app(OpenCL_app&&) = delete;
    OpenCL_app &operator=(OpenCL_app&&) = delete;

    void build_program(const std::string &path);

    std::pair<std::vector<float>, unsigned long> bitonic_sort(const std::vector<float> &sequence) const;
    void dump(std::ostream &os) const;
};

class IOpenCL_app final {
    OpenCL_app app;
    std::string path{};

    public:
    std::tuple<std::vector<float>, unsigned long, unsigned long> bitonic_sort(const std::vector<float> &sequence);
    int direction() const { return app.direction; }
    void dump(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const OpenCL_app &app);
std::ostream &operator<<(std::ostream &os, const IOpenCL_app &app);

} // OpenCL
