#pragma once

#include <cstdint>

#include <string>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <functional>


struct AbstractPainter
{
 public:
  virtual void paint( GLFWwindow * window ) = 0;
  virtual ~AbstractPainter() = default;
};
