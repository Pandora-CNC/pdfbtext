CC=armarmv5te-unknown-linux-gnueabi-gcc
CC_ARGS=
LD_ARGS=--static

all: application

application: font.o pdfbtext.o application.o
	$(CC) $(LD_ARGS) -o application font.o pdfbtext.o application.o

font.o: font.c
	$(CC) $(CC_ARGS) -o font.o -c font.c

pdfbtext.o: pdfbtext.c
	$(CC) $(CC_ARGS) -o pdfbtext.o -c pdfbtext.c

application.o: application.c
	$(CC) $(CC_ARGS) -o application.o -c application.c

clean:
	rm -f application *.o
