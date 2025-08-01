#ifndef GRPAHIC_ELEMENTS_H
#define GRPAHIC_ELEMENTS_H

#include <stdint.h>

typedef struct GraphicElement GraphicElement;


struct GraphicElement {

    int16_t x;
    int16_t y;

    void (*draw)(struct GraphicElement* self);
}; 



#endif