#pragma once 

#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics.h"

#include <string>

#include "animationComponent.h"

enum class MemberState
{
  idle,
  run,
  ragdoll,
  dead
};

//structure de stockage d'un animation, respecte la structure des fichier json
struct MemberAnim 
{
  std::vector<Texture> textures; //<! ensemble de texture qui form l'animation
  double duration; // temps de l'animation
  int nFrame; // nombre de frame 
  double startTime; // depart de l'anim
  double lastFrameStartTime; // depart de la dernière frame 
  double frameTime;
};

class Member : public Sprite, public AnimationComponent<MemberState>
{
private:
  //physique
  Vector2f attachPoint;
  float force;
  
  //info
  std::string name;
  
  //anim
  MemberState state;

  friend class GameCharacter;
public:
  Member();

  void update();

  void setAttachPoint(Vector2f _attachPoint);
  void setAttachPoint(float x, float y);

  Vector2f getAttachPoint() const;

  void createTextureCache(std::string path);

  void setetat(int k);
  int getetat();
};
