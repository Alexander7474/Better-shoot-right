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
  virtual void updatePhysic() = 0;
};
