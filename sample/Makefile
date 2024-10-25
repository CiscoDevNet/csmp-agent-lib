#define your own gcc here if you need cross-compile the code
CC = gcc -Wall -Wextra

CFLAGS += -I ../include -I ../src/lib/trickle_timer/
#CFLAGS += -DPRINTDEBUG
LIBS += -lpthread

#if the openssl lib is availabe, please uncomment the following four lines
#CFLAGS += -DOPENSSL
#LIBS += -lssl
#LIBS += -lcrypto
#LIBS += -ldl

OBJ = CsmpAgentLib_sample.o signature_verify.o

LIB_OBJECT = csmp_agent_lib.a
OBJECT = CsmpAgentLib_sample

$(OBJECT):$(OBJ)
	$(CC) -o $@ $^ $(LIB_OBJECT) $(LIBS)

.c.o:
	$(CC) -c $< $(CFLAGS)

clean:
	-rm -rf *.o $(LIB_OBJECT) CsmpAgentLib_sample
