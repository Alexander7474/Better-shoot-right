#include "member.h"
#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/textureClass.h"

#include <GLFW/glfw3.h>
#include <exception>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

Member::Member()
  : AnimationComponent<MemberState>(this), attachPoint(Vector2f(0.f, 0.f)),
    force(1.f) {
  state = MemberState::idle;
}

void Member::update()
{
  //play animation 
  //
  if (state!=MemberState::dead) {
    play(state);
  }
}

void Member::setAttachPoint(Vector2f _attachPoint)
{
  attachPoint = _attachPoint;
  setPosition(attachPoint);
}

void Member::setAttachPoint(float x, float y)
{
  setAttachPoint(Vector2f(x,y));
}

Vector2f Member::getAttachPoint() const
{
  return attachPoint;
}

void Member::createTextureCache(std::string path)
{
  for (int i = 0; i < static_cast<int>(MemberState::dead); ++i) {
    MemberState state = static_cast<MemberState>(i);
    loadTextureCache(state, path);
  }
}

void Member::setetat(int k){
  if (k==0)
  {
    state = MemberState::idle;
  }else if (k==1)
  {
    state = MemberState::run;
  }else if (k==2)
  {
    state = MemberState::dead;
  }else{
    cerr<<"state do not exist"<<endl;
  }
  
}

int Member::getetat(){
  return static_cast<int>(state);
}
