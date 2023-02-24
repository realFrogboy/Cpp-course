#include <cmath>
#include <limits>
#include <fstream>
#include <iostream>

#include "bsort.h"

namespace {

std::string read_file(const std::string &path) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::string code(std::istreambuf_iterator<char>(file),
                    (std::istreambuf_iterator<char>()));
    return code;
}

}

namespace OpenCL {

cl::Platform OpenCL_app::choose_platform() {
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

cl::Device OpenCL_app::choose_device(const cl::Platform &platform) {
    cl::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    return devices.front();
}

cl::Program OpenCL_app::build_program(const std::string &path) const {
    std::string code = read_file(path);
    cl::Program::Sources source{code};
    cl::Program program(context, source);
    cl::vector<cl::Device> devices{device};
    program.build(devices);
    return program;
}

std::pair<std::vector<float>, unsigned long> OpenCL_app::bitonic_sort(const std::vector<float> &sequence) const {
    cl::Program program = build_program(bitonic_path);
    cl::vector<cl::Kernel> kernels;
    program.createKernels(&kernels);
    unsigned local_size = kernels.front().getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
    local_size = static_cast<unsigned>(std::pow(2, std::trunc(std::log2(local_size))));

    std::vector<float> aligned_seq = sequence;
    unsigned aligned_sz = static_cast<unsigned>(std::pow(2, std::ceil(std::log2(sequence.size()))));
    aligned_seq.resize(aligned_sz, CL_FLT_MAX);

    cl::Buffer data(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
            aligned_seq.size() * sizeof(float), aligned_seq.data());

    for (auto kernel : kernels) {
        kernel.setArg(0, data);
        kernel.setArg(1, cl::Local(wi_size * local_size * sizeof(float)));
    }

    cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE);
    cl::Event profileEvent;
    unsigned long total_time = 0;

    unsigned global_size = aligned_seq.size() / wi_size;
    if(global_size < local_size) 
        local_size = global_size;

    cl::NDRange global(global_size);
    cl::NDRange local(local_size);
    queue.enqueueNDRangeKernel(kernels[0] /*bsort_init*/, 0, global, local, nullptr, &profileEvent);
    queue.finish();
    total_time += command_duration(profileEvent);

    unsigned num_stages = global_size / local_size;
    for(unsigned high_stage = 2; high_stage < num_stages; high_stage <<= 1) {
        kernels[1].setArg(2, high_stage); // bsort_stage_0
        kernels[2].setArg(3, high_stage); // bsort_stage_n

        for (unsigned stage = high_stage; stage > 1; stage >>= 1) {
            kernels[2].setArg(2, stage); // bsort_stage_n
            queue.enqueueNDRangeKernel(kernels[2] /*bsort_stage_n*/, 0, global, local, nullptr, &profileEvent);
            queue.finish();
            total_time += command_duration(profileEvent);
        }

        queue.enqueueNDRangeKernel(kernels[1] /*bsort_stage_0*/, 0, global, local, nullptr, &profileEvent);
        queue.finish();
        total_time += command_duration(profileEvent);
    }

    kernels[3].setArg(3, direction); // bsort_merge
    kernels[4].setArg(2, direction); // bsort_merge_last

    for (unsigned stage = num_stages; stage > 1; stage >>= 1) {
        kernels[3].setArg(2, stage); // bsort_merge
        queue.enqueueNDRangeKernel(kernels[3] /*bsort_merge*/, 0, global, local, nullptr, &profileEvent);
        queue.finish();
        total_time += command_duration(profileEvent);
    }

    queue.enqueueNDRangeKernel(kernels[4] /*bsort_merge_last*/, 0, global, local, nullptr, &profileEvent);
    queue.finish();
    total_time += command_duration(profileEvent);

    cl::copy(queue, data, aligned_seq.begin(), aligned_seq.end());
    aligned_seq.resize(sequence.size());
    return {aligned_seq, total_time};
}

void OpenCL_app::dump(std::ostream &os) const {
    os << platform.getInfo<CL_PLATFORM_NAME>() << " " << device.getInfo<CL_DEVICE_NAME>() << " " << context.getInfo<CL_CONTEXT_NUM_DEVICES>();
}

std::ostream &operator<<(std::ostream &os, const OpenCL_app &app) {
    app.dump(os);
    return os;
}

} // OpenCL
