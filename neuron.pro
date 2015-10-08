TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS=-L/usr/local/lib -lpthread -levent
INCLUDEPATH += "/usr/local/include"

SOURCES += main.c \
    fixarray.c \
    neu-pdu.c \
    neu-service.c \
    neu-session.c \
    neu-worker.c \
    neulog.c

include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    logger.js \
    neuron.pro.user \
    LICENSE \
    Makefile \
    README.md \
    spec.md

HEADERS += \
    fixarray.h \
    neu-handle.h \
    neu-msg.h \
    neu-pdu.h \
    neu-service.h \
    neu-session.h \
    neu-worker.h \
    neulog.h \
    neuron.h

