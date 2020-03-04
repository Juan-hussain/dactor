#-------------------------------------------------
#
# Project created by QtCreator 2019-06-20T11:10:17
#
#-------------------------------------------------

QT       += core gui sql \
            multimedia \
            location \
            concurrent\


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DaCToR
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

CONFIG += c++11

SOURCES += \
        src/loginform.cpp \
        src/conditionaltransitions.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
        src/recstatemachine.cpp \
        src/settings.cpp \
        src/speakerprofileform.cpp \
        src/audiorecorder.cpp \
        src/stmmanager.cpp \
        src/user.cpp \
        src/userRecords.cpp \
        src/userRecords_list.cpp \
        src/util.cpp \
        src/wavepcmfile.cpp


HEADERS += \
        headers/loginform.h \
        headers/conditionaltransitions.h \
        headers/mainwindow.h \
        headers/options.h \
        headers/recstatemachine.h \
        headers/settings.h \
        headers/speakerprofileform.h \
        headers/audiorecorder.h \
        headers/stmmanager.h \
        headers/transitions.h \
        headers/user.h \
        headers/userRecords.h \
        headers/userRecords_list.h \
        headers/util.h \
        headers/wavepcmfile.h

FORMS += \
        ui/loginform.ui \
        ui/mainwindow.ui \
        ui/settings.ui \
        ui/speakerprofileform.ui


RESOURCES += \
    resources/icons.qrc

DISTFILES += \
    resources/stylesheet.qss


RC_FILE = resources\icons\VR.rc

# Default rules for deployment.
target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audioinput
INSTALLS += target

