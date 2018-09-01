QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG += static
LIBS+= -dead_strip

DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        main.cpp \
    timeserver.cpp \
    tcptimeserver.cpp \
    udptimeserver.cpp \
    datetimeparseexception.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    timeserver.hpp \
    tcptimeserver.hpp \
    udptimeserver.hpp \
    datetimeparseexception.hpp \
    datetimemode.hpp
