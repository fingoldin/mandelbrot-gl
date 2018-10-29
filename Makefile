CXX := g++ -std=c++11
FLAGS := -Wall
LDFLAGS := -lm -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lpthread -ldl -lXrandr

mandelbrot-gl: main.cpp core.h inputhandler.h shader.h utils.h
	$(CXX) $(FLAGS) main.cpp -o $@ $(LDFLAGS)
