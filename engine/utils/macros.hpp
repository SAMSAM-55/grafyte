#pragma once

#include "glad/glad.h"
#include <iostream>


#if defined(_MSC_VER)
  #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
  #define DEBUG_BREAK() __builtin_trap()
#else
  #define DEBUG_BREAK() std::abort()
#endif

#define ASSERT(x) do { if(!(x)) { DEBUG_BREAK(); } } while(0)
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ") : " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}