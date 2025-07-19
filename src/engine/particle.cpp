#include "particle.h"

#include "macro.h"

#include <nlohmann/json.hpp>

bool ParticleFactory::initialized = false;
std::unordered_map<std::string, std::unique_ptr<AnimatedSprite>> ParticleFactory::particles;

void ParticleFactory::loadAllParticles(){
#ifdef PARTICLE_DEBUG
	DEBUG_MESSAGE("Appelle ParticleFactory::loadAllParticles");
#endif

	std::string jsonPath = "assets/particles/particles.json";
        std::ifstream jsonFile(jsonPath);
        if (!jsonFile.is_open()) {
                ERROR_MESSAGE("Impossible d'ouvrir les items " + jsonPath);
                return;
        }

        // loading with json
        nlohmann::json jsonData;
        try {
                jsonFile >> jsonData;
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Erreur parsing json pour " + jsonPath);
                return;
        }
        initialized = true;

}

AnimatedSprite* ParticleFactory::getParticle(std::string name){
#ifdef PARTICLE_DEBUG
	DEBUG_MESSAGE("Appelle ParticleFactory::getParticle " + name);
#endif
	
	return nullptr;
}
