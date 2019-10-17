.phony: all clean

all:ge

ge:main.cpp
	g++ -g  -o ge main.cpp 

clean: 
	rm ge
