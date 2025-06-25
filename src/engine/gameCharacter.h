#pragma once 

#include <string>
#include <vector>

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/textureClass.h"
#include "member.h"
#include "gun.h"
#include "../game/entity.h"

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

class GameCharacter : public BbopDrawable, public Geometric, public Entity
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

  //taille du caractère
  float scale;
  
  //gestion de la physique
  float maxVelocityX;
  float maxVelocityY;
  float hp;

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
  void createTextureCache(const std::string& path);
  
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
  void setPos(const Vector2f &pos);
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
  float getSpeed() const;
  float getJumpForce() const;
  float getWeight() const;
  float getHp() const;
  

  //SETTER
  //
  void setSpeed(float _speed);
  void setJumpForce(float _jumpForce);
  void setWeight(float _weight);
  void setHp(float _hp);

  //ENTITY HERITAGE gestion de la physique
  void computePhysic(b2World *world) override;
  void updatePhysic() override;
};
