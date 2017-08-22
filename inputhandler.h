#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>

class InputHandler
{
public:

    static void begin(void);

    //static void end();

    static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
    //static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
    //static void mouse_callback(GLFWwindow * window, double xpos, double ypos);

    inline static bool getKey(int key);

    //static void update(GLfloat dtime);

private:

    InputHandler() { }

    static bool keys[1024];

};

void InputHandler::begin(void)
{
    for(int i = 0; i < 1024; i++) {
        keys[i] = false;
    }
}

void InputHandler::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    switch(action)
    {
    case GLFW_PRESS:
        keys[key] = true;
        break;
    case GLFW_RELEASE:
        keys[key] = false;
        break;
    default:
        break;
    };
}

bool InputHandler::getKey(int key)
{
    return keys[key];
}

#endif
