#ifndef __GLFW_H_
#define __GLFW_H_

// This header includes all the OpenGL headers we need in the correct order.
// Include this header rather than including any GL/* or GLFW/* headers.

// Must include glew.h before glfw3.h, glew.h fails to compile if gl.h has
// been included before it and glfw3.h includes gl.h.
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#endif // __GLFW_H_