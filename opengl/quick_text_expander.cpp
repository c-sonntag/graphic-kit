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

    float text_expander::get_base_width() const
    {
      //
      const quick_text_bbf_wrapper * const bbf_p( qt.get_font() );
      if ( !bbf_p ) return 0.f;
      const quick_text_bbf_wrapper & bbf( *bbf_p );

      //
      float width( 0.f );

      //
      const size_t length( text.size() );
      for ( uint i( 0 ); i < length; ++i )
      {
        const uint character_base( static_cast<uint>( text[i] ) );
        if ( ( character_base <  bbf.font.start_char ) || ( character_base > bbf.font.end_char ) )
          continue;

        //
        width += float ( bbf.font.characters_width[character_base] );
      }

      //
      return width;
    }

    float text_expander::get_base_height() const
    {
      const quick_text_bbf_wrapper * const bbf_p( qt.get_font() );
      if ( !bbf_p ) return 0.f;
      //
      return bbf_p->font.cell_height;
    }

    // ---- ---- ---- ----

    //float text_expander::get_width() const
    //{
    //  return get_base_width() * ( size / text_size_normal_base );
    //}
    //
    //float text_expander::get_height() const
    //{
    //  return get_base_height() * ( size / text_size_normal_base );
    //}

    // ---- ---- ---- ----

    float text_expander::get_normal_width() const
    {
      const quick_text_bbf_wrapper * const bbf_p( qt.get_font() );
      if ( !bbf_p ) return 0.f;
      //
      return get_base_width() * bbf_p->font.normal_scale.x();
    }

    float text_expander::get_normal_height() const
    {
      const quick_text_bbf_wrapper * const bbf_p( qt.get_font() );
      if ( !bbf_p ) return 0.f;
      //
      return get_base_height() * bbf_p->font.normal_scale.y();
    }

    // ---- ----

    float text_expander::get_normal_width_for_current_size() const
    {
      return get_normal_width() * normal_size;
    }

    float text_expander::get_normal_height_for_current_size() const
    {
      return get_normal_height() * normal_size;
    }

    // ---- ----

    /**
     * @see https://github.com/CodeheadUK/CBFG/blob/master/examples/BFF%20Example/BitmapFontClass.cpp#L256
     */
    void text_expander::push_text()
    {
      //
      const quick_text_bbf_wrapper * const bbf_p( qt.get_font() );
      if ( !bbf_p ) return;
      const quick_text_bbf_wrapper & bbf( *bbf_p );

      //
      quick_text::text_heap_t::vertex_expander triangles( qt.text_heap.complete_primitive( primitive_type::triangles ) );

      /** @todo implement return line */

      //
      float pos_x( 0.f );
      float pos_y( 0.f );

      const size_t length( text.size() );
      for ( uint i( 0 ); i < length; ++i )
      {
        const uint character_base( static_cast<uint>( text[i] ) );
        if ( ( character_base <  bbf.font.start_char ) || ( character_base >  bbf.font.end_char ) )
          continue;

        //
        const uint character( character_base -  bbf.font.start_char );

        const int row( int( character ) / int( bbf.font.row_pitch ) );
        const int col( int( character ) - row * int( bbf.font.row_pitch ) );

        const float char_width( bbf.font.characters_width[character_base] );
        const float char_width_factor_cell_width( char_width / bbf.font.cell_width );

        const float U( col * bbf.font.col_factor );
        const float V( row * bbf.font.row_factor );
        const float U1( U +  bbf.font.col_factor * char_width_factor_cell_width );
        const float V1( V +  bbf.font.row_factor );

        const QVector2D vertex_up_left( pos_x, pos_y );
        const QVector2D vertex_up_right( pos_x + char_width, pos_y );
        const QVector2D vertex_down_right( pos_x + char_width, pos_y + bbf.font.cell_height );
        const QVector2D vertex_down_left( pos_x, pos_y + bbf.font.cell_height );

        const QVector2D uv_up_left( U, V1 );
        const QVector2D uv_up_right( U1, V1 );
        const QVector2D uv_down_right( U1, V );
        const QVector2D uv_down_left( U, V );

        triangles.push( vertex_up_left, uv_up_left );
        triangles.push( vertex_down_left, uv_down_left );
        triangles.push( vertex_up_right, uv_up_right );

        triangles.push( vertex_down_right, uv_down_right );
        triangles.push( vertex_up_right, uv_up_right );
        triangles.push( vertex_down_left, uv_down_left );

        pos_x += char_width;
      }

      const QVector2D position_decal(
        ( align_h == text_expander::left ) ?
        0.f :
        ( ( align_h == text_expander::center ) ?
          ( -pos_x / 2.f ) :
          -pos_x
        ),
        ( align_v == vertical_align::top ) ?
        0.f :
        ( ( align_v == vertical_align::middle ) ?
          ( -float( bbf.font.cell_height ) / 2.f ) :
          -float( bbf.font.cell_height )
        )
      );

      //
      QMatrix4x4 text_model;
      text_model.setToIdentity();
      text_model.translate( pos );
      text_model.rotate( degree_angle_3d.x(), 1.f, 0, 0 );
      text_model.rotate( degree_angle_3d.y(), 0, 1.f, 0 );
      text_model.rotate( degree_angle_3d.z(), 0, 0, 1.f );
      text_model.scale( normal_size );
      text_model.translate( position_decal * bbf.font.normal_scale );

      //
      triangles.set_uniform( "text_model", text_model );
      triangles.set_uniform( "text_color", color );
    }

  }
}
