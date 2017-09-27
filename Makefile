CXX := g++ -std=c++11
FLAGS := -Wall
LDFLAGS := -lm -lGL -lGLEW -lglfw3 -lX11 -lXxf86vm -lpthread -ldl -lXrandr -lXi -lXinerama -lXcursor

mandelbrot-gl: main.cpp core.h inputhandler.h shader.h utils.h
	$(CXX) $(FLAGS) main.cpp -o $@ $(LDFLAGS)
