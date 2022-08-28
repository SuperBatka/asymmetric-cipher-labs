QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "$$_PRO_FILE_PWD_/include"
win32:LIBS += "-L$$_PRO_FILE_PWD_/lib" -lgmpxx -lgmp


SOURCES += \
    Elgamal.cpp \
    Elgamal_MPW.cpp \
    Elgamal_eds_MPW.cpp \
    Gost3410_12_MPW.cpp \
    Hash_MPW.cpp \
    Rsa_eds_MPW.cpp \
    deffiehellman.cpp \
    deffiehellman_MPW.cpp \
    gost3410_12.cpp \
    main.cpp \
    mainwindow.cpp \
    Stacked_W.cpp \
    Rsa_MPW.cpp \
    md5.cpp \
    rsa.cpp \
    sha1.cpp \
    shamir.cpp \
    shamir_mpw.cpp \
    streebog.cpp


HEADERS += \
    Elgamal.h \
    Elgamal_MPW.h \
    Elgamal_eds_MPW.h \
    Gost3410_12_MPW.h \
    HashI.h \
    Hash_MPW.h \
    Rsa_eds_MPW.h \
    deffiehellman.h \
    deffiehellman_MPW.h \
    gost3410_12.h \
    mainwindow.h \
    Stacked_W.h \
    Rsa_MPW.h \
    md5.h \
    rsa.h \
    sha1.h \
    shamir.h \
    shamir_mpw.h \
    streebog.h

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
QMAKE_LFLAGS_DEBUG += -static -static-libgcc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    Gost3410_12_MPW.ui \
    Hash_MPW.ui

