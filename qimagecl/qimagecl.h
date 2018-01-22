#ifndef QIMAGECL_H
#define QIMAGECL_H

#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX) || defined(__linux__)
#include "OpenCL/cl.hpp"
#endif

#include <iostream>
#include <string>

#include <QCoreApplication>
#include <QResource>
#include <QFile>
#include <QString>
#include <QImage>
#include <QColor>
#include <QException>
#include <QDebug>
#include <QTextCodec>

class QImageCL : public QObject
{
    Q_OBJECT
public:
    QImageCL();

    //! maps the first colors in the map onto the second
    //! e.g. QPair<> = ( RGB(255,0,0), RGB(127,127,127) )
    //! will map red to medium gray
    bool mapColors(const QImage &imgIn,
                   QImage &imgOut,
                   QList<QPair<QColor, QColor>> colorMap = QList<QPair<QColor, QColor>>());

protected:
    bool readKernelSource(const QString &path, std::string &kernelSource);
    cl::Platform default_platform;
    std::vector<cl::Device> all_devices;
    cl::Device default_device;
    cl::Context context;
    cl::Program program;
    cl::CommandQueue queue;

    cl::Kernel kernel_map_color;
};

#endif // QIMAGECL_H
