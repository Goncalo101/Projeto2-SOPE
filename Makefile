CC=gcc
CFLAGS = -Wall -Wextra -pedantic
DEPS = constants.h types.h accounts.h types.h sope.h
COMN = log.o 
SRV = server.o accounts.o
USR = user.o

all: user server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

user: $(USR) $(COMN) 
	$(CC) -o $@ $^ $(CFLAGS)

server:  $(SRV) $(COMN) 
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o user server
	
clean-logs:
	rm *.txt
	
format:
	clang-format -i *.c *.h -style=WebKit


