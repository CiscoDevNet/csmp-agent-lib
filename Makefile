CC = gcc
AR = ar rc

DIRs += $(shell find ./src -maxdepth 3 -type d)

#CFLAGS += -DPRINTDEBUG
CFLAGS += $(foreach dir, $(DIRs), -I $(dir))

LIBS += -lpthread

LIBSRC = $(foreach dir, $(DIRs), $(wildcard $(dir)/*.c))
LIBOBJ_nodir = $(notdir $(LIBSRC))
LIBOBJ = $(patsubst %.c,%.o,$(LIBOBJ_nodir))

LIB_OBJECT = csmp_agent_lib.a

$(LIB_OBJECT):$(LIBOBJ)
	$(AR) $@ $^

$(LIBOBJ):$(LIBSRC)
	$(CC) -c $(CFLAGS) $^ $(LIBS)

clean:
	-rm -rf *.o $(LIB_OBJECT)
