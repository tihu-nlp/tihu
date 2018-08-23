QT += core xml gui multimedia
QT += widgets

TARGET = tihu_console
TEMPLATE = app

HEADERS +=  ./tihu_console.h
SOURCES +=  ./main.cpp \
            ./tihu_console.cpp
FORMS +=    ./tihu_console.ui
RESOURCES += ./tihu_console.qrc

unix {

    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path = $$PREFIX/bin
}

INSTALLS += target
