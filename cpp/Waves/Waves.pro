QT       += core gui

TARGET = waves
TEMPLATE = app

LIBS += sndfile

unix { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}


SOURCES += main.cpp\
        mainwindow.cpp \
    manager.cpp \
    painter.cpp \
    cff.cpp \
    history.cpp \
    cf.cpp \
    nodes.cpp \
    modifiers.cpp \
    genarray.cpp \
    grapher.cpp \
    writer.cpp \
    old.cpp \
    kiss_fft.cpp \
    grapherbase.cpp \
    contin.cpp \
    show.cpp

HEADERS  += mainwindow.h \
    manager.h \
    painter.h \
    cff.h \
    history.h \
    cf.h \
    nodes.h \
    modifiers.h \
    genarray.h \
    grapher.h \
    writer.h \
    old.h \
    kiss_fft_guts.h \
    kiss_fft.h \
    grapherbase.h \
    contin.h \
    show.h

FORMS    += mainwindow.ui
