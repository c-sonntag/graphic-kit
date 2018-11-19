#version 330 core

//
uniform float uniform_time = 1.f;

//
in vec2 uv;
out vec4 color;

/**
 * @see https://www.shadertoy.com/new
 */
void main()
{
  // Time varying pixel color
  vec3 col = 0.5 + 0.5 * cos( uniform_time + uv.xyx + vec3( 0, 2, 4 ) );

  // Output to screen
  color = vec4( col, 1.0 );
};
