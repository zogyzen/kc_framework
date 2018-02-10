#-------------------------------------------------
#
# Project created by QtCreator 2017-09-26T21:37:54
#
#-------------------------------------------------

QT       -= core gui

INCLUDEPATH += ../include
LIBS += -static-libstdc++

DESTDIR += ../bin
TARGET = kc_framework_test_module
TEMPLATE = lib

DEFINES += KC_DB_LINK_SRV_MAIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activator.cpp \
    test_mod.cpp

HEADERS += \
    activator.h \
    stdn.h \
    test_mod.h

win32 {
    INCLUDEPATH += D:\mycode\library\windows\boost-1_66-qt\include
    LIBS += -LD:\mycode\library\windows\boost-1_66-qt\lib720
    LIBS += D:\mycode\library\windows\boost-1_66-qt\lib720\libboost_system-mgw72-mt-x32-1_66.a
    LIBS += D:\mycode\library\windows\boost-1_66-qt\lib720\libboost_filesystem-mgw72-mt-x32-1_66.a
    LIBS += D:\mycode\library\windows\boost-1_66-qt\lib720\libboost_thread-mgw72-mt-x32-1_66.a
}
unix {
    target.path = /usr/lib
    INSTALLS += target
}
