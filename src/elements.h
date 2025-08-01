#ifndef ELEMENTS_H 
#define ELEMENTS_H

#include    <stdint.h>

#define TRUE    1
#define FALSE   0

typedef char Bool; 

typedef struct LogicElement LogicElement;
typedef struct GraphicElement GraphicElement;


struct GraphicElement {

    int16_t x;
    int16_t y;

    void (*draw)(struct GraphicElement* self);
}; 

struct LogicElement{
    struct LogicElement** i;

    void (*compute)(struct LogicElement* self);
    Bool o;

};




#endif