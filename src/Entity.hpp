#ifndef ENTITY_HPP
#define ENTITY_HPP

struct Entity {
    virtual auto draw() -> void;

    int x, y;
};

#endif
