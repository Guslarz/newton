TEMPLATE = app
TARGET = gui

QT = core gui widgets

RC_ICONS += \
    logo1.ico

RC_FILE += \
    gui.rc

SOURCES += \
    datainput.cpp \
    intervalinput.cpp \
    intervaloutput.cpp \
    intervals.cpp \
    main.cpp \
    mainwindow.cpp \
    newton.cpp \
    newtonoutput.cpp

HEADERS += \
    datainput.h \
    intervalinput.h \
    intervaloutput.h \
    intervals.h \
    mainwindow.h \
    newton.h \
    newtonoutput.h
