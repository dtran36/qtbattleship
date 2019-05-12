#-------------------------------------------------
#
# Project created by QtCreator 2019-04-24T18:56:33
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtbattleship
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        gamescreen.cpp \
        main.cpp \
        mainmenu.cpp \
        mainwindow.cpp \
        matrix.cpp \
        npc.cpp \
        setupscreen.cpp \
        ship.cpp

HEADERS += \
        gamescreen.h \
        mainmenu.h \
        mainwindow.h \
        matrix.h \
        npc.h \
        setupscreen.h \
        ship.h

FORMS += \
        gamescreen.ui \
        mainmenu.ui \
        mainwindow.ui \
        setupscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qtbattleship_resources.qrc
