include(../../common.pri)

MOC_DIR = ../../build/image
OBJECTS_DIR = ../../build/image
UI_DIR = ../../build/image

TEMPLATE = app
TARGET = wkhtmltoimage
DESTDIR = ../../bin
DEPENDPATH += . ../shared
INCLUDEPATH += . ../shared

readme.target=README_WKHTMLTOIMAGE
readme.commands=./wkhtmltoimage --readme > README_WKHTMLTOIMAGE
readme.depends=wkhtmltoimage

QMAKE_EXTRA_TARGETS += readme

unix {
    man.target=wkhtmltoimage.1.gz
    man.commands=./wkhtmltoimage --manpage | gzip > $@
    man.depends=wkhtmltoimage

    manins.target=manins
    manins.depends=man
    manins.files=wkhtmltoimage.1.gz
    manins.path=$$INSTALLBASE/share/man/man1

    QMAKE_EXTRA_TARGETS += manins man
    INSTALLS += manins
}

INSTALLS += target
target.path=$$INSTALLBASE/bin

# Input
HEADERS += pageloader.hh settings.hh converter.hh
SOURCES += wkhtmltoimage.cc arguments.cc commandlineparser.cc docparts.cc \
           converter.cc utilities.cc \

include(../shared/shared.pri)
