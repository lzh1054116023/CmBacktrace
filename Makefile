# Make Accelerometer & Gyroscope library

export

LIB_OUT = $(LIB_CM_BACKTRACE)

SRCS  := $(shell find ./cm_backtrace -name '*.c')
SRCA  := $(shell find ./cm_backtrace/fault_handler/gcc -name '*.S')
HEADS := $(shell find ./cm_backtrace -name '*.[h]')

LIB_OBJS  = $(sort $(patsubst %.c,%.o,$(SRCS)))
LIB_OBJS += $(sort $(patsubst %.S,%.o,$(SRCA)))

$(warning  "1111111111111 LIB_OUT  = $(LIB_OUT)")
$(warning  "1111111111112 SRCS     = $(SRCS)")
$(warning  "1111111111113 HEADS    = $(HEADS)")
$(warning  "1111111111114 LIB_OBJS = $(LIB_OBJS)")

.PHONY: all
.PHONY: clean

all: $(LIB_OUT)

$(LIB_OUT): $(LIB_OBJS) $(HEADS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

clean:
	-rm -f $(LIB_OBJS) $(LIB_OUT)
