QT += core gui

CONFIG += c++14

TARGET = qimagecl
CONFIG += lib_bundle staticlib # remove staticlib for creating shared object

TEMPLATE = lib
DEFINES += QIMAGECL_LIBRARY

SOURCES += \
    qimagecl.cpp
HEADERS += \
    qimagecl.h

RESOURCES += \
    cl_files.qrc

macx {
    LIBS += -framework OpenCL
    HEADERS += OpenCL/cl.hpp
}

unix:!macx {
    LIBS += -lOpenCL
}

DEFINES += QT_DEPRECATED_WARNINGS
