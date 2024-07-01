CC = gcc
CFLAGS = -O2 
LIBS = -lcurl -lxml2
SRCDIR = sources
OBJDIR = objects

SOURCES = $(wildcard $(SRCDIR)/*.c) 

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: main

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	 rm -f $(OBJECTS) main

