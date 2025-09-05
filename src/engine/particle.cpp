#include "particle.h"

#include "macro.h"

#include <nlohmann/json.hpp>

#include <GLFW/glfw3.h>

bool ParticleFactory::initialized = false;
std::unordered_map<std::string, std::unique_ptr<AnimatedSprite>>
    ParticleFactory::particles;

void ParticleFactory::loadAllParticles() {
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
                for (const auto &[name, config] : jsonData.items()) {
                        const std::string sheet = config["sheet"];
                        int column = config["column"];
                        int row = config["row"];
                        int frame = config["frame"];
                        const float frame_time = config["frame_time"];

                        // Creation du Sprite animé
                        particles[name] = std::make_unique<AnimatedSprite>(
                            sheet, Vector2i(row, column), frame_time,
                            row * column - frame, false);
                }
        } catch (const std::exception &e) {
                ERROR_MESSAGE("Récupération particule dans " + jsonPath);
                return;
        }

        initialized = true;
}

ParticleFactory::ParticleFactory(std::string name) {
        if (!initialized)
                return;
        particle = new AnimatedSprite(*particles[name].get());
        particle->anim_start = glfwGetTime();   // illegal
        particle->last_frame_t = glfwGetTime(); // go fuck yourself
}

ParticleFactory ParticleFactory::getParticle(std::string name) {
#ifdef PARTICLE_DEBUG
        DEBUG_MESSAGE("Appelle ParticleFactory::getParticle " + name);
#endif

        return ParticleFactory(name);
}

ParticleFactory &ParticleFactory::withSize(Vector2f size) {
        this->size = size;
        return *this;
}

ParticleFactory &ParticleFactory::withOrigin(Vector2f origin) {
        this->origin = origin;
        return *this;
}

ParticleFactory &ParticleFactory::withPosition(Vector2f position) {
        this->position = position;
        return *this;
}

ParticleFactory &ParticleFactory::withRotation(float rotation) {
        this->rotation = rotation;
        return *this;
}

ParticleFactory &ParticleFactory::withVerticalFlip() {
        verticalFlip = true;
        return *this;
}

ParticleFactory &ParticleFactory::withHorizontalFlip() {
        horizontalFlip = true;
        return *this;
}

AnimatedSprite *ParticleFactory::build() {
        if (!initialized) {
                ERROR_MESSAGE("ParticleFactorty::build appellé main "
                              "ParticleFactory non initializé");
                return nullptr;
        }

        particle->setSize(size);
        particle->setOrigin(origin);
        particle->setPosition(position);
        particle->setRotation(rotation);

        if (verticalFlip)
                particle->flipVertically();

        if (horizontalFlip)
                particle->flipHorizontally();

        return particle;
}
