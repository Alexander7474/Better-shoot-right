#pragma once 

#include <string>
#include <vector>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/textureClass.h"
#include "member.h"
#include "gun.h"

extern std::string gameCharacterStateString[2];

//éviter le redéfinition 
#ifndef DIRECTION 
#define DIRECTION

enum Direction
{
  rightDir,
  leftDir
};

#endif

class GameCharacter : public BbopDrawable, public Geometric
{
private:
  // attribute for the body of the character 
  Member legs;
  Member rightArm;
  Member leftArm;
  Member body;
  Member head;

  //objet du character 
  Gun gun;

  //gestion du regard et de l'orientation du character
  Vector2f lookingPoint; //<! Ou le character regarde
  Direction characterDirection; //<! Direction du regard
  //
  float scale;
  
  //gestion de la physique
  float speed;
  float jumpForce;
  float weight;
  float hp;

  // gérer par le caractère
  Vector2f inertie;
  float forceInertie;
  double startFall;
  bool canJump;
  double startJump;
  double jumpTime;
  bool isJumping;

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
  void Draw(GLint *renderUniforms) const override;

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
  Gun& getGun();
  float getSpeed();
  float getJumpForce();
  float getWeight();
  float gethp();
  

  //SETTER
  //
  void setSpeed(float _speed);
  void setJumpForce(float _jumpForce);
  void setWeight(float _weight);
  void sethp(float hp);
};
