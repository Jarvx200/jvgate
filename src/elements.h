#ifndef GATES_H
#define GATES_H

#include "logic_elements.h"
#include "graphic_elements.h"

#include "raylib.h"
#include "raymath.h"

typedef struct Element Element;
typedef struct Switch  Switch;
typedef struct Output Output;
typedef struct Gate Gate;

struct Element{
    enum ElementType t;
    GraphicElement g;
    LogicElement   l;
    struct Element* corespondence;
} ;

struct Switch{
    struct Element e;
    GateBool on;
};

struct Output{
    struct Element e;
    GateBool powered;
};

struct Gate{
    struct Element e;
};


Element* create_element(enum ElementType t, Vector2 coords);
void connect_gate(Element* x, Element* y);
void disconnect_gate(Element* x, Element* y);
void create_inputs_and_output(Element* nlg, Vector2 coords);


#endif
