#pragma once 

#include "../../Bbop-Library/include/BBOP/Graphics.h"
#include "bullet.h"
#include "member.h"

#include <irrKlang/ik_ISoundSource.h>
#include <irrKlang/irrKlang.h>
#include <string>

enum GunState 
{
  gun_idle_state,
  shoot_state,
  reload_state
};


class Gun : public Sprite
{
private:
  std::string name;

  MemberAnim animations[15];

  Vector2f attachPoint;

  //ressource
  irrklang::ISoundSource* gunShotSound; // bruit de tir du gun 
  Texture *bulletTexture; // Pointeur vers la texture des balles pour la charger une seule fois 

  //gestion des tirs
  float damage; // degat de l'arme
  bool isArmed; // l'arme est armé ?
  int magazineSize; // taille du chargeur 
  int ammo; // nombre de mun dans le chargeur
  double lastShotTime; // moment du dernier tire
  double rearmTime; // temps pour réarmer l'arme
  std::vector<Bullet> bulletVector; // stock le balle de l'arme en cours "d'utilisation"

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
};
