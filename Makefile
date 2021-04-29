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
	@cp .turtlerc.scm ~
	@cp turtle ~/.local/bin
	@mkdir -p ~/.local/share/turtle
	@cp -r lib/ ~/.local/share/turtle
	@echo "Installed turtle to ~/.local/bin"
uninstall:
	rm -rf ~/.turtle_history ~/.turtlerc.scm 
	rm -rf ~/.local/share/turtle ~/.local/bin/turtle

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) 

