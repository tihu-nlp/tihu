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

win32: QMAKE_POST_LINK += copy /y "$$shell_path($$DESTDIR/tihu_tester.exe)" "$$shell_path($$PWD/../build/)"
unix: QMAKE_POST_LINK += cp "$$shell_path($$DESTDIR/tihu_tester.exe)" "$$shell_path($$PWD/../build/)"
