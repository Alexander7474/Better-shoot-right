#include "game.h"
#include "../engine/dynamicSprite.h"
#include "../engine/macro.h"
#include "../engine/physic.h"
#include "../engine/particle.h"

#include <box2d/box2d.h>
#include <memory>
#include <string>

GLFWwindow *gameWindow = nullptr;

double DELTA_TIME = 0;
double FPS = 0;
double FPS_COUNTER = 0;
double LAST_FPS_UPDATE = glfwGetTime();
float GRAVITY = 9.8f;
default_random_engine RANDOM_ENGINE;

Game::Game()
    : mainPlayer(this),
      map("assets/map/default/"),
      physicalWorld(b2Vec2(
          0.0f,
          GRAVITY)) // création du monde physique avec un vecteur de gravité
{
        auto listener = new CustomContactListener();
	listener->setGameOwner(this);
        physicalWorld.SetContactListener(listener);

        if (map.getSpawnPoints().size() > 1) {
                mainPlayer.getCharacter().setPosition(map.getSpawnPoints()[0]);
		testPnj.setPosition(mainPlayer.getCharacter().getPosition());
        }

	// init
        // physic-------------------------------------------------------------------------
        // rajoute le boite de collision au monde physique
        for (CollisionBox &box : map.getCollision()) {
                addStaticBox(&physicalWorld, &box);
        }

        entities.push_back(&mainPlayer.getCharacter());
	entities.push_back(&testPnj);

        // compute entities
        unsigned long long cptEnt = 0;
        for (const auto &e : entities) {
                e->computePhysic(&physicalWorld);
                cptEnt++;
        }
        const string log =
            to_string(cptEnt) +
            " entitées initialisées dans le monde box2d pas Game";
        DEBUG_MESSAGE(log);
        //------------------------------------------------------------------------------------
}

void Game::update() {
	for(const auto &item : items)
		item->update();

	for(unsigned long i = 0; i < particles.size(); i++){
 		if(particles[i]->update()){
			particlesTempShit.push_back(std::move(particles[i])); // TODO -- patch temp AnimatedSPrite / see addParticle todo 
			particles.erase(particles.begin()+i);
		}
	}

        // update des éléments des la game 
        map.update();

	// Gestion de la caméra ------------------------------------------------------------
        // déterminer la position du milieu entre le joueur et son crossair
        Vector2f middlePos;
        middlePos.x = (mainPlayer.getCharacter().getPosition().x +
                       mainPlayer.getCrossair().getPosition().x) /
                      2.f;
        middlePos.y = (mainPlayer.getCharacter().getPosition().y +
                       mainPlayer.getCrossair().getPosition().y) /
                      2.f;

        float distance =
            bbopGetDistance(middlePos, mainPlayer.getCharacter().getPosition());

        // limite la distance à la quelle la caméra peut aller
        if (distance > 150.f) {
                const double dx =
                    middlePos.x - mainPlayer.getCharacter().getPosition().x;
                const double dy =
                    middlePos.y - mainPlayer.getCharacter().getPosition().y;
                const double scale = 150 / distance;
                middlePos.x =
                    (mainPlayer.getCharacter().getPosition().x + scale * dx);
                middlePos.y =
                    (mainPlayer.getCharacter().getPosition().y + scale * dy);
        }

        mainPlayerCam.setScale(0.5f);
        mainPlayerCam.setPosition(middlePos);
        mainPlayer.update(&mainPlayerCam, &map);

	testPnj.update(&map);

        // Gestion de la
        // physique-------------------------------------------------------------------------
        constexpr int velocityIterations = 6;
        constexpr int positionIterations = 2;

        // mis a jour du monde box2d
        physicalWorld.Step(DELTA_TIME, velocityIterations, positionIterations);

        // mise a jour des entitées après la mise a jour du monde box2d
        for (auto &ent : entities) {
                ent->updatePhysic();
        }
        //----------------------------------------------------------------------------------
}

void Game::Draw() {
        map.Draw(scene, mainPlayerCam);
        scene.Draw(mainPlayer);
	scene.Draw(testPnj);

        for (auto &d : dynamics) {
                scene.Draw(*d);
#ifdef DEBUG_COLLISION
                bbopDebugCollisionBox(d->getCollisionBox(), scene);
#endif
        }

        for (auto &i : items) {
                scene.Draw(*i);
#ifdef DEBUG_COLLISION
                bbopDebugCollisionBox(i->getCollisionBox(), scene);
#endif
        }

#ifdef DEBUG_COLLISION
        bbopDebugCollisionBox(
            mainPlayer.getCharacter().getHead().getCollisionBox(), scene);
        bbopDebugCollisionBox(
            mainPlayer.getCharacter().getLegs().getCollisionBox(), scene);
        bbopDebugCollisionBox(
            mainPlayer.getCharacter().getBody().getCollisionBox(), scene);
        bbopDebugCollisionBox(
            mainPlayer.getCharacter().getRightArm().getCollisionBox(), scene);
        bbopDebugCollisionBox(
            mainPlayer.getCharacter().getLeftArm().getCollisionBox(), scene);

        for (CollisionBox &box : map.getCollision()) {
                bbopDebugCollisionBox(box, scene);
        }
#endif

	for(auto &p : particles)
		scene.Draw(*p);

        scene.render();
}

void Game::addItem(Item *item) {
        item->update();
        items.push_back(std::unique_ptr<Item>(item));
        entities.push_back(items.back().get());
}

// TODO -- URGENT gérer de bug lors de la destruction d'un ANimatedSprite qui empêche l'utilisation de textureColorBuffer de scene
// TODO -- Modifier la class AnimatedSprite pour reset le départ de l'animation sans accéder à ds membres sensé êtres privés
void Game::addParticle(AnimatedSprite *p) {
	particles.push_back(std::unique_ptr<AnimatedSprite>(p));
}


b2World *Game::getPhysicalWorld() { return &physicalWorld; }
