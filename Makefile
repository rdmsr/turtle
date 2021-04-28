TARGET ?= turtle
SRC_DIRS ?= ./src
CC = clang
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
ROOT ?= sudo
CFLAGS ?= -ansi  -g -Wall -Wextra -Wno-unused-result -Werror -Wno-unused-function -Isrc/ `pkg-config --cflags guile-2.2`
LDLIBS = -lreadline `pkg-config --libs guile-2.2`
%.o: %.c
	@echo [ CC ] $<
	@$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJS)

	@echo [ BIN ] $<
	@$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBS) $(LDLIBS)

install:
	cp .turtlerc.scm ~
	$(ROOT) cp turtle /usr/local/bin
uninstall:
	rm -rf ~/.turtle_history ~/.turtlerc.scm
	$(ROOT) rm -rf /usr/local/bin

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) 

