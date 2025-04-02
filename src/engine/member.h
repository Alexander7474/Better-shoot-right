#pragma once 

#include "../../Bbop-Library/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-Library/include/BBOP/Graphics.h"

#include <string>


enum MemberState
{
  idle,
  run,
  dead
};

//structure de stockage d'un animation, respecte la structure des fichier json
struct MemberAnim 
{
  std::vector<Texture> textures; //<! ensemble de texture qui forma l'animation 
  double duration; // temps de l'animation
  int nFrame; // nombre de frame 
  double startTime; // depart de l'anim
  double lastFrameStartTime; // depart de la dernière frame 
  double frameTime;
};

class Member : public Sprite 
{
private:
  //physique
  Vector2f attachPoint;
  float force;
  
  //info
  std::string name;
  
  //anim
  MemberAnim animations[15];
  MemberState state;
  int animCnt;
  bool isReverse;

  friend class GameCharacter;
public:
  Member();

  void update();

  void setAttachPoint(Vector2f _attachPoint);
  void setAttachPoint(float x, float y);

  Vector2f getAttachPoint() const;

  void createTextureCache(std::string path);
};
