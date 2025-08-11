#ifndef GATES_H
#define GATES_H

#include "logic_elements.h"
#include "graphic_elements.h"
#include "settings.h"

#include "raylib.h"
#include "raymath.h"

#define MTEXT 128

typedef struct Element Element;
typedef struct Switch  Switch;
typedef struct Output Output;
typedef struct Gate Gate;
typedef struct GraphMeta GraphMeta;
typedef struct Compound Compound;

struct GraphMeta {
    size_t max_input_copy;
    GateBool visited;
    GateBool in_stack;
};

extern Element* elements[MAX_GATES_SIZE];
extern size_t elements_size;

struct Element{
    enum ElementType t;
    GraphicElement g;
    LogicElement   l;
    struct Element* corespondence[MAX_GATES_SIZE];
    size_t corespondence_size;
    GraphMeta g_meta;
    
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
 

struct Compound{
    struct Element e;
    struct Element** internal_graph;
    char* label[MTEXT];
};



Element* create_element(enum ElementType t, Vector2 coords, Element** inner_graph, size_t inner_graph_size);
void connect_gate(Element* x, Element* y);
void disconnect_gate(Element* x, Element* y);
void delete_element(Element* x);
void create_inputs_and_output(Element* nlg, Vector2 coords);


#endif
