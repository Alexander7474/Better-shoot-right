#pragma once 

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>

/**
  * @brief Entité du monde 
  *
  * @details tous objets qui va intéragir dans le monde de manière physique doit hérité de cette class 
  */
class Entity
{
protected:
  b2Body *body; //<! pointeur vers le body Box2d pour l'update
public:

  /**
  * @brief créé l'objet physique dans le monde box2D
  * @details fais dans un second temps du constructeur pour permettre de modifier les caractèristiques du sprite
  */
  virtual void computePhysic(b2World *world) = 0;

  /**
  * @brief Met a jour la phisique dans le monde box2D
  */
  virtual void updatePhysic() = 0;
};
