QT += core xml gui multimedia
QT += widgets

TARGET = tihu_console
TEMPLATE = app

HEADERS +=  ./tihu_console.h
SOURCES +=  ./main.cpp \
            ./tihu_console.cpp
FORMS +=    ./tihu_console.ui
RESOURCES += ./tihu_console.qrc

### to copy target file to build folder
CONFIG(debug, debug|release) {
    TARGET_PATH = $$OUT_PWD/debug
}
CONFIG(release, debug|release) {
    TARGET_PATH = $$OUT_PWD/release
}

win32: QMAKE_POST_LINK += copy /y "$$shell_path($$TARGET_PATH/tihu_console.exe)" "$$shell_path($$PWD/../build/)"
unix: QMAKE_POST_LINK += cp "$$shell_path($$TARGET_PATH/tihu_console)" "$$shell_path($$PWD/../build/)"
