#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_HPP_MINIMUM_OPENCL_VERSION 200
#endif

#define CL_HPP_ENABLE_EXCEPTIONS

#include <cmath>
#include <fstream>
#include <iostream>
#include <CL/cl2.hpp>

namespace OpenCL {

const std::string bitonic_path     = "../bsort.cl";
const std::string bsort_init       = "bsort_init";
const std::string bsort_stage_0    = "bsort_stage_0";
const std::string bsort_stage_n    = "bsort_stage_n";
const std::string bsort_merge      = "bsort_merge";
const std::string bsort_merge_last = "bsort_merge_last";

std::string read_file(const std::string &path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::string code(std::istreambuf_iterator<char>(file),
                    (std::istreambuf_iterator<char>()));
    return code;
}

class OpenCL_app final {
    cl::Platform platform;
    cl::Device device;
    cl::Context context;

    static cl::Platform choose_platform() {
        cl::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        for (auto platform : platforms) {
            cl::vector<cl::Device> devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
            for (auto device : devices) 
                if (device.getInfo<CL_DEVICE_TYPE>() ==  CL_DEVICE_TYPE_GPU)
                    return platform;
        }
        throw std::runtime_error("Couldn't find platform");
    }

    static cl::Device choose_device(const cl::Platform &platform) {
        cl::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        return devices.front();
    }

    cl::Program build_program(const std::string &path) {
        std::string code = read_file(path);
        cl::Program::Sources source{code};
        cl::Program program(context, source);
        cl::vector<cl::Device> devices{device};
        program.build(devices);
        return program;
    }

    public:

    OpenCL_app() : platform{choose_platform()}, device{choose_device(platform)}, context{device} {}
    OpenCL_app(const OpenCL_app&) = delete;
    OpenCL_app &operator=(const OpenCL_app&) = delete;
    OpenCL_app(OpenCL_app&&) = delete;
    OpenCL_app &operator=(OpenCL_app&&) = delete;

    void bitonic_sort(const std::vector<float> &sequence) {
        cl::Program program = build_program(bitonic_path);
        cl::vector<cl::Kernel> kernels;
        program.createKernels(&kernels);
        //for (auto kernel : kernels)
        //    std::cout << kernel.getInfo<CL_KERNEL_FUNCTION_NAME>() << std::endl;
        unsigned local_size = kernels.front().getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
        local_size = (int)std::pow(2, std::trunc(std::log2(local_size)));

        cl::Buffer data(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
                sequence.size() * sizeof(float), const_cast<float*>(sequence.data()));
    }

    void dump(std::ostream &os) const {
        os << platform.getInfo<CL_PLATFORM_NAME>() << " " << device.getInfo<CL_DEVICE_NAME>() << " " << context.getInfo<CL_CONTEXT_NUM_DEVICES>();
    }
};

std::ostream &operator<<(std::ostream &os, const OpenCL_app &app) {
    app.dump(os);
    return os;
}

} // OpenCL


int main() {
    try {
        OpenCL::OpenCL_app app{};
        std::cout << app << std::endl;
        std::vector<float> sequence{1.f, 2.f, 3.f};
        app.bitonic_sort(sequence);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
