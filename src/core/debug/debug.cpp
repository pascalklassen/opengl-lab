#include "Debug.h"

#include <gl/glew.h>

#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ; // do nothing
}

bool GLLogCall(const char* stmt, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): "
            << stmt << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
