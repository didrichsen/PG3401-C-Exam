INCLDIR = ./include
TARGET = oppgave_5
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

LINKTHREAD = -lpthread

OBJDIR = ./obj

_DEPS = oppgave5_server.h oppgave5_client.h oppgave5_generateRandomIPAddress.h oppgave5_valid_address.h oppgave5_network_defs.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o oppgave5_server.o oppgave5_client.o oppgave5_generateRandomIPAddress.o oppgave5_valid_address.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKTHREAD)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~



