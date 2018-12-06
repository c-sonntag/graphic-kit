#version 330 core

//
uniform vec4 uniform_color = vec4( 1.f, 0.f, 0.f, 1.f );
uniform int uniform_texture_mode = 0;
uniform sampler2D uniform_sampler_texture2d;
// uniform sampler2D uniform_sampler_texture2d;
// uniform sampler2D uniform_sampler_texture2d;

//
in vec2 uv2d;
in vec3 norm;
out vec4 color;

/**
 * @brief   Object importer
 * @see gk::opengl::model::from_vectors
 */
void main()
{
  //  if( uniform_texture_mode == 1 )
  //    color = vec4( texture( uniform_sampler_texture2d, uv2d ).rgb, 1.f );
  //  else if( uniform_texture_mode == 2 )
  //    color = texture( uniform_sampler_texture2d, uv2d ).rgba;
  //  else if( uniform_texture_mode == 3 )
  //    color = vec4( texture( uniform_sampler_texture2d, uv2d ).r, 0.f, 0.f, 1.f );
  //  else
  //    color =  uniform_color;

  if( uniform_texture_mode == 0 )
  {
    //color = length( uv + 0.5f ) * uniform_color;
    color = smoothstep( 0.2, 1.f, 1.0 - length( uv  - 0.5 ) ) * uniform_color;
  }
  else
    color = vec4( texture( uniform_sampler_texture2d, uv2d ).rgb, 1.f );

}
