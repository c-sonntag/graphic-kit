#pragma once

#include <glm/mat4x4.hpp>

#include <gk/matrices/projection.hpp>

#include <gk/matrices/view_lookat.hpp>
#include <gk/matrices/view_subjective.hpp>

#include <gk/matrices/model.hpp>

#include <gk/matrices/composed_matrices.hpp>
#include <gk/matrices/base_camera.hpp>

#include <ostream>

//inline void dump( std::ostream & os, const glm::mat4x4 & m )
//{
//  os << " [" << std::endl
//     << "  [" << m[0][0] << "," << m[0][1] << "," << m[0][2] << "," << m[0][3] << "]" << std::endl
//     << "  [" << m[1][0] << "," << m[1][1] << "," << m[1][2] << "," << m[1][3] << "]" << std::endl
//     << "  [" << m[2][0] << "," << m[2][1] << "," << m[2][2] << "," << m[2][3] << "]" << std::endl
//     << "  [" << m[3][0] << "," << m[3][1] << "," << m[3][2] << "," << m[3][3] << "]" << std::endl
//     << " ]" << std::endl;
//}
