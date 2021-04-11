#-------------------------------------------------
#
# Project created by QtCreator 2020-08-23T11:15:24
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ePaper
TEMPLATE = app

CONFIG += c++11

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
    src/mainEPaper.cpp \
    src/ui/ToolAdd/tooladdcircle.cpp \
    src/ui/ToolAdd/tooladdline.cpp \
    src/ui/ToolAdd/tooladdpoint.cpp \
    src/ui/ToolAdd/tooladdrectangle.cpp \
    src/ui/ToolAdd/tooladdtext.cpp \
    src/ui/ToolAdd/tooladdtriangle.cpp \
    src/ui/ToolAdd/toolmoveelement.cpp \
    src/ui/colorwidget.cpp \
    src/ui/comboboxdelegate.cpp \
    src/ui/directionwidget.cpp \
    src/ui/elementsmodel.cpp \
    src/ui/elementswidget.cpp \
    src/ui/epaperwindow.cpp \
    src/ui/loggerconsole.cpp \
    src/ui/parameterdelegate.cpp \
    src/ui/parametereditor.cpp \
    src/ui/serialconfigwidget.cpp \
    src/ui/toolswidget.cpp \
    src/ui/widgetdisplay.cpp \
    src/core/drawable/drawablecircle.cpp \
    src/core/drawable/drawableelement.cpp \
    src/core/drawable/drawableimage.cpp \
    src/core/drawable/drawableline.cpp \
    src/core/drawable/drawablepoint.cpp \
    src/core/drawable/drawablerectangle.cpp \
    src/core/drawable/drawabletext.cpp \
    src/core/drawable/drawabletriangle.cpp \
    src/core/epapercontroller.cpp \
    src/core/properties.cpp \
    src/core/serialdisplay.cpp \
    src/core/serialdriver.cpp \
    src/core/session.cpp \
    src/core/tools.cpp

HEADERS += \
    src/ui/ToolAdd/tooladdcircle.h \
    src/ui/ToolAdd/tooladdelement.h \
    src/ui/ToolAdd/tooladdline.h \
    src/ui/ToolAdd/tooladdpoint.h \
    src/ui/ToolAdd/tooladdrectangle.h \
    src/ui/ToolAdd/tooladdtext.h \
    src/ui/ToolAdd/tooladdtriangle.h \
    src/ui/ToolAdd/toolmoveelement.h \
    src/ui/colorwidget.h \
    src/ui/comboboxdelegate.h \
    src/ui/directionwidget.h \
    src/ui/elementsmodel.h \
    src/ui/elementswidget.h \
    src/ui/epaperwindow.h \
    src/ui/loggerconsole.h \
    src/ui/parameterdelegate.h \
    src/ui/parametereditor.h \
    src/ui/serialconfigwidget.h \
    src/ui/toolswidget.h \
    src/ui/widgetdisplay.h \
    src/core/drawable/drawablecircle.h \
    src/core/drawable/drawableelement.h \
    src/core/drawable/drawableimage.h \
    src/core/drawable/drawableline.h \
    src/core/drawable/drawablepoint.h \
    src/core/drawable/drawablerectangle.h \
    src/core/drawable/drawabletext.h \
    src/core/drawable/drawabletriangle.h \
    src/core/definition.h \
    src/core/displaymanager.h \
    src/core/epapercontroller.h \
    src/core/properties.h \
    src/core/serialdisplay.h \
    src/core/serialdriver.h \
    src/core/session.h \
    src/core/tools.h

INCLUDEPATH = $$PWD/src/ui $$PWD/src/core $$PWD/src/ui/ToolAdd $$PWD/src/core/drawable

