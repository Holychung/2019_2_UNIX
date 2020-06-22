CC = gcc
EXE = sandbox
O_OBJ = *.so
C_OBJ_MAIN = sandbox.c
C_OBJ_LIB = sandboxso.c
SO_OBJ = sandboxso.so

LD_FLAGS = -shared -fPIC
FLAGS = -ldl

.PHONY: all clean

# $< first prerequisite
all: $(C_OBJ_MAIN) $(C_OBJ_LIB)
	$(CC) -o $(EXE) -Wall $<
	$(CC) -o $(SO_OBJ) $(LD_FLAGS) $(C_OBJ_LIB) $(FLAGS)

clean:
	rm $(exe) $(O_OBJ)
