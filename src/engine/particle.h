#pragma once 

#include "../../Bbop-2D/include/BBOP/Graphics.h"

#include <unordered_map>

#include <memory>

class ParticleFactory final {
	private:
		static bool initialized;

		static std::unordered_map<std::string, std::unique_ptr<AnimatedSprite>> particles;


		ParticleFactory(std::string name);

		AnimatedSprite *particle;
		Vector2f size;
		Vector2f origin;
		Vector2f position;
		float rotation;
		bool verticalFlip;
		bool horizontalFlip;
	public:
		/**
		 * @brief Charge toutes le particules du fichier particles.json dans assets/particle/
		 */
		static void loadAllParticles();

		/**
		 * @brief Donne une usine avec la particule demandé
		 *
		 * @param name Nom de la particule 
		 * @return Usine prette a setup la particle demandé
		 */
		static ParticleFactory getParticle(std::string name);

		ParticleFactory& withSize(Vector2f size);
		ParticleFactory& withOrigin(Vector2f origin);
		ParticleFactory& withPosition(Vector2f position);
		ParticleFactory& withRotation(float rotation);

		AnimatedSprite *build();

};
