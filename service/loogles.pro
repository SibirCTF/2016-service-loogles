TEMPLATE = app
TARGET = loogles

QT += core sql network
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
OBJECTS_DIR = tmp/
MOC_DIR = tmp/
RCC_DIR = tmp/
CONFIG  += c++11 c++14

# Joyent http_parser
SOURCES  += src/qhttp/3rdparty/http-parser/http_parser.c
HEADERS  += src/qhttp/3rdparty/http-parser/http_parser.h

INCLUDEPATH +=  . src/qhttp/src src/qhttp/3rdparty

SOURCES += \
    src/qhttp/src/qhttpabstracts.cpp \
    src/qhttp/src/qhttpserverconnection.cpp \
    src/qhttp/src/qhttpserverrequest.cpp \
    src/qhttp/src/qhttpserverresponse.cpp \
    src/qhttp/src/qhttpserver.cpp \
    src/dbconnection.cpp \
	src/main.cpp \
	src/log.cpp \
	src/scanner.cpp \


HEADERS += \
	src/qhttp/src/qhttpfwd.hpp \
    src/qhttp/src/qhttpabstracts.hpp \
    src/qhttp/src/qhttpserverconnection.hpp \
    src/qhttp/src/qhttpserverrequest.hpp \
    src/qhttp/src/qhttpserverresponse.hpp \
    src/qhttp/src/qhttpserver.hpp \
    src/dbconnection.h \
	src/log.h \
	src/scanner.h \
