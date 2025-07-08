#include "member.h"

#include <GLFW/glfw3.h>

#include <exception>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../../Bbop-2D/include/BBOP/Graphics/bbopMathClass.h"
#include "../../Bbop-2D/include/BBOP/Graphics/textureClass.h"

using namespace std;
using json = nlohmann::json;

Member::Member() : attachPoint(Vector2f(0.f, 0.f)), force(1.f) {
        state = MemberState::idle;
        animation = std::unique_ptr<AnimationComponent<MemberState>>(
            new AnimationComponent<MemberState>(this));
}

void Member::update() {
        // play animation
        //
        if (animation) {
                if (state != MemberState::dead) {
                        animation->play(state);
                }
        }
}

void Member::setAttachPoint(const Vector2f& _attachPoint) {
        attachPoint = _attachPoint;
        setPosition(attachPoint);
}

void Member::setAttachPoint(float x, float y) {
        setAttachPoint(Vector2f(x, y));
}

Vector2f Member::getAttachPoint() const { return attachPoint; }

void Member::createTextureCache(const std::string& path) {
        if (animation) {
                for (int i = 0; i < static_cast<int>(MemberState::dead); ++i) {
                        const auto state = static_cast<MemberState>(i);
                        animation->loadTexture(state, path);
                }
        }
}

void Member::setetat(int k) {
        if (k == 0) {
                state = MemberState::idle;
        } else if (k == 1) {
                state = MemberState::run;
        } else if (k == 2) {
                state = MemberState::dead;
        } else {
                cerr << "state do not exist" << endl;
        }
}

int Member::getetat() { return static_cast<int>(state); }
