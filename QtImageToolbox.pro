#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T14:05:59
#
#-------------------------------------------------

QT       += core gui help charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtImageToolbox
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES  += src/Main.cpp \
            src/MainWindow.cpp \
            src/DialogAdjust.cpp \
            src/DialogBinarize.cpp \
            src/DialogBlend.cpp \
            src/DialogConvolve.cpp \
            src/DialogGrayscale.cpp \
            src/DialogPixelate.cpp \
            src/DialogQuality.cpp \
            src/DialogRandom.cpp \
            src/DialogResize.cpp \
            src/DialogRotate.cpp \
            src/DialogSingleColor.cpp \
            src/FormHistogram.cpp \
            src/Stats.cpp \
            src/Tools.cpp

HEADERS  += hdr/MainWindow.h \
            hdr/DialogAdjust.h \
            hdr/DialogBinarize.h \
            hdr/DialogBlend.h \
            hdr/DialogConvolve.h \
            hdr/DialogGrayscale.h \
            hdr/DialogPixelate.h \
            hdr/DialogQuality.h \
            hdr/DialogRandom.h \
            hdr/DialogResize.h \
            hdr/DialogRotate.h \
            hdr/DialogSingleColor.h \
            hdr/FormHistogram.h \
            hdr/Stats.h \
            hdr/Tools.h

FORMS    += ui/MainWindow.ui \
            ui/DialogAdjust.ui \
            ui/DialogBinarize.ui \
            ui/DialogBlend.ui \
            ui/DialogConvolve.ui \
            ui/DialogGrayscale.ui \
            ui/DialogPixelate.ui \
            ui/DialogQuality.ui \
            ui/DialogRandom.ui \
            ui/DialogResize.ui \
            ui/DialogRotate.ui \
            ui/DialogSingleColor.ui \
            ui/FormHistogram.ui

CONFIG += c++14

RC_FILE = toolbox.rc

RESOURCES += \
    resoucres.qrc
