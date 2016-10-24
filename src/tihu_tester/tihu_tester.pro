QT += core gui xml concurrent widgets
TARGET = tihu_tester
TEMPLATE = app
CONFIG += c++11


SOURCES +=  main.cpp\
            mainwindow.cpp \
            result_table.cpp

HEADERS  += mainwindow.h \
            result_table.h

RESOURCES += \
            tihu_tester.qrc
            
unix|win32: LIBS += -L$$PWD/../build/ -ltihu
INCLUDEPATH += $$PWD/../tihu

### to copy target file to build folder
CONFIG(debug, debug|release) {
    TARGET_PATH = $$OUT_PWD/debug
}
CONFIG(release, debug|release) {
    TARGET_PATH = $$OUT_PWD/release
}

win32: QMAKE_POST_LINK += copy /y "$$shell_path($$TARGET_PATH/tihu_tester.exe)" "$$shell_path($$PWD/../build/)"
unix: QMAKE_POST_LINK += cp "$$shell_path($$TARGET_PATH/tihu_tester)" "$$shell_path($$PWD/../build/)"
