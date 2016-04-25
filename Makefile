CC ?= gcc
CMFLAGS := -std=c89 -MMD -MP -Wall -Wextra -Waddress -ffunction-sections -fdata-sections -Werror -Wno-unused-function -o
CFLAGS := -c $(CMFLAGS)
LFLAGS := -Wl,--gc-sections $(CMFLAGS)

AR ?= ar
ARFLAGS := rcs

OBJS := cvec.o
TEST_OBJS := test.o

ifneq ($(filter Windows%,$(OS)),)
TEST_BIN := test.exe
else
TEST_BIN := test
endif

.PHONY: all test
all: libcvec.a
test: $(TEST_BIN)

$(TEST_BIN): libcvec.a $(OBJS) $(TEST_OBJS)
	$(CC) $(LFLAGS) $@ $^
	./$@

libcvec.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $@ $(filter %.c,$^)

-include $(OBJS:%.o=%.d)
-include $(TEST_OBJS:%.o=%.d)

clean:
	rm -f libcvec.a $(TEST_BIN) $(OBJS) $(TEST_OBJS) $(OBJS:%.o=%.d) $(TEST_OBJS:%.o=%.d)
