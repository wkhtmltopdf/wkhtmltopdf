include(../../version.pri)
include(../../common.pri)

TEMPLATE = app
DESTDIR = ../../bin
TARGET = wkhtmltopdf
DEPENDPATH += . ../shared
INCLUDEPATH += . ../shared

readme.target=README
readme.commands=./wkhtmltopdf --readme > README
readme.depends=wkhtmltopdf

QMAKE_EXTRA_TARGETS += readme

unix {
    man.target=wkhtmltopdf.1.gz
    man.commands=./wkhtmltopdf --manpage | gzip > $@
    man.depends=wkhtmltopdf

    manins.target=manins
    manins.depends=man
    manins.files=wkhtmltopdf.1.gz
    manins.path=$$INSTALLBASE/share/man/man1

    QMAKE_EXTRA_TARGETS += manins man
    INSTALLS += manins
}

INSTALLS += target
target.path=$$INSTALLBASE/bin

include(../shared/shared.pri)

#Libaray part
HEADERS += pdfconverter.hh pdfconverter_p.hh

SOURCES += settings.cc pdfconverter.cc \
           outline.cc tocstylesheet.cc

#Application part

SOURCES += wkhtmltopdf.cc arguments.cc commandlineparser.cc \
           docparts.cc
           

