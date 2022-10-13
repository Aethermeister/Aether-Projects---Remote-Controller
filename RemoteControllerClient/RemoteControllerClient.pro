QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addnewconnectiondialog.cpp \
    connectionlistwidget.cpp \
    connectionlistwidgetbutton.cpp \
    filelistwidget.cpp \
    listwidgetitembutton.cpp \
    main.cpp \
    mainwindow.cpp \
    remotefunctionbasewidget.cpp \
    remotefunctionslistwidget.cpp \
    systemfunctionslistwidget.cpp

HEADERS += \
    addnewconnectiondialog.h \
    connectionlistwidget.h \
    connectionlistwidgetbutton.h \
    filelistwidget.h \
    listwidgetitembutton.h \
    mainwindow.h \
    remotefunctionbasewidget.h \
    remotefunctionslistwidget.h \
    systemfunctionslistwidget.h

FORMS += \
    addnewconnectiondialog.ui \
    connectionlistwidget.ui \
    connectionlistwidgetbutton.ui \
    filelistwidget.ui \
    mainwindow.ui \
    remotefunctionslistwidget.ui \
    systemfunctionslistwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Resources.qrc
