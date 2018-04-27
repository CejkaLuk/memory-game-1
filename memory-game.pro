
QT += core gui opengl widgets
CONFIG += c++11

TARGET = memory-game
TEMPLATE = app
INSTALLS = target
DEFINES += HAVE_OPENGL

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

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    translations/memory-game_hu_HU.ts \
    translations/memory-game_he_IL.ts

OTHER_FILES += \
    readme.txt \
    installables/memory-game.png \
    installables/memory-game.ico \
    installables/memory-game.desktop

unix {
    QMAKE_CXXFLAGS += -Wall -Wextra
    INSTALLS += iconfile desktopfile

    target.path = /usr/bin
    iconfile.path = /usr/share/pixmaps
    iconfile.files = installables/memory-game.png
    desktopfile.path = /usr/share/applications
    desktopfile.files = installables/memory-game.desktop
}
win32 {
    DEFINES -= HAVE_OPENGL
    QT -= opengl
    RC_FILE = memory-game.rc
}
