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
                ERROR_MESSAGE("Impossible d'ouvrir les particules " + jsonPath);
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

	try {
		for (const auto& [name, config] : jsonData.items()) {
			const std::string sheet = config["sheet"];
			int column = config["column"];
			int row = config["row"];
			int frame = config["frame"];
			const float frame_time = config["frame_time"];
			
			// Creation du Sprite animé
			particles[name] = std::make_unique<AnimatedSprite>(sheet, Vector2i(row,column), frame_time, row*column-frame, false);
		}
	}catch (const std::exception &e) {
		ERROR_MESSAGE("Récupération particule dans " + jsonPath);
		return;
	}

        initialized = true;

}

AnimatedSprite* ParticleFactory::getParticle(std::string name){
#ifdef PARTICLE_DEBUG
	DEBUG_MESSAGE("Appelle ParticleFactory::getParticle " + name);
#endif 
	if(!initialized){
		ERROR_MESSAGE("ParticleFactorty::getParticle appellé main ParticleFactory non initializé");
		return nullptr;
	}

	return particles[name].get();
}
