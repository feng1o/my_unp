.PHONEY:clean 

cc = gcc
cflags= -Wall -g 

BIN = vx-client vx-server
all:$(BIN)
%.o:%.c
	$(cc) $(cflags) $< -o $@

clean:
	rm $(BIN)
