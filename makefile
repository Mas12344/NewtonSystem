LIBS=-lm -LInterval -lstdc++ -ldl -lmpfr -Lexprtk -LGaussJordanElim -LNewtonSystem -LWindow
CC=g++
GTKLIBS=`pkg-config gtkmm-3.0 --cflags --libs`
main: main.cpp
	$(CC) -o main -g main.cpp Window.o NewtonSystem.o GaussJordanElim.o $(LIBS) $(GTKLIBS)

window: Window.cpp
	$(CC) -c -g Window.cpp $(GTKLIBS)

newton: NewtonSystem.cpp
	$(CC) -c -g NewtonSystem.cpp $(LIBS)

gauss: GaussJordanElim.cpp
	$(CC) -c -g GaussJordanElim.cpp $(LIBS)

clean:
	@if [ -f Window.o ]; then rm Window.o; fi
	@if [ -f main ]; then rm main; fi
all:
	make clean
	make window
	make newton
	make gauss
	make main
	./main
run:
	./main