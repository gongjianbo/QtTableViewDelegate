QT += core gui widgets

CONFIG += c++11 utf8_source


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/DemoStandardModel
include($$PWD/DemoStandardModel/DemoStandardModel.pri)

INCLUDEPATH += $$PWD/DemoTableModel
include($$PWD/DemoTableModel/DemoTableModel.pri)

INCLUDEPATH += $$PWD/DemoStyledDelegate
include($$PWD/DemoStyledDelegate/DemoStyledDelegate.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
