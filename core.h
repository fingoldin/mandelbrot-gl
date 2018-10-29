#ifndef CORE_H
#define CORE_H

#include <cstdlib>
#include <cstdio>

#include <string>
#include <vector>
#include <cmath>
#include <cstring>

#ifndef GLEW_STATIC
    #define GLEW_STATIC
#endif
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "inputhandler.h"
#include "shader.h"


class Core
{
public:

    void begin(const char * winName);

    void end(void);

    inline bool shouldClose(void) const;

    void update(void);

    Core() {}
    ~Core() {}

private:

    void render(void);

    inline void clearScreen(void);

    inline void updateScreen(void);

    void init_gl(void);
    void init_shaders(void);

    GLint gl_context_version_major,
          gl_context_version_minor,
          gl_samples,
          gl_opengl_profile,
          gl_resizable,
          gl_opengl_forward_compat,

          viewport_x,
          viewport_y,
          viewport_w,
          viewport_h;

    GLuint window_width,
           window_height;

    GLclampf clear_color[3];

    unsigned int VBO;
    unsigned int VAO;

    GLfloat * vertices;

    GLint max_iters;
    GLint max_max_iters;
    GLint min_max_iters;

    std::string window_name;

    std::string mandelbrot_vertex_shader_path;
    std::string mandelbrot_fragment_shader_path;

    Shader * mandelbrot_shader;
    Shader * current_shader;

    GLFWmonitor * window_monitor;
    GLFWwindow * window_share;
    GLFWwindow * window;
    bool fullscreen;

    long double center_z[2];
    long double zoom;

    bool firstupdate;
};

void Core::begin(const char * winName)
{
    this->gl_context_version_major = 4;
    this->gl_context_version_minor = 1;
    this->gl_samples = 2;
    this->gl_opengl_profile = GLFW_OPENGL_CORE_PROFILE;
    this->gl_resizable = GL_FALSE;
    this->gl_opengl_forward_compat = GL_TRUE;

    this->window_width = 1920;
    this->window_height = 1200;
    this->viewport_x = 0;
    this->viewport_y = 0;
    this->viewport_w = this->window_width;
    this->viewport_h = this->window_height;

    this->window_name = winName;

    this->mandelbrot_vertex_shader_path = "shader.vs";
    this->mandelbrot_fragment_shader_path = "shader.frag";

    this->window_monitor = NULL;
    this->window_share = NULL;
    this->window = NULL;
    this->fullscreen = true;

    this->VBO = 0;
    this->VAO = 0;

    this->clear_color[0] = 0.0f;
    this->clear_color[1] = 0.0f;
    this->clear_color[2] = 0.0f;

    this->init_gl();
    this->init_shaders();

    this->max_iters = 256;
    this->max_max_iters = 2048;
    this->min_max_iters = 10;

    this->current_shader = this->mandelbrot_shader;

    this->center_z[0] = 0.275f;
    this->center_z[1] = 0.0f;
    this->zoom = 0.6f;

    this->firstupdate = true;

    InputHandler::begin();
}

void Core::end(void)
{
    delete this->mandelbrot_shader;

    free(this->vertices);

    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);

    glfwTerminate();
}

bool Core::shouldClose() const
{
    return glfwWindowShouldClose(this->window);
}

