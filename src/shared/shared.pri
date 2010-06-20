#Lbaray Part
HEADERS += ../shared/multipageloader_p.hh ../shared/multipageloader.hh ../shared/converter.hh \
	 ../shared/converter_p.hh ../shared/progressfeedback.hh
SOURCES += ../shared/loadsettings.cc ../shared/multipageloader.cc ../shared/tempfile.cc \
	../shared/converter.cc ../shared/websettings.cc ../shared/progressfeedback.cc

#Application part
HEADERS +=  ../shared/utilities.hh

SOURCES += ../shared/outputter.cc ../shared/manoutputter.cc ../shared/htmloutputter.cc \
           ../shared/textoutputter.cc ../shared/arghandler.cc ../shared/commondocparts.cc \
 	   ../shared/commandlineparserbase.cc ../shared/commonarguments.cc ../shared/utilities.cc
           
