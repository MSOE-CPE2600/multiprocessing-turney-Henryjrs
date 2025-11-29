CC=gcc
CFLAGS=-c -Wall -g
MANDLDFLAGS=-ljpeg -pthread
MOVIELDFLAGS=
MANDSOURCES= mandel.c jpegrw.c
MOVIESOURCES= movie.c
EXECUTABLE=mandel movie

all: $(SOURCES) $(EXECUTABLE) 

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

mandel:
	$(CC) $(MANDSOURCES) $(MANDLDFLAGS) -o mandel

movie:
	$(CC) $(MOVIESOURCES) $(MOVIELDFLAGS) -o movie

clean:
	rm -rf $(EXECUTABLE)
