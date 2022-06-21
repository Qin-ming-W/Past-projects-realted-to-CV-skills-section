# A Makefile to build 'duplicates' project

C11 		= cc -std=c11
CFLAGS 		= -Wall -Werror -pedantic

PROJECT 	= duplicates
HEADERS		= $(PROJECT).h
OBJ 		= duplicates.o scanning.o statistic.o options-support.o \
		      strSHA2.o build-file.o build-filetable.o show.o


$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) *.o