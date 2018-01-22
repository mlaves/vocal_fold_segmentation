#include "qimagecl.h"

QImageCL::QImageCL()
{
    // init resource file containig OpenCL kernels
    Q_INIT_RESOURCE(cl_files);

    // get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0)
    {
        qDebug() << "No platforms found. Check OpenCL installation!\n";
        // todo: throw exception
    }

    default_platform = all_platforms[0];
    std::cout << "Using platform: "
              << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n" ;

    // get default device of the default platform
    default_platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &all_devices);
    if (all_devices.size() == 0)
    {
        qDebug() << "No devices found. Check OpenCL installation!\n";
        // todo: throw exception
    }
    default_device = all_devices[0];
    std::cout << "Using device: "
              << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
    std::cout << "Compute units: "
              << default_device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";

    // create context
    context= cl::Context({ default_device });

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code;
    if (!readKernelSource(":/qimage_map_color.cl", kernel_code))
    {
        qDebug() << "Could not load :/qimage_map_color.cl";
    }
    sources.push_back({ kernel_code.c_str(),
                        kernel_code.length() });

    program = cl::Program(context, sources);

    try
    {
        program.build({ default_device });
    }
    catch (const cl::Error &e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Error building: "
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device);
    }

    // create queue to which we will push commands for the device.
    queue = cl::CommandQueue(context, default_device);
    // end of preparation ######################################################

    // create the kernel
    kernel_map_color = cl::Kernel(program, "qimage_map_color");
}

bool QImageCL::readKernelSource(const QString &path, std::string &kernelSource)
{
    // Read the OpenCL kernel code
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Could not read kernel source "
                  << path.toStdString() << std::endl;
        return false;
    }

    kernelSource = file.readAll().toStdString();

    return true;
}

bool QImageCL::mapColors(const QImage &imgIn, QImage &imgOut,
                         QList<QPair<QColor, QColor>> colorMap)
{
    // check if imgIn and imgOut are the same size, not null etc
    if (imgIn.isNull() || imgOut.isNull())
        return false;

    // create memory on device
    cl::Image2D buffer_A(
                this->context,
                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, // copies directly to device mem
                cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8),
                imgIn.width(),
                imgIn.height(),
                0, // row pitch (gets calculated)
                (void*)imgIn.constBits()
                );

    cl::Image2D buffer_B(
                this->context,
                CL_MEM_WRITE_ONLY, // creates empty write image on device
                cl::ImageFormat(CL_RGBA, CL_UNSIGNED_INT8),
                imgIn.width(),
                imgIn.height(),
                0,   // row pitch (gets calculated)
                NULL // download pixel data from device later
                );

    // set arguments
    this->kernel_map_color.setArg(0, buffer_A);
    this->kernel_map_color.setArg(1, buffer_B);
    // third, fourth: color map

    // enqueue kernel
    this->queue.enqueueNDRangeKernel(this->kernel_map_color,
                                     cl::NullRange,
                                     cl::NDRange(imgIn.width(),
                                                 imgIn.height()), // global size
                                     cl::NullRange);  // local size

    // wait for queue to finish
    this->queue.finish();

    // read back result B from the device
    // (I really do not like how the C++ wrapper does this)
    cl::size_t<3> origin;
    cl::size_t<3> size;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;
    size[0] = imgIn.width();
    size[1] = imgIn.height();
    size[2] = 1;

    // CL_TRUE means wait for the entire image to be copied before continuing
    this->queue.enqueueReadImage(buffer_B, CL_TRUE,
                                 origin, size, 0, 0, imgOut.bits());

    return true;
}
