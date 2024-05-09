INCLDIR = ./include
TARGET = oppgave_6
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

_DEPS = oppgave6_extract_body_from_while_loop.h oppgave6_extract_condition_from_while_loop.h oppgave6_extract_incrementer_from_while_loop.h oppgave6_extract_init_from_while_loop.h oppgave6_generate_for_loop.h oppgave6_cleanup.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o oppgave6_extract_body_from_while_loop.o oppgave6_extract_condition_from_while_loop.o oppgave6_extract_incrementer_from_while_loop.o oppgave6_extract_init_from_while_loop.o oppgave6_generate_for_loop.o oppgave6_cleanup.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~



