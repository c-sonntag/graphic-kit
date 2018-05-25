#include <graphic_toolkit/opengl/quick_text_expander.h>

#include <stdexcept>

namespace graphic_toolkit {
  namespace opengl {


    text_expander::text_expander( quick_text & _qt ) :
      qt( _qt )
    {}
    text_expander::text_expander( quick_text & _qt, std::string t ) :
      text( std::move( t ) ), qt( _qt )
    {}

    text_expander::~text_expander()
    {
      qt.lock();
      push_text();
      qt.unlock();
    }

    // ---- ----

    float text_expander::get_width() const
    {
      const quick_text_bbf_wrapper * const bbf( qt.get_font() );
      if ( bbf )
        return text.size() * bbf->font.cell_width * size;
      else
        return 0.f;
    }

    float text_expander::get_height() const
    {
      return size;
    }

    // ---- ----

    void text_expander::push_text()
    {
      const quick_text_bbf_wrapper * const bbf( qt.get_font() );
      if ( !bbf )
        return;


      /**<
       * @todo add return line parser
       * @todo add align_v
       */

      const uint t_length( text.size() );

      //
      const uint nb_carac_per_column(  bbf->font.height /  bbf->font.cell_height );
      const uint nb_carac_per_line(  bbf->font.width /  bbf->font.cell_width );

      //
      const float nb_carac_per_line_f( nb_carac_per_line ) ;
      const float nb_carac_per_column_f( nb_carac_per_column ) ;

      //
      const float cell_width(  bbf->font.cell_width );
      const float cell_height(  bbf->font.cell_height );

      //
      //const float cell_width_normal(  bbf->font.coef_width / cell_width );
      //const float cell_height_normal(  bbf->font.coef_height / cell_height );
      const float cell_width_normal( cell_width );
      const float cell_height_normal( cell_height );

      //
      const float half_size( size / 2.f );

      //
      quick_text::text_heap_t::vertex_expander triangles( qt.text_heap.complete_primitive( primitive_type::triangles ) );

      //
      QMatrix4x4 text_model;
      text_model.setToIdentity();
      text_model.translate( pos );
      text_model.rotate( degree_angle_3d.x(), 1.f, 0, 0 );
      text_model.rotate( degree_angle_3d.y(), 0, 1.f, 0 );
      text_model.rotate( degree_angle_3d.z(), 0, 0, 1.f );

      //
      triangles.set_uniform( "text_model", text_model );
      triangles.set_uniform( "text_color", color );

      //
      const float position_decal(
        ( align_h == text_expander::left ) ?
        0.f :
        ( ( align_h == text_expander::center ) ?
          -( float( t_length ) / 2.f ) :
          -float( t_length )
        )
      );

      //
      for ( uint i = 0 ; i < t_length ; ++i )
      {
        const float column( ( float( i ) + position_decal ) *  coef_extra_spacement );

        const QVector2D vertex_up_left( column * size, half_size );
        const QVector2D vertex_up_right( column * size + size, half_size );
        const QVector2D vertex_down_right( column * size + size, -half_size );
        const QVector2D vertex_down_left( column * size, -half_size );

        const uint character_base( static_cast<uint>( text[i] ) );
        if ( ( character_base <  bbf->font.start_char ) || ( character_base >  bbf->font.end_char ) )
          continue;

        //
        const uint character( character_base -  bbf->font.start_char );

        //
        const float character_mod( character % nb_carac_per_line );
        const float character_div( character / nb_carac_per_line );

        //
        const float uv_x( character_mod / nb_carac_per_line_f );
        const float uv_y( character_div / nb_carac_per_column_f );

        //
        const QVector2D uv_up_left( uv_x, uv_y );
        const QVector2D uv_up_right( uv_x + cell_width_normal, uv_y );
        const QVector2D uv_down_right( uv_x + cell_width_normal, uv_y + cell_height_normal );
        const QVector2D uv_down_left( uv_x, uv_y + cell_height_normal );

        triangles.push( vertex_up_left, uv_up_left );
        triangles.push( vertex_down_left, uv_down_left );
        triangles.push( vertex_up_right, uv_up_right );

        triangles.push( vertex_down_right, uv_down_right );
        triangles.push( vertex_up_right, uv_up_right );
        triangles.push( vertex_down_left, uv_down_left );
      }
    }

  }
}
