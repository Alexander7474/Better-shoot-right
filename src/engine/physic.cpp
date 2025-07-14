#include "physic.h"
#include "macro.h"
#include <box2d/b2_body.h>
#include <string>
#include <utility>

#include "gameCharacter.h"

b2Body *addStaticBox(b2World *world, const Geometric *box) {
        // 1. Définir le corps statique
        b2BodyDef bodyDef;
        bodyDef.position.Set(
            (box->getPosition().x + (box->getSize().x / 2)) / PIXEL_PER_METER,
            (box->getPosition().y + (box->getSize().y / 2)) / PIXEL_PER_METER);
        bodyDef.type = b2_staticBody;

        b2Body *body = world->CreateBody(&bodyDef);

        // 2. Définir la forme (boîte centrée)
        b2PolygonShape boxShape;
        boxShape.SetAsBox((box->getSize().x / 2) / PIXEL_PER_METER,
                          (box->getSize().y / 2) / PIXEL_PER_METER);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 0.f;
        fixtureDef.friction = 1.f;

#ifdef PHYSIC_DEBUG
        std::string log =
            "Box static ajouter l:" + std::to_string(box->getSize().x) +
            " h:" + std::to_string(box->getSize().y) +
            " x:" + std::to_string(box->getPosition().x) +
            " y:" + std::to_string(box->getPosition().y);
        log += " | BOX2D COORD x:" + std::to_string(body->GetPosition().x) +
               " y:" + std::to_string(body->GetPosition().y);
        DEBUG_MESSAGE(log);
#endif

        // 3. Ajouter la fixture au corps
        body->CreateFixture(&fixtureDef); // densité = 0 pour statique

	// Ajout d'un pointer vers BodyData Undefined 
	// pour eviter les erreurs dans handleContact
	auto *data = new BodyData;
        data->type = BodyType::Static;

	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
        
	return body;
}

b2Body *addDynamicBox(b2World *world, Geometric *box, const float restitution,
                      const float density, const float friction,
                      const float linearDamping, const bool rotationLock,
                      const Vector2f &offsetX, const Vector2f &offsetY) {

        // Définir les mesures de la boite de BBOP vers Box2D
        const Vector2f sizeNoOffset(
            (box->getSize().x - offsetX.x - offsetX.y) / 2.f,
            (box->getSize().y - offsetY.x - offsetY.y) / 2.f);

        const Vector2f posNoOffset(
            box->getPosition().x + box->getSize().x / 2.f + offsetX.x / 2.f,
            box->getPosition().y + box->getSize().y / 2.f + offsetY.x / 2.f);

        box->setOrigin(
            box->getSize().x / 2.f + offsetX.x / 2.f - offsetX.y / 2.f,
            box->getSize().y / 2.f + offsetY.x / 2.f - offsetY.y / 2.f);

        // 1. Définir le corps statique
        b2BodyDef bodyDef;
        bodyDef.position.Set(posNoOffset.x / PIXEL_PER_METER,
                             posNoOffset.y / PIXEL_PER_METER);
        bodyDef.type = b2_dynamicBody;
        bodyDef.fixedRotation = rotationLock;
        bodyDef.linearDamping = linearDamping; // Très utile !
        bodyDef.angle = box->getRotation();

        b2Body *body = world->CreateBody(&bodyDef);

        // 2. Définir la forme (boîte centrée)
        b2PolygonShape boxShape;
        boxShape.SetAsBox(sizeNoOffset.x / PIXEL_PER_METER,
                          sizeNoOffset.y / PIXEL_PER_METER);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;
        fixtureDef.friction = friction;

#ifdef PHYSIC_DEBUG
        std::string log =
            "Box dynamic ajouter l:" + std::to_string(box->getSize().x) +
            " h:" + std::to_string(box->getSize().y) +
            " x:" + std::to_string(box->getPosition().x) +
            " y:" + std::to_string(box->getPosition().y);
        log += " | BOX2D COORD x:" + std::to_string(body->GetPosition().x) +
               " y:" + std::to_string(body->GetPosition().y);
        DEBUG_MESSAGE(log);
#endif

        // 3. Ajouter la fixture au corps
        body->CreateFixture(&fixtureDef);

	// Ajout d'un pointer vers BodyData Undefined 
	// pour eviter les erreurs dans handleContact
	auto *data = new BodyData;
        data->type = BodyType::Undefined;
        
	body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
	
	return body;
}

void CustomContactListener::BeginContact(b2Contact *contact) {
        handleContact(contact, true);
}

void CustomContactListener::EndContact(b2Contact *contact) {
        handleContact(contact, false);
}

void CustomContactListener::handleContact(b2Contact *contact,
                                          const bool begin) {
#ifdef CONTACT_DEBUG
        DEBUG_MESSAGE("Appelle CustomContactListener::handleContact");
#endif

	b2Fixture *fixtureA = contact->GetFixtureA();
        b2Fixture *fixtureB = contact->GetFixtureB();
        b2Body *bodyA = fixtureA->GetBody();
        b2Body *bodyB = fixtureB->GetBody();

        b2WorldManifold manifold;
        contact->GetWorldManifold(&manifold);
        b2Vec2 normal = manifold.normal;

	if(!bodyB->GetUserData().pointer || !bodyA->GetUserData().pointer){
		ERROR_MESSAGE("Impossible de gérer un contact sans pointer dans UserData");
		return;
	}

	auto *dataA = reinterpret_cast<BodyData *>(bodyA->GetUserData().pointer);
	auto *dataB = reinterpret_cast<BodyData *>(bodyB->GetUserData().pointer);

	// Lambda de detection du type de collision 
	auto hasTypes = [](BodyData *a, BodyData *b, BodyType tA, BodyType tB){
		return (a->type == tA && b->type == tB);
	};

	// La gestion du contact peut-être fais
	// avec bodyA = GameCharacter et bodyB
	// = Static.
	// Si ces l'inverse, on échange bodyA et B.
	//
	// (On ne peut pas prédire dans quelle 
	// ordre box2D va renvoyer la collision)
	//
	
	// Gestion GameCharacter Static
	if (hasTypes(dataA, dataB, BodyType::Static, BodyType::GameCharacter)){
		std::swap(dataA, dataB);
		normal.y = -normal.y;
	}
	if (hasTypes(dataA, dataB, BodyType::GameCharacter, BodyType::Static)){
		if (normal.y > 0.2f) {
			auto* character = reinterpret_cast<GameCharacter*>(dataA->ptr);
			character->setTouchingDown(begin);
		}
		return;
	}

	// Gestion GameCharacter Bullet
	// TODO -- Ajouter la gestion
	
	// Gestion Bullet Static
	if(hasTypes(dataA, dataB, BodyType::Static, BodyType::Bullet)){
		std::swap(dataA, dataB);
	}
	if(hasTypes(dataA, dataB, BodyType::Bullet, BodyType::Static)){
		auto *bullet = reinterpret_cast<Bullet*>(dataA->ptr);

		if(bullet->getState() != BulletState::broken){
			bullet->broke();
		}
	}
}