void Core::update()
{
    glfwPollEvents();

    bool shouldrender = false;

    if(InputHandler::getKey(GLFW_KEY_UP)) {
        this->zoom = this->zoom * 0.9L;
        shouldrender = true;
    }
    else if(InputHandler::getKey(GLFW_KEY_DOWN)) {
        this->zoom = this->zoom / 0.9L;
        shouldrender = true;
    }

    long double move_c = this->zoom * 0.005L;

    if(InputHandler::getKey(GLFW_KEY_W)) {
        this->center_z[1] += move_c;
        shouldrender = true;
    }
    else if(InputHandler::getKey(GLFW_KEY_S)) {
        this->center_z[1] -= move_c;
        shouldrender = true;
    }

    if(InputHandler::getKey(GLFW_KEY_D)) {
        this->center_z[0] += move_c;
        shouldrender = true;
    }
    else if(InputHandler::getKey(GLFW_KEY_A)) {
        this->center_z[0] -= move_c;
        shouldrender = true;
    }

    if(InputHandler::getKey(GLFW_KEY_X)) {
        this->max_iters = (this->max_iters * 10) / 9;
	if(this->max_iters > this->max_max_iters)
	    this->max_iters = this->max_max_iters;

        shouldrender = true;
    }
    else if(InputHandler::getKey(GLFW_KEY_Z)) {
        this->max_iters = (9 * this->max_iters) / 10;
	if(this->max_iters < this->min_max_iters)
            this->max_iters = this->min_max_iters;

	shouldrender = true;
    }

    if(InputHandler::getKey(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(this->window, 1);
        shouldrender = false;
    }

    if(shouldrender || firstupdate)
    {
        GLfloat stime = glfwGetTime();

        this->current_shader->Use();

        glUniform2f(glGetUniformLocation(this->current_shader->program(), "center_z"), (float)this->center_z[0], (float)this->center_z[1]);
        glUniform1f(glGetUniformLocation(this->current_shader->program(), "screen_ratio"), (float)this->window_height / (float)this->window_width);
        glUniform1f(glGetUniformLocation(this->current_shader->program(), "zoom"), (float)this->zoom);
        glUniform1f(glGetUniformLocation(this->current_shader->program(), "max_iters"), (float)this->max_iters);

        this->clearScreen();

        this->render();

        this->updateScreen();

        GLfloat etime = glfwGetTime();
        GLfloat dtime = etime - stime;

        printf("Render time: %.2f millis   Center: %.16Lf %.16Lf   Zoom: %.16Lf   Iters: %d\n", dtime * 1000.0f, this->center_z[0], this->center_z[1], this->zoom, this->max_iters);
    }

    if(firstupdate)
        firstupdate = false;
}

void Core::render(void)
{
    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Core::clearScreen(void)
{
    glClearColor(this->clear_color[0], this->clear_color[1], this->clear_color[2], this->clear_color[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Core::updateScreen(void)
{
    glfwSwapBuffers(this->window);
}

void Core::init_gl(void)
{
    if(glfwInit() != GL_TRUE) {
        std::printf("\nERROR in Core::init_gl(void): could not initialize GLFW!\nTerminating program...\n");
        std::exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->gl_context_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->gl_context_version_minor);
    glfwWindowHint(GLFW_SAMPLES, this->gl_samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, this->gl_opengl_profile);
    glfwWindowHint(GLFW_RESIZABLE, this->gl_resizable);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, this->gl_opengl_forward_compat);

    if(this->fullscreen)
        this->window_monitor = glfwGetPrimaryMonitor();

    this->window = glfwCreateWindow(this->window_width,
                                    this->window_height,
                                    this->window_name.c_str(),
                                    this->window_monitor,
                                    this->window_share
    );
    glfwMakeContextCurrent(this->window);

    glfwSetKeyCallback(this->window, InputHandler::key_callback);
    //glfwSetCursorPosCallback(window, this->mouse_callback);
    //glfwSetScrollCallback(window, this->scroll_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::printf("\nERROR in Core::init_gl(void): could not initialize GLEW!\nTerminating program...\n");
        std::exit(1);
    }

    glViewport(this->viewport_x, this->viewport_y, this->viewport_w, this->viewport_h);

    glEnable(GL_DEPTH_TEST);
}

void Core::init_shaders(void)
{
    this->mandelbrot_shader = new Shader(this->mandelbrot_vertex_shader_path.c_str(),
                                         this->mandelbrot_fragment_shader_path.c_str());

    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    GLfloat temp[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,

        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    this->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 12);
    memcpy(this->vertices, temp, sizeof(GLfloat) * 12);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, this->vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

/*void Core::init_screenshot(void)
{
    glGenFrameBuffers(1, &this->framebufferID);
    glBindFrameBuffer(GL_FRAMEBUFFER, this->framebufferID);

    glGenTextures(1, &this->screenshotTexture);
    glBindTexture(GL_TEXTURE_2D, this->screenshotTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->window_width, this->window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->screenshotTexture, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Could not create screenshot framebuffer\n");
        

*/
#endif
