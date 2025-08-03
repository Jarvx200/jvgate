#ifndef GRPAHIC_ELEMENTS_H
#define GRPAHIC_ELEMENTS_H

#include <stdint.h>
#include "raylib.h"
#include "logic_elements.h"

typedef struct GraphicElement GraphicElement;
typedef struct ConnectionPoint ConnectionPoint;


struct ConnectionPoint{
    Vector2 coords;
    struct ConnectionPoint* corespondence;
};

struct GraphicElement {

    Vector2 pos;
    struct ConnectionPoint* connection_points;
    struct ConnectionPoint connection_output_point;
    size_t max_connection_points;
    size_t connection_points_size;
    GateBool selected;

    void (*draw_element)(enum ElementType t,struct GraphicElement* self);
}; 


void draw_gate(enum ElementType t,struct GraphicElement* self);

#endif