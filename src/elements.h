#ifndef GATES_H
#define GATES_H

#include "logic_elements.h"
#include "graphic_elements.h"

#include "raylib.h"
#include "raymath.h"

typedef struct Element Element;

struct Element{
    GraphicElement g;
    LogicElement   l;
    enum ElementType t;
    Element* corespondence;
} ;



Element* create_gate(enum ElementType t, Vector2 coords);
void connect_gate(Element* x, Element* y);
void disconnect_gate(Element* x, Element* y);
void create_inputs_and_output(Element* new_logic_gate, Vector2 coords);


#endif
