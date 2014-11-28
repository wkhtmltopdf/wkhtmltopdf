TEMPLATE = app
TARGET = pdf_c_api

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += pdf_c_api.c
DESTDIR = ../../bin
DEPENDPATH += ../../include
INCLUDEPATH += ../../include
LIBS += -L../../bin -lwkhtmltox
