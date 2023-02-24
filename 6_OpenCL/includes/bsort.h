#pragma once

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_HPP_MINIMUM_OPENCL_VERSION 200
#endif

#define CL_HPP_ENABLE_EXCEPTIONS

#include <utility>
#include <CL/cl2.hpp>

namespace OpenCL {

const std::string bitonic_path     = "../bsort.cl";
const std::string bsort_init       = "bsort_init";
const std::string bsort_stage_0    = "bsort_stage_0";
const std::string bsort_stage_n    = "bsort_stage_n";
const std::string bsort_merge      = "bsort_merge";
const std::string bsort_merge_last = "bsort_merge_last";
constexpr unsigned wi_size = 8;
constexpr int direction = 0; /* Ascending: 0, Descending: -1 */

class OpenCL_app final {
    cl::Platform platform;
    cl::Device device;
    cl::Context context;

    static cl::Platform choose_platform();
    static cl::Device choose_device(const cl::Platform &platform);
    cl::Program build_program(const std::string &path) const;
    unsigned long command_duration(const cl::Event &evt) const {
        unsigned long start = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
        unsigned long end = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
        return (end - start);
    }

    public:

    OpenCL_app() : platform{choose_platform()}, device{choose_device(platform)}, context{device} {}
    OpenCL_app(const OpenCL_app&) = delete;
    OpenCL_app &operator=(const OpenCL_app&) = delete;
    OpenCL_app(OpenCL_app&&) = delete;
    OpenCL_app &operator=(OpenCL_app&&) = delete;

    std::pair<std::vector<float>, unsigned long> bitonic_sort(const std::vector<float> &sequence) const;
    void dump(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const OpenCL_app &app);

} // OpenCL
