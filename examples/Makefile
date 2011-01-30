CFLAGS= -I ../include 
LDFLAGS=-L ../bin -lwkhtmltox -Wall -ansi -pedantic -ggdb

EXES=pdf_c_api image_c_api

all: $(EXES)

.PHONY: clean run all

run: pdf_c_api
	LD_LIBRARY_PATH=../bin ./pdf_c_api 

runimage: image_c_api
	LD_LIBRARY_PATH=../bin ./image_c_api

clean:
	$(RM) $(EXES)

