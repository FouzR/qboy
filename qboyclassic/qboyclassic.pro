#-------------------------------------------------
#
# Project created by QtCreator 2013-03-23T11:42:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qboyclassic
TEMPLATE = app


SOURCES += main.cpp\
        qboyclassic.cpp \
    qboythread.cpp

HEADERS  += qboyclassic.h \
    qboythread.h

FORMS    += qboyclassic.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libqboy/release/ -llibqboy
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libqboy/debug/ -llibqboy
else:unix: LIBS += -L$$OUT_PWD/../libqboy/ -llibqboy

INCLUDEPATH += $$PWD/../libqboy
DEPENDPATH += $$PWD/../libqboy
