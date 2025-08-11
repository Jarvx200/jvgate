#ifndef GRPAHIC_ELEMENTS_H
#define GRPAHIC_ELEMENTS_H

#include <stdint.h>
#include "raylib.h"
#include "logic_elements.h"

typedef struct GraphicElement GraphicElement;
typedef struct ConnectionPoint ConnectionPoint;

#define COMPOUND_SIZE(x)  max(100, x*35 )
#define GET_ELEMENT_SIZE(e) element_sizes[(e->t >= SWITCH ? e->t : 0)]

struct ConnectionPoint{
    Vector2 coords;
    struct ConnectionPoint* corespondence;
};

struct GraphicElement {

    Vector2 pos;
    struct ConnectionPoint* connection_points;
    struct ConnectionPoint connection_output_point;
    size_t *max_connection_points;
    size_t connection_points_size;
    GateBool selected;

    void* wrapper; // parent

    void (*draw_element)(enum ElementType t,struct GraphicElement* self, ...);
}; 

extern uint16_t element_sizes[];
extern void (*graphicElementsMeta[])(enum ElementType t,struct GraphicElement*, ...);
extern char* nameBinds[];

void draw_gate(enum ElementType t,struct GraphicElement* self);
void draw_switch(enum ElementType t,struct GraphicElement* self, GateBool on);
void draw_output(enum ElementType t,struct GraphicElement* self, GateBool powered); 
void draw_compound(enum ElementType t,struct GraphicElement* self);




#endif