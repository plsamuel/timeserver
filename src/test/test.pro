include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += static
LIBS+= -dead_strip

HEADERS += \
        tst_parsedatetimemode.h \
        tst_parsedatetimegenerate.h

SOURCES += \
        main.cpp \
        ../app/datetimeparseexception.cpp

