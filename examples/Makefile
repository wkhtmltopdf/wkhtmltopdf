CFLAGS= -I ../include 
LDFLAGS=-L ../bin -lwkhtmltox -Wall -ansi -pedantic -ggdb

EXES=pdf_c_api

all: $(EXES)

.PHONY: clean run all

run: $(EXES)
	LD_LIBRARY_PATH=../bin ./pdf_c_api

clean:
	$(RM) $(EXES)

