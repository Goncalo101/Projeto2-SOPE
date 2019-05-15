CC=gcc
CFLAGS = -Wall -Wextra -pedantic
DEPS = constants.h types.h accounts.h sope.h userflag.h communication.h serverfifoaux.h answerfifoaux.h crypto.h define.h
COMN = log.o communication.o crypto.o
SRV = server.o accounts.o answerfifoaux.o
USR = user.o userflag.o serverfifoaux.o

all: user server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

user: $(USR) $(COMN) 
	$(CC) -o $@ $^ $(CFLAGS)

server:  $(SRV) $(COMN) 
	$(CC) -o $@ $^ $(CFLAGS) -lpthread

clean:
	rm -f *.o user server /tmp/secure* /tmp/pipe*
	
clean-logs:
	rm *.txt
	
format:
	clang-format -i *.c *.h -style=WebKit


