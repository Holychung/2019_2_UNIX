CXX = g++
exe = 0516205_hw1
obj = main.o utils.o

.PHONY: all clean

all: $(obj)
	$(CXX) $(obj) -o $(exe)

%.o: %.cpp
	$(CXX) -c $?

clean:
	rm $(exe) $(obj)
