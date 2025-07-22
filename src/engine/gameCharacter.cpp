#ifdef IMGUI_DEBUG
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <memory>

#include "../game/game.h"
#include "gameCharacter.h"
#include "gun.h"
#include "item.h"
#include "member.h"
#include "physic.h"

const char *gameCharacterStateString[4] = {"idle", "run", "ragdoll", "dead"};

GameCharacter::GameCharacter()
    : maxVelocityX(3.f), maxVelocityY(10.f), newtonX(8.f), newtonY(6.f), restitution(0.f), friction(1.f), density(1.5f),
      linearDamping(2.f), hp(10.f) {
        characterDirection = rightDir;
        scale = 0.65f;

        leftArm.setSize(32 * scale, 32 * scale);

        rightArm.setSize(32 * scale, 32 * scale);
        rightArm.name = "right arm";

        body.setSize(32 * scale, 32 * scale);
        body.setOrigin(16 * scale, 16 * scale); // origine au centre du torse
        body.name = "body";

        head.setSize(32 * scale, 32 * scale);
        head.setOrigin(16 * scale, 16 * scale); // origine au coup
        head.name = "head";

        legs.setSize(64 * scale, 32 * scale);
        legs.setOrigin(32 * scale, 0 * scale); // origine sur les hanche
        legs.name = "legs";

        const auto gunPtr = dynamic_cast<Gun *>(ItemFactory::getItem("ak47"));
        gun = std::make_unique<Gun>(*gunPtr);
        gun->setSize(64 * scale, 32 * scale);

        createTextureCache("assets/personnages/soldier/");
}

void GameCharacter::createTextureCache(const string &path) {
        const string armLeftPath = path + "arm_left/";
        leftArm.createTextureCache(armLeftPath);

        const string armRightPath = path + "arm_right/";
        rightArm.createTextureCache(armRightPath);

        const string bodyPath = path + "body/";
        body.createTextureCache(bodyPath);

        const string headPath = path + "head/";
        head.createTextureCache(headPath);

        const string legsPath = path + "legs/";
        legs.createTextureCache(legsPath);
}

void GameCharacter::update(Map *map) {
        // gestion de l'état du personnage
        if (!onRagdoll) {
                if (entityBody->GetLinearVelocity().x <= 0.5f &&
                    entityBody->GetLinearVelocity().x >= -0.5f &&
                    legs.state == MemberState::run) {
                        legs.state = MemberState::idle;
                }

                if (!touchingDown &&
                    legs.state == MemberState::run) {
                        legs.state = MemberState::idle;
                }

                // Récupère la vélocité actuelle
                b2Vec2 velocity = entityBody->GetLinearVelocity();

                // Limite la composante x
                if (velocity.x > maxVelocityX)
                        velocity.x = maxVelocityX;
                else if (velocity.x < -maxVelocityX)
                        velocity.x = -maxVelocityX;

                // Réapplique la vélocité limitée
                entityBody->SetLinearVelocity(velocity);
        }

        // mise à jour des membres
        leftArm.update();
        rightArm.update();
        head.update();
        body.update();
        legs.update();

        // mise à jour des objets
        gun->update();

#ifdef IMGUI_DEBUG
        // Interface character info
        ImGui::Begin("GameCharacter Info");
        ImGui::Text("Position: (%f, %f)", getPosition().x, getPosition().y);
        ImGui::Text("Looking point: (%f, %f)", lookingPoint.x, lookingPoint.y);
        ImGui::Text("Member angle: (head: %f, right arm: %f, left arm: %f)",
                    head.getRotation(), rightArm.getRotation(),
                    leftArm.getRotation());
        ImGui::Text("Legs state: %s",
                    gameCharacterStateString[static_cast<int>(legs.state)]);
        ImGui::Text("Body state: %s",
                    gameCharacterStateString[static_cast<int>(body.state)]);
        ImGui::Text("Right Arm state: %s",
                    gameCharacterStateString[static_cast<int>(rightArm.state)]);
        ImGui::Text("Left Arm state: %s",
                    gameCharacterStateString[static_cast<int>(leftArm.state)]);
        ImGui::Text("Head state: %s",
                    gameCharacterStateString[static_cast<int>(head.state)]);
        ImGui::End();
#endif
}

