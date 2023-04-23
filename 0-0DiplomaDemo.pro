QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosecpudialog.cpp \
    choosegpudialog.cpp \
    choosembdialog.cpp \
    choosenetworkcarddialog.cpp \
    chooseramdialog.cpp \
    choosesoundcarddialog.cpp \
    cpu.cpp \
    item.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    choosecpudialog.h \
    choosegpudialog.h \
    choosembdialog.h \
    choosenetworkcarddialog.h \
    chooseramdialog.h \
    choosesoundcarddialog.h \
    cpu.h \
    item.h \
    mainwindow.h

FORMS += \
    choosecpudialog.ui \
    choosegpudialog.ui \
    choosembdialog.ui \
    choosenetworkcarddialog.ui \
    chooseramdialog.ui \
    choosesoundcarddialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
