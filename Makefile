DC=dmd
OBJS= src/builtins.o src/shell.o src/main.o
BIN=dsh
FLAGS=-Isrc/
all: $(BIN)

dsh: $(OBJS)
	$(DC) $(FLAGS) $(OBJS) -of=$(BIN)
%.o : %.d	
	$(DC) $(FLAGS) -c $< -of=$@
clean:
	rm $(OBJS) $(BIN)
