#pragma once 

#include <string>
#include <vector>

#include "Bbop-Library/include/BBOP/Graphics.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "Bbop-Library/include/BBOP/Graphics/textureClass.h"

enum GameCharacterState
{
  idle,
  run
};

enum GameCharacterDirection
{
  rightDir,
  leftDir
};

//structure de stockage d'un animation
struct GameCharacterAnim 
{
  std::vector<Texture> textures; //<! ensemble de texture qui forma l'animation 
  double time; // temps de l'animation
};

class GameCharacter : public BbopDrawable, public Geometric
{
private:
  // attribute for the body of the character 
  Sprite legs;
  Sprite rightArm;
  Sprite leftArm;
  Sprite body;
  Sprite head;

  //Tableau de texture de chaque élément du character, chaque vector correspond à l'anim d'un état
  GameCharacterAnim legsTexture[15];
  GameCharacterAnim rightArmTexture[15];
  GameCharacterAnim leftArmTexture[15];
  GameCharacterAnim bodyTexture[15];
  GameCharacterAnim headTexture[15];

  //gestion du regard et de l'orientation du character
  Vector2f lookingPoint; //<! Ou le character regarde
  GameCharacterDirection characterDirection; //<! Direction du regard
  
  //gestion de la physique
  float speed;
  float jumpForce;
  float weight;
  Vector2f position;

  /**
   *  @brief Détermine la rotaion d'un membre en fonction du lookingPoint
   */ 
  void setMemberRotation(Sprite &member);

public:
  GameCharacter();

  /**
   * @brief remplie les listes de textures 
   */
  void createTextureCache(std::string path);
  
  /**
   * @brief Met jour le character
   */
  void update();

  /**
  * @brief Draw the character
  */
  virtual void Draw(GLint renderModeLoc) const override;

  /**
  * @brief change la postition du character 
  * @details Donc toutes les position des sprites constituant le character avec le centre du torse comment référence/origine
  */
  void setPos(Vector2f pos);

  /**
  * @brief renvoie l'endroite ou regarde le character
  */
  Vector2f getLookingPoint() const;
  
  /**
  * @brief Fait regarder le character au coordonnées lp
  */
  void lookAt(Vector2f lp);

  void goLeft();

  void goRight();

  void jump();

  /**
  * @brief retourne le character sur l'axe y
  */
  void flipY();
};