void GameCharacter::setPos(const Vector2f &pos) {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        setPosition(pos);

        body.setAttachPoint(pos);

        if (characterDirection == rightDir) {

                leftArm.setOrigin(8 * scale,
                                  16 * scale); // origine au niveau de l'épaule
                rightArm.setOrigin(8 * scale, 16 * scale);
                gun->setOrigin(8 * scale, 16 * scale);

                rightArm.setAttachPoint(pos.x - 5 * scale, pos.y - 2 * scale);
                leftArm.setAttachPoint(pos.x - 2 * scale, pos.y - 2 * scale);

        } else {

                leftArm.setOrigin(24 * scale,
                                  16 * scale); // origine au niveau de l'épaule
                rightArm.setOrigin(24 * scale, 16 * scale);
                gun->setOrigin(56 * scale, 16 * scale);

                rightArm.setAttachPoint(pos.x + 5 * scale, pos.y - 2 * scale);
                leftArm.setAttachPoint(pos.x + 2 * scale, pos.y - 2 * scale);
        }

        gun->setAttachPoint(rightArm.attachPoint);

        head.setAttachPoint(pos.x, pos.y - 9 * scale);

        legs.setAttachPoint(pos.x, pos.y + 8 * scale);
}

void GameCharacter::setPos(const float x, const float y) {
        setPos(Vector2f(x, y));
}

void GameCharacter::Draw(GLint *renderUniforms) const {
        leftArm.Draw(renderUniforms);
        legs.Draw(renderUniforms);
        body.Draw(renderUniforms);
        head.Draw(renderUniforms);
        gun->Draw(renderUniforms);
        rightArm.Draw(renderUniforms);
}

void GameCharacter::lookAt(const Vector2f &lp) {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        lookingPoint = lp;

        // on utilise le centre x entre la pos du body et du right arm pour
        // déterminer la direction du character
        float xCenter;
        if (characterDirection == rightDir) {
                xCenter =
                    rightArm.getPosition().x +
                    ((body.getPosition().x - rightArm.getPosition().x) / 2.f);
        } else {
                xCenter =
                    body.getPosition().x +
                    ((rightArm.getPosition().x - body.getPosition().x) / 2.f);
        }

        // faut-il changer la direction ?
        if (characterDirection == rightDir && xCenter > lookingPoint.x) {
                characterDirection = leftDir;
                flipY();
        }

        if (characterDirection == leftDir && xCenter < lookingPoint.x) {
                characterDirection = rightDir;
                flipY();
        }

        setMemberRotation(leftArm);
        setMemberRotation(rightArm);
        setMemberRotation(head);

        gun->setRotation(rightArm.getRotation());
}

void GameCharacter::setMemberRotation(Sprite &member) {
        setMemberRotation(member, 1.f);
}

void GameCharacter::setMemberRotation(Sprite &member, const float m) {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        // on détermine l'angle de rotation des bras pour suivre la souris
        const float distanceC =
            bbopGetDistance(member.getPosition(), lookingPoint);
        const float distanceA = member.getPosition().x - lookingPoint.x;
        const float distanceB = member.getPosition().y - lookingPoint.y;

        float rotation = acos((distanceA * distanceA + distanceC * distanceC -
                               distanceB * distanceB) /
                              (2 * distanceA * distanceC));
        rotation *= m;

        if (distanceA < 0)
                rotation += M_PI;
        if (distanceB < 0)
                rotation = -rotation;

        member.setRotation(rotation);
}

Vector2f GameCharacter::getLookingPoint() const { return lookingPoint; }

void GameCharacter::flipY() {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        rightArm.flipVertically();
        leftArm.flipVertically();
        body.flipVertically();
        head.flipVertically();
        legs.flipVertically();

        gun->flipVertically();

        if (characterDirection == rightDir) {

                leftArm.setOrigin(8 * scale,
                                  8 * scale); // origine au niveau de l'épaule
                rightArm.setOrigin(8 * scale, 8 * scale);

                rightArm.setAttachPoint(body.getPosition().x - 5 * scale,
                                        rightArm.getPosition().y);
                leftArm.setAttachPoint(body.getPosition().x - 2 * scale,
                                       leftArm.getPosition().y);

                gun->setGunDirection(rightDir);
        } else {
                leftArm.setOrigin(24 * scale,
                                  8 * scale); // origine au niveau de l'épaule
                rightArm.setOrigin(24 * scale, 8 * scale);

                gun->setGunDirection(leftDir);

                rightArm.setAttachPoint(body.getPosition().x + 5 * scale,
                                        rightArm.getPosition().y);
                leftArm.setAttachPoint(body.getPosition().x + 2 * scale,
                                       leftArm.getPosition().y);
        }
}

