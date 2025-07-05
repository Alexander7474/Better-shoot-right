//
// Created by artyom on 7/4/25.
//

#include "item.h"

Item::Item(const Texture &texture)
        : Sprite(texture)
{}

const std::string &Item::getName() const { return name; }

void Item::setName(const std::string &name) { this->name = name; }

