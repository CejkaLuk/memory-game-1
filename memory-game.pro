#-------------------------------------------------
#
# Project created by QtCreator 2011-05-08T23:44:50
#
#-------------------------------------------------

QT += core gui opengl widgets

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
    INSTALLS += iconfile desktopfile

    target.path = /usr/bin
    iconfile.path = /usr/share/pixmaps
    iconfile.files = installables/memory-game.png
    desktopfile.path = /usr/share/applications
    desktopfile.files = installables/memory-game.desktop
}
maemo5 {
    QT += maemo5 opengl
    DEFINES += MOBILE

    target.path = /opt/memory-game
    desktopfile.path = /usr/share/applications/hildon
}
win32 {
    DEFINES -= HAVE_OPENGL
    QT -= opengl
    RC_FILE = memory-game.rc
}
wince {
    DEFINES -= HAVE_OPENGL
    QT -= opengl
    DEFINES += MOBILE
    RC_FILE = memory-game.rc
}
