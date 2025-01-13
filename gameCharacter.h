#pragma once 

#include <string>
#include <vector>

#include "Bbop-Library/include/BBOP/Graphics.h"
#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "Bbop-Library/include/BBOP/Graphics/textureClass.h"
#include "member.h"


enum GameCharacterState
{
  idle,
  run
};

extern std::string gameCharacterStateString[2];

enum GameCharacterDirection
{
  rightDir,
  leftDir
};

class GameCharacter : public BbopDrawable, public Geometric
{
private:
  // attribute for the body of the character 
  Member legs;
  Member rightArm;
  Member leftArm;
  Member body;
  Member head;

  //gestion du regard et de l'orientation du character
  Vector2f lookingPoint; //<! Ou le character regarde
  GameCharacterDirection characterDirection; //<! Direction du regard
  
  //gestion de la physique
  float speed;
  float jumpForce;
  float weight;
  Vector2f position;
  Vector2f inertie;
  float forceInertie;
  double startFall;
  bool canJump;
  double startJump;
  double jumpTime;
  bool isJumping;

  //gestion de l'etat et des animations
  int animCnt;
  GameCharacterState state;

  /**
   *  @brief Détermine la rotaion d'un membre en fonction du lookingPoint
   */ 
  void setMemberRotation(Sprite &member);
  void setMemberRotation(Sprite &member, float m);

public:
  GameCharacter();

  /**
   * @brief remplie les listes de textures 
   */
  void createTextureCache(std::string path);
  
  /**
   * @brief Met jour le character
   */
  void update(Map* map);

  /**
  * @brief Draw the character
  */
  virtual void Draw(GLint renderModeLoc) const override;

  /**
  * @brief change la postition du character 
  * @details Donc toutes les position des sprites constituant le character avec le centre du torse comment référence/origine
  */
  void setPos(Vector2f pos);
  void setPos(float x, float y);

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

  //GETTER 
  //
  Member& getLeftArm();
  Member& getRightArm();
  Member& getBody();
  Member& getHead();
  Member& getLegs();
};
