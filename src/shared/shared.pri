#Lbaray Part
HEADERS += ../shared/multipageloader_p.hh ../shared/multipageloader.hh
SOURCES += ../shared/loadsettings.cc ../shared/multipageloader.cc ../shared/tempfile.cc 

#Application part

SOURCES += ../shared/outputter.cc ../shared/manoutputter.cc ../shared/htmloutputter.cc \
           ../shared/textoutputter.cc ../shared/arghandler.cc ../shared/commondocparts.cc \
 	   ../shared/commandlineparserbase.cc ../shared/commonarguments.cc
           
