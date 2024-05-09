INCLDIR = ./include
TARGET = oppgave_4
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)
LINKTHREAD = -lpthread

OBJDIR = ./obj

_DEPS = oppgave4_clean_up.h sha1.h oppgave4_defs.h oppgave4_struct.h oppgave4_create_hash.h oppgave4_read_file.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o oppgave4_clean_up.o sha1.o oppgave4_create_hash.o oppgave4_read_file.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKTHREAD)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~



