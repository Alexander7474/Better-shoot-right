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
  std::vector<Texture> textures; //<! ensemble de texture qui form l'animation
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
  bool armed; // l'arme est armé ?
  int magazineSize; // taille du chargeur 
  int ammo; // nombre de mun dans le chargeur
  double lastShotTime; // moment du dernier tire
  double rearmTime; // temps pour réarmer l'arme
  std::vector<Bullet> bulletVector; // stock le balle de l'arme en cours "d'utilisation"
  float bulletSpeed; //vitesse des balles
  Vector2f gunMouth; // sortie des balles
  
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

  const std::string & getName() const;

  void setName(const std::string &name);

  GunState getState() const;

  void setState(GunState state);

  [[nodiscard]] Direction getGunDirection() const;

  void setGunDirection(Direction gunDirection);

  float getDamage() const;

  void setDamage(float damage);

  bool isArmed() const;

  void setArmed(bool armed);

  int getMagazineSize() const;

  void setMagazineSize(int magazineSize);

  int getAmmo() const;

  void setAmmo(int ammo);

  const std::vector<Bullet> & getBulletVector() const;

  void setBulletVector(const std::vector<Bullet> &bulletVector);
};
