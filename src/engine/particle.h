#pragma once 

#include "../../Bbop-2D/include/BBOP/Graphics.h"

#include <unordered_map>

#include <memory>

class ParticleFactory final {
	private:
		static bool initialized;

		static std::unordered_map<std::string, std::unique_ptr<AnimatedSprite>> particles;

	public:
		/**
		 * @brief Charge toutes le particules du fichier particles.json dans assets/particle/
		 */
		static void loadAllParticles();

		/**
		 * @brief Donne une copie de la particule demandé
		 *
		 * @param name Nom de la particule 
		 * @return pointeur vers une copie de la particule 
		 */
		static AnimatedSprite *getParticle(std::string name);

};
