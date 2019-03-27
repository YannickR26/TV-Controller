#-------------------------------------------------
#
# Project created by QtCreator 2017-11-11T12:42:06
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TVController
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

win32: RC_ICONS = pixmap/tv-icon.ico

SOURCES += \
    main.cpp \
    tAddBackground.cpp \
    tAddEvent.cpp \
    tConfig.cpp \
    tFenPrincipale.cpp \
    tGesMedia.cpp \
    tGesScript.cpp \
    tLive.cpp \
    tPlanning.cpp \
    tPlanningArrierePlan.cpp \
    tPreferences.cpp \
    tShowColor.cpp \
    tTcpSocket.cpp

HEADERS += \
    tAddBackground.h \
    tAddEvent.h \
    tBackground.h \
    tConfig.h \
    tEvent.h \
    tFenPrincipale.h \
    tGesMedia.h \
    tGesScript.h \
    tLive.h \
    tMedia.h \
    tParametreGlobal.h \
    tPlanning.h \
    tPlanningArrierePlan.h \
    tPreferences.h \
    tShowColor.h \
    tTcpSocket.h \
    tTv.h \
    tScheduler.h

RESOURCES += \
    resource.qrc
