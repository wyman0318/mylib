BIN = main
SRC = main.c pianostart.c bmpauto.c button.c list.c
CLE = ../main

CC = arm-none-linux-gnueabi-gcc

CPPFLAGS = -I ../inc

LDFLAGS += -lpthread

$(BIN):$(SRC)
	$(CC) $^ -o $@ $(CPPFLAGS) $(LDFLAGS)
	mv $@ ..

clean:
	$(RM) $(CLE)

.PHONY:clean	
