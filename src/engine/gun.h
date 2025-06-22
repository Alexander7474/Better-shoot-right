#pragma once 

#include "../../Bbop-2D/include/BBOP/Graphics.h"
#include "bullet.h"
#include "member.h"

#include <string>

enum GunState 
{
  gun_idle_state,
  shoot_state,
  reload_state
};

#ifndef DIRECTION 
#define DIRECTION

enum Direction
{
  rightDir,
  leftDir
};

#endif

//structure de stockage d'un animation, respecte la structure des fichier json
struct GunAnim 
{
  std::vector<Texture> textures; //<! ensemble de texture qui forma l'animation 
  double duration; // temps de l'animation
  int nFrame; // nombre de frame 
  double startTime; // depart de l'anim
  double lastFrameStartTime; // depart de la dernière frame 
  double frameTime;
};

class Gun : public Sprite
{
private:
  std::string name;

  GunAnim animations[15];
  GunState state;

  Vector2f attachPoint;
  Direction gunDirection;
  int animCnt;

  //ressource
  //irrklang::ISoundSource* gunShotSound; // bruit de tir du gun 
  Texture *bulletTexture; // Pointeur vers la texture des balles pour la charger une seule fois 

  //gestion des tirs
  float damage; // degat de l'arme
  bool isArmed; // l'arme est armé ?
  int magazineSize; // taille du chargeur 
  int ammo; // nombre de mun dans le chargeur
  double lastShotTime; // moment du dernier tire
  double rearmTime; // temps pour réarmer l'arme
  std::vector<Bullet> bulletVector; // stock le balle de l'arme en cours "d'utilisation"
  float bulletSpeed; //vitesse des balles
  Vector2f gunMouth; // sortie des balles 
  //
  friend class GameCharacter;
  
public:
  Gun();
  ~Gun();

  void update();

  void setAttachPoint(Vector2f ap);
  void setAttachPoint(float x, float y);

  /**
  * @brief fais tirer l'arme 
  */
  void shoot();

  /**
  * remplie le nombre de munition dans le chargeur
  */
  void reload();

  /**
  * @brief charge une arme depuis un fichier json
  */
  void loadJsonFile(std::string path);

  //GETTER 
  //
  const std::vector <Bullet>& getBullets() const;

  float getDamage() const;

};