void GameCharacter::goLeft(const float newtonDiff) {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        // gestion physique
        const b2Vec2 velocity(-(newtonX + newtonDiff), 0.0f);
        entityBody->ApplyForceToCenter(velocity, true);

        // gestion animation
        if (legs.state != MemberState::run) {
                legs.state = MemberState::run;
                if (characterDirection == rightDir)
                        legs.animation->setReverse(true);
                else
                        legs.animation->setReverse(false);
        }
}

void GameCharacter::goRight(const float newtonDiff) {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        // gestion physique
        const b2Vec2 velocity(newtonX + newtonDiff, 0.0f);
        entityBody->ApplyForceToCenter(velocity, true);

        // gestion animation
        if (legs.state != MemberState::run) {
                legs.state = MemberState::run;
                if (characterDirection == rightDir)
                        legs.animation->setReverse(false);
                else
                        legs.animation->setReverse(true);
        }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void GameCharacter::jump() {

        // Ragdoll secure check
        if (onRagdoll)
                return;

        if (!touchingDown)
                return;

        const b2Vec2 impulsion(0.0f, -entityBody->GetMass() *
                                         newtonY); // vers le haut
        entityBody->ApplyLinearImpulseToCenter(impulsion, true);
        touchingDown = false;
}

// GETTER
Member &GameCharacter::getLeftArm() { return leftArm; }
Member &GameCharacter::getRightArm() { return rightArm; }
Member &GameCharacter::getBody() { return body; }
Member &GameCharacter::getHead() { return head; }
Member &GameCharacter::getLegs() { return legs; }
float GameCharacter::getHp() const { return hp; }
Gun &GameCharacter::getGun() const { return *gun; }
bool GameCharacter::isTouchingDown() const { return touchingDown; }

// SETTER
void GameCharacter::setHp(const float _hp) { this->hp = _hp; }
void GameCharacter::setTouchingDown(const bool touchingDown) { this->touchingDown = touchingDown; };

// ENTITY
void GameCharacter::computePhysic(b2World *world) {
        setSize(10.f * scale, 63.f * scale);
        setOrigin(getSize().x / 2, getSize().y / 2);
        entityBody = addDynamicBox(world, this, restitution, density, friction,
                                   linearDamping, true, false);

        auto *data = new BodyData;
        data->type = BodyType::GameCharacter;
        data->ptr = reinterpret_cast<uintptr_t>(this);
        entityBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

void GameCharacter::updatePhysic() {
        if (!onRagdoll) {
                setPos(entityBody->GetPosition().x * PIXEL_PER_METER,
                       entityBody->GetPosition().y * PIXEL_PER_METER - 5.f);
        } else {
                head.setPosition(headR->GetPosition().x * PIXEL_PER_METER,
                                 headR->GetPosition().y * PIXEL_PER_METER);
                head.setRotation(headR->GetAngle());

                body.setPosition(bodyR->GetPosition().x * PIXEL_PER_METER,
                                 bodyR->GetPosition().y * PIXEL_PER_METER);
                body.setRotation(bodyR->GetAngle());

                legs.setPosition(legsR->GetPosition().x * PIXEL_PER_METER,
                                 legsR->GetPosition().y * PIXEL_PER_METER);
                legs.setRotation(legsR->GetAngle());

                rightArm.setPosition(
                    rightArmR->GetPosition().x * PIXEL_PER_METER,
                    rightArmR->GetPosition().y * PIXEL_PER_METER);
                rightArm.setRotation(rightArmR->GetAngle());

                leftArm.setPosition(leftArmR->GetPosition().x * PIXEL_PER_METER,
                                    leftArmR->GetPosition().y *
                                        PIXEL_PER_METER);
                leftArm.setRotation(leftArmR->GetAngle());
        }
}

void GameCharacter::toggleRagdollMod(b2World *world) {

        // Destruction de l'ancion corps
        world->DestroyBody(entityBody);

        onRagdoll = true;

        for (Member *m : {&head, &body, &legs, &rightArm, &leftArm}) {
                m->state = MemberState::ragdoll;
        }

        // changement de taille pour les nouvelle texture ragdoll.png
        head.setSize(12 * scale, 12 * scale);
        body.setSize(18 * scale, 26 * scale);
        legs.setSize(19 * scale, 32 * scale);
        rightArm.setSize(23 * scale, 11 * scale);
        leftArm.setSize(23 * scale, 11 * scale);

        // mise en place des ragdolls pour chaque membres (TEMPORAIRE)
        bodyR = addDynamicBox(world, &body.getCollisionBox(), 0.f, 1.f, 1.f,
                              1.f, false, false, body.getCollisionBox().getOffsetX(),
                              body.getCollisionBox().getOffsetY());
        headR = addDynamicBox(world, &head.getCollisionBox(), 0.f, 1.f, 1.f,
                              1.f, false, false, head.getCollisionBox().getOffsetX(),
                              head.getCollisionBox().getOffsetY());
        legsR = addDynamicBox(world, &legs.getCollisionBox(), 0.f, 1.f, 1.f,
                              1.f, false, false, legs.getCollisionBox().getOffsetX(),
                              legs.getCollisionBox().getOffsetY());
        rightArmR =
            addDynamicBox(world, &rightArm.getCollisionBox(), 0.f, 1.f, 1.f,
                          1.f, false, false, rightArm.getCollisionBox().getOffsetX(),
                          rightArm.getCollisionBox().getOffsetY());
        leftArmR =
            addDynamicBox(world, &leftArm.getCollisionBox(), 0.f, 1.f, 1.f, 1.f,
                          false, false, leftArm.getCollisionBox().getOffsetX(),
                          leftArm.getCollisionBox().getOffsetY());

        headR->SetLinearVelocity(b2Vec2(20.0f, 0.0f));

        head.setOrigin(head.getCollisionBox().getOrigin());
        legs.setOrigin(legs.getCollisionBox().getOrigin());
        body.setOrigin(body.getCollisionBox().getOrigin());
        rightArm.setOrigin(rightArm.getCollisionBox().getOrigin());
        leftArm.setOrigin(leftArm.getCollisionBox().getOrigin());

        // creation des joints
        b2RevoluteJointDef neckJoint;
        neckJoint.bodyA = bodyR;
        neckJoint.bodyB = headR;
        neckJoint.collideConnected = false;
        neckJoint.enableLimit = false;
        neckJoint.localAnchorA.Set(
            0.f, -13.f * scale / PIXEL_PER_METER); // en haut du torse
        neckJoint.localAnchorB.Set(
            0.f, 6.f * scale / PIXEL_PER_METER); // en bas de la tête
        neckJoint.enableLimit = true;
        neckJoint.lowerAngle = -0.3f;
        neckJoint.upperAngle = 0.3f;

        world->CreateJoint(&neckJoint);

        b2RevoluteJointDef legJoint;
        legJoint.bodyA = bodyR;
        legJoint.bodyB = legsR;
        legJoint.collideConnected = false;
        legJoint.enableLimit = false;
        legJoint.localAnchorA.Set(0.f, 13.f * scale / PIXEL_PER_METER);
        legJoint.localAnchorB.Set(0.f, -16.f * scale / PIXEL_PER_METER);
        legJoint.enableLimit = true;
        legJoint.lowerAngle = -0.3f;
        legJoint.upperAngle = 0.3f;
        world->CreateJoint(&legJoint);

        b2RevoluteJointDef armRightJoint;
        armRightJoint.bodyA = bodyR;
        armRightJoint.bodyB = rightArmR;
        armRightJoint.collideConnected = false;
        armRightJoint.enableLimit = false;
        armRightJoint.localAnchorA.Set(-9.f * scale / PIXEL_PER_METER,
                                       -13.f * scale / PIXEL_PER_METER);
        armRightJoint.localAnchorB.Set(11.5f * scale / PIXEL_PER_METER,
                                       5.5f * scale / PIXEL_PER_METER);

        world->CreateJoint(&armRightJoint);

        b2RevoluteJointDef armLeftJoint;
        armLeftJoint.bodyA = bodyR;
        armLeftJoint.bodyB = leftArmR;
        armLeftJoint.collideConnected = false;
        armLeftJoint.enableLimit = false;
        armLeftJoint.localAnchorA.Set(9.f * scale / PIXEL_PER_METER,
                                      -13.f * scale / PIXEL_PER_METER);
        armLeftJoint.localAnchorB.Set(-11.5f * scale / PIXEL_PER_METER,
                                      5.5f * scale / PIXEL_PER_METER);

        world->CreateJoint(&armLeftJoint);
}
