#this is universal makefile
CC=g++
CLEAN=rm -f
PROGRAM_NAME=myclient
OBJECT_FILES=main.o
SOURCE_FILES=main.c

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) -pthread -o $@ $^

$(OBJECT_FILES): $(SOURCE_FILES)
	$(CC) -fPIC -c -pthread $^

clean:
	$(CLEAN) $(OBJECT_FILES)
	$(CLEAN) $(PROGRAM_NAME)
