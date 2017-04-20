CC ?= gcc
PRG_SUFFIX_FLAG := 0
LDFLAGS := -lm -lmpdclient
CFLAGS_INC :=
CFLAGS := -std=c99 -O3 -Wall $(CFLAGS_INC)

SRCS := $(wildcard *.c)
PRGS := $(patsubst %.c,%,$(SRCS))

PRG_SUFFIX=.exe
BINS := $(patsubst %,%$(PRG_SUFFIX),$(PRGS))

OBJS := $(patsubst %,%.o,$(PRGS))

all : $(BINS)

.SECONDEXPANSION:
OBJ = $(patsubst %$(PRG_SUFFIX),%.o,$@)
ifeq ($(PRG_SUFFIX_FLAG),0)
        BIN = $(patsubst %$(PRG_SUFFIX),%,$@)
else
        BIN = $@
endif

%$(PRG_SUFFIX) : $(OBJS)
	$(CC) $(OBJ)  $(LDFLAGS) $(CFLAGS) -o $(BIN)

clean:
ifeq ($(PRG_SUFFIX_FLAG),0)
	$(RM) $(PRGS)
else
	$(RM) $(BINS)
endif
	$(RM) *~

rebuild: clean all
