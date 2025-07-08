#include "crossair.h"

Crossair::Crossair() {

        middleRect.setColor(255, 0, 0);
        middleRect.setSize(1, 1);
        middleRect.setOrigin(middleRect.getSize().x / 2.f,
                             middleRect.getSize().y / 2.f);

        for (int i = 0; i < 4; i++) {
                externRect[i].setColor(255, 0, 0);
                if (i % 2 == 0) {
                        externRect[i].setSize(5, 1);
                } else {
                        externRect[i].setSize(1, 5);
                }
                externRect[i].setOrigin(externRect[i].getSize().x / 2.f,
                                        externRect[i].getSize().y / 2.f);
        }

        setSize(5, 5);

        setPosition(200, 200);
}

void Crossair::setPosition(float x_, float y_) {
        pos.x = x_;
        pos.y = y_;

        middleRect.setPosition(pos);
        externRect[0].setPosition(pos.x - size.x, pos.y);
        externRect[2].setPosition(pos.x + size.x, pos.y);
        externRect[1].setPosition(pos.x, pos.y - size.y);
        externRect[3].setPosition(pos.x, pos.y + size.y);
}

void Crossair::Draw(GLint *renderUniforms) const {
        middleRect.Draw(renderUniforms);
        for (int i = 0; i < 4; i++) {
                externRect[i].Draw(renderUniforms);
        }
}
