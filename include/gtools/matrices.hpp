#pragma once

#include <glm/mat4x4.hpp>

#include <gtools/matrices/projection.hpp>

#include <gtools/matrices/view_lookat.hpp>
#include <gtools/matrices/view_subjective.hpp>

#include <gtools/matrices/model.hpp>

#include <gtools/matrices/composed_matrices.hpp>
#include <gtools/matrices/base_camera.hpp>

#include <ostream>

inline void dump( std::ostream & os, const glm::mat4x4 & m )
{
  os << " [" << std::endl
     << "  [" << m[0][0] << "," << m[0][1] << "," << m[0][2] << "," << m[0][3] << "]" << std::endl
     << "  [" << m[1][0] << "," << m[1][1] << "," << m[1][2] << "," << m[1][3] << "]" << std::endl
     << "  [" << m[2][0] << "," << m[2][1] << "," << m[2][2] << "," << m[2][3] << "]" << std::endl
     << "  [" << m[3][0] << "," << m[3][1] << "," << m[3][2] << "," << m[3][3] << "]" << std::endl
     << " ]" << std::endl;
}
