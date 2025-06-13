#pragma once 

#include <box2d/b2_body.h>
#include <box2d/box2d.h>

/**
  * @brief Entité du monde 
  *
  * @details tous objets qui va intéragir dans le monde de manière physique doit hérité de cette class 
  */
class Entity
{
private:
  b2Body *body; //<! pointeur vers le body Box2d pour l'update
public:
  virtual void updatePhysique() = 0;

  virtual ~Entity();
};
