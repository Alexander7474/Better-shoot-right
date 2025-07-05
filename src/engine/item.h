//
// Created by artyom on 7/4/25.
//

#ifndef ITEM_H
#define ITEM_H

#include <memory>

#include "animationComponent.h"


class Item : public Sprite {
private:
        std::string name;
protected:
        std::unique_ptr<IAnimationComponent> animation; //<! permet de stocker un AnimationComponent<EnumState> pour certain item
public:
        explicit Item(const Texture& texture);

        virtual void update() = 0;

        const std::string &getName() const;

        void setName(const std::string &name);
};



#endif //ITEM_H
