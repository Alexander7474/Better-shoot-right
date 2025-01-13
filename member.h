#pragma once 

#include "Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "Bbop-Library/include/BBOP/Graphics.h"

#include <string>

//structure de stockage d'un animation
struct MemberAnim 
{
  std::vector<Texture> textures; //<! ensemble de texture qui forma l'animation 
  double time; // temps de l'animation
};

class Member : public Sprite 
{
private:
  Vector2f attachPoint;
  float force;
  MemberAnim animations[15];
  std::string name;

  friend class GameCharacter;
public:
  Member();

  void update();

  void setAttachPoint(Vector2f _attachPoint);
  void setAttachPoint(float x, float y);

  Vector2f getAttachPoint() const;

  void createTextureCache(std::string path);
};
