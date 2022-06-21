
# A Makefile to build "server"

PROJECT 	= server
HEADER 		= $(PROJECT).h

C99 		= cc -std=c99
CFLAGS 		= -Wall -Werror -pedantic

OBJECT 		= execute.o server.o support.o Networking.o strsplit.o directories.o

$(PROJECT) 	:  $(OBJECT)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJECT)

%.o 		: %.c $(HEADER)
	$(C99) $(CFLAGS) -c $<

clean 		:
	rm -f $(PROJECT) $(OBJECT)
	