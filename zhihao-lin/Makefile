all: cad

cad: main.o
	@g++ -Wall -O3 $<  -o $@

main.o: main.cpp util.h chip.h layer.h window.h polygon.h
	@g++ -Wall -O3 -c $<

clean:
	@rm main.o cad
