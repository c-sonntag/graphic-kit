#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

struct ModelViewProjection
{
  glm::mat4 Projection, View, Model;
  float fov = 45.0f;

  ModelViewProjection() {
    projectionInit();
    viewInit();
    modelInit();
    mvpRefresh();
  }

  inline void projectionInit() {
    Projection = glm::perspective(
                   fov,       // Le champ de vision horizontal, en degrés : la valeur de « zoom ». Pensez « lentille de caméra ». Habituellement entre 90° (très large) et 30° (assez zoomé)
                   4.0f / 3.0f, // Ratio d'aspect. Dépend de la taille de votre fenêtre. Notez que 4/3 == 800/600 == 1280/960, cela vous dit quelque chose ?
                   0.1f,        // Plan de découpe proche. Gardez-la la plus grande possible, ou vous allez avoir des problèmes de précision.
                   100.0f       // Plan de découpe lointain. Gardez-la aussi petite que possible.
                 );
  }

  inline void viewInit() {
    // Matrice de la caméra
    View = glm::lookAt(
             glm::vec3( 0, 0, 3 ), // La caméra est à ..., dans l'espace monde
             glm::vec3( 0, 0, 0 ), // et regarde l'origine
             glm::vec3( 0, 1, 0 ) // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers)
           );
  }

  inline void modelInit() {
    // Matrice de modèle : une matrice d'identité (le modèle sera à l'origine)
    Model = glm::mat4( 1.0f ); // Changez pour chaque modèle !
  }

 public:

  inline const glm::mat4 & mvpRefresh() {
    m_mvp = Projection * View * Model;
    return m_mvp;
  }

  inline const glm::mat4 & mvp() {
    return m_mvp;
  }

 private:
  glm::mat4 m_mvp;

};


struct ModelViewProjectionBis
{
  glm::mat4 Projection, View, Model;
  float fov = 45.0f;

  ModelViewProjectionBis() {

    //
    projectionRefresh();
    viewRefresh();
    modelRefresh();

    //
    mvpRefresh();

  }

  /*
  void ModelViewProjection::projectionRefresh()
  {
    Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
  }
  /*/
  void projectionRefresh() {
    Projection = glm::perspective(
                   fov,       // Le champ de vision horizontal, en degrés : la valeur de « zoom ». Pensez « lentille de caméra ». Habituellement entre 90° (très large) et 30° (assez zoomé)
                   4.0f / 3.0f, // Ratio d'aspect. Dépend de la taille de votre fenêtre. Notez que 4/3 == 800/600 == 1280/960, cela vous dit quelque chose ?
                   0.1f,        // Plan de découpe proche. Gardez-la la plus grande possible, ou vous allez avoir des problèmes de précision.
                   100.0f       // Plan de découpe lointain. Gardez-la aussi petite que possible.
                 );
  }
  // */

  /*
  void viewRefresh()
  {
    // Matrice de la caméra
    View = glm::lookAt(
             glm::vec3( 4, 3, 3 ), // La caméra est à (4,3,3), dans l'espace monde
             glm::vec3( 0, 0, 0 ), // et regarde l'origine
             glm::vec3( 0, 1, 0 ) // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers)
           );
  }
  /*/
  void viewRefresh() {
    // Matrice de la caméra
    View = glm::lookAt(
             glm::vec3( 0, 0, 3 ), // La caméra est à ..., dans l'espace monde
             glm::vec3( 0, 0, 0 ), // et regarde l'origine
             glm::vec3( 0, 1, 0 ) // La tête est vers le haut (utilisez 0,-1,0 pour regarder à l'envers)
           );
  }
  //*/

  void modelRefresh() {
    // Matrice de modèle : une matrice d'identité (le modèle sera à l'origine)
    Model = glm::mat4( 1.0f ); // Changez pour chaque modèle !
  }


  const glm::mat4 & mvpRefresh() {
    // Notre matrice ModelViewProjection : la multiplication des trois  matrices
    //   Souvenez-vous, la multiplication de matrice fonctionne dans l'autre sens
    MVP = Projection * View * Model;

    return MVP;
  }

 private:
  glm::mat4 MVP;

};

