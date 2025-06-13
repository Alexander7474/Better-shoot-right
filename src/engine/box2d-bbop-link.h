#pragma once 

// Contient les fonction de lien entre BBOP et box2D 

#include <box2d/box2d.h>
#include "../../Bbop-Library/include/BBOP/Graphics.h"

//Box 2D foncitonne en mètre donc il faut une constante pour savoir combien de pixel vaut un mètre 
#define PIXEL_PER_METER 32

/**
* @brief Créé une boite de collision Static dans un monde box2d à partir d'une collision box BBOP 
*
* @param world Pointeur vers le monde 
* @param box pointeur vers la boite à rajouter 
*
* @return body Corps box2d de la boite 
*/
b2Body* addStaticBox(b2World* world, CollisionBox* box);

/**
* @brief Créé une boite de collision Dynamic dans un monde box2d à partir d'une collision box BBOP 
*
* @param world Pointeur vers le monde 
* @param box pointeur vers la boite à rajouter 
*
* @return body Corps box2d de la boite 
*/
b2Body* addDynamicBox(b2World* world, CollisionBox* box);

