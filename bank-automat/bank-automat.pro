QT       += core gui
QT +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    balance.cpp \
    environment.cpp \
    login.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    selectaccount.cpp \
    transactions.cpp \
    withdraw.cpp

HEADERS += \
    balance.h \
    environment.h \
    login.h \
    mainmenu.h \
    mainwindow.h \
    selectaccount.h \
    transactions.h \
    withdraw.h

FORMS += \
    balance.ui \
    login.ui \
    mainmenu.ui \
    mainwindow.ui \
    selectaccount.ui \
    transactions.ui \
    withdraw.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
