QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    form.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    form.h \
    widget.h

FORMS += \
    form.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../QT/5.14.2/mingw73_64/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../QT/5.14.2/mingw73_64/lib/ -lqwtd
else:unix: LIBS += -L$$PWD/../../QT/5.14.2/mingw73_64/lib/ -lqwt

INCLUDEPATH += $$PWD/../../QT/5.14.2/mingw73_64/include/Qwt
DEPENDPATH += $$PWD/../../QT/5.14.2/mingw73_64/include/Qwt

RESOURCES += \
    pic.qrc
