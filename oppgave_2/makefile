INCLDIR = ./include
TARGET = oppgave_2
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

_DEPS = eksamen_v24_oppgave2_palindrom.h eksamen_v24_oppgave2_heterogram.h eksamen_v24_oppgave2_all_upper.h eksamen_v24_oppgave2_all_lower.h eksamen_v24_oppgave2_anagram.h eksamen_v24_oppgave2_disjoint.h oppgave2_bin_to_char.h oppgave2_constants.h oppgave2_task2_world_metadata.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o eksamen_v24_oppgave2_palindrom.o eksamen_v24_oppgave2_heterogram.o eksamen_v24_oppgave2_all_upper.o eksamen_v24_oppgave2_all_lower.o eksamen_v24_oppgave2_anagram.o eksamen_v24_oppgave2_disjoint.o oppgave2_binToChar.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~