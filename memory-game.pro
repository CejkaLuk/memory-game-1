#-------------------------------------------------
#
# Project created by QtCreator 2011-05-08T23:44:50
#
#-------------------------------------------------

QT += core gui

TARGET = memory-game
TEMPLATE = app
INSTALLS = target

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    memorycard.cpp \
    memorygameboard.cpp

HEADERS += \
    mainwindow.h \
    memorycard.h \
    memorygameboard.h

FORMS += \
    mainwindow.ui

unix {
    target.path = /usr/bin
}
maemo5 {
    target.path = /opt/memory-game/bin
}
