TEMPLATE = app
TARGET = Matrix_Calculator

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    MainWindow_functions.cpp \
    MainWindow_interface.cpp \
    Matrix.cpp \
    MenuBar.cpp \
    MenuWindows.cpp \
    TableWidget.cpp \
    main.cpp \
    notification.cpp
HEADERS += \
    MainWindow.h \
    Matrix.h \
    MenuBar.h \
    MenuWindows.h \
    TableWidget.h \
    notification.h

RESOURCES += \
    Resources.qrc

DISTFILES += \
    README.md
