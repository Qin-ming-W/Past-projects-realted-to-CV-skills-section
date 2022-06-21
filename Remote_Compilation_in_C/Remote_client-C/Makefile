
# A Makefile to build "rake-c" 

PROJECT 	= rake-c
HEADER 		= $(PROJECT).h

C99 		= cc -std=c99
CFLAGS 		= -Wall -Werror -pedantic

OBJECT 		= strsplit.o rake.o Rakefile_parser.o parser_debugger.o choose_server.o \
		 		execute.o support.o Networking.o

$(PROJECT) 	: $(OBJECT)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJECT) 

%.o 		: %.c $(HEADER)
	$(C99) $(CFLAGS) -c $<

clean		:
	rm -f rake-c *.o		