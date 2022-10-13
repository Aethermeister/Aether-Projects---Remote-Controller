TEMPLATE = app

QT -= gui
QT += core network websockets

CONFIG += c++17 console
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
        DESTDIR = ../Build/Debug/RemoteControllerComputerServer
}

CONFIG(release, debug|release) {
        DESTDIR = ../Build/Release/RemoteControllerComputerServer
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commandprocessor.cpp \
    logger.cpp \
    main.cpp \
    serverrunner.cpp

HEADERS += \
    commandprocessor.h \
    logger.h \
    serverrunner.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = rccs
