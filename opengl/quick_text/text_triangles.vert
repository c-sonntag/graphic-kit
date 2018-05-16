#version 330 core

// Input vertex data, different for all executions of this shader.
layout( location = 0 ) in vec2 vertex_from_buffer;
layout( location = 1 ) in vec2 uv_from_buffer;

//
uniform mat4 mvp_matrix;
uniform vec3 text_position;
uniform vec3 text_angle;

// Output data ; will be interpolated for each fragment.
out vec2 text_uv;

void main()
{
  //
  text_uv = vertex_from_buffer;

  // Output position of the vertex, in clip space
  // map [0..800][0..600] to [-1..1][-1..1]
  // [0..800][0..600] -> [-400..400][-300..300]
  vec2 vertexPosition_homoneneousspace = vertex_from_buffer - vec2( 400, 300 );
  vertexPosition_homoneneousspace /= vec2( 400, 300 );
  gl_Position =  mvp_matrix * vec4( vertexPosition_homoneneousspace, 0, 1 );
}

