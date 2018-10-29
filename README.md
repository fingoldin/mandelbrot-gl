# mandelbrot-gl

A real-time, navigable mandelbrot set renderer using only OpenGL calls. 

## Usage

run `make` in this directory. Then, run `mandelbrot-gl`. Navigate using the following keys:
`WASD`: Move the center of the viewport around
Up arrow key: Zoom in (multiply zoom factor by 0.9)
Donw arrow key: Zoom out (divide zoom factor by 0.9)
`z`: Decrease max iterations (multiply by a factor of 0.9)
`x`: Increase max iterations (divide by a factor of 0.9)

## Screenshots

![](https://www.github.com/fingoldin/mandelbrot-gl/raw/master/image.png)
![](https://www.github.com/fingoldin/mandelbrot-gl/raw/master/image2.png)

Both images were rendered with a maximum of 256 iterations
