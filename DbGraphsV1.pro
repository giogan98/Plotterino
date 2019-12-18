QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Completion/getcomplsuggestions.cpp \
    DBmanaging/IScanLogDatabase.cpp \
    Driver/ILog.cpp \
    Graphs/qcustombars.cpp \
    Graphs/qcustomplot.cpp \
    Processing/IProcessBase.cpp \
    Processing/IProcessGen.cpp \
    Processing/IProcessLog.cpp \
    Processing/IProcessMC.cpp \
    Processing/IProcessTable.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Completion/getcomplsuggestions.h \
    DBmanaging/IScanLogDatabase.h \
    Driver/ILog.h \
    Graphs/qcustombars.h \
    Graphs/qcustomplot.h \
    Processing/IProcessBase.h \
    Processing/IProcessGen.h \
    Processing/IProcessLog.h \
    Processing/IProcessMC.h \
    Processing/IProcessTable.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
