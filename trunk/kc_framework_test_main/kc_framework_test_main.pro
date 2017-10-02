QT -= gui core

INCLUDEPATH += ../include

CONFIG += c++11

DESTDIR += ../bin
TARGET = kc_framework_test_main

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS +=

win32 {
    INCLUDEPATH += D:/library/boost_1_64_qt/include/
    LIBS += -L"D:/library/boost_1_64_qt/lib/"
    LIBS += -lboost_system-mgw53-mt-1_64 -lboost_thread-mgw53-mt-1_64 -lboost_filesystem-mgw53-mt-1_64
    SOURCES +=
    DEFINES += WIN32
}
unix {
    SOURCES +=
}
