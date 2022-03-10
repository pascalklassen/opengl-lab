#pragma once

#define ASSERT(stmt) if (!(stmt)) __debugbreak();
#define GL_CHECK(stmt) GLClearError();\
    stmt;\
    ASSERT(GLLogCall(#stmt, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* stmt, const char* file, int line);
