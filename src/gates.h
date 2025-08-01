#ifndef GATES_H
#define GATES_H

#include "elements.h"
#include "gate_functions.h"

enum Type{
    NOT,
    AND,
    NAND,
    OR,
    NOR,
    XOR,
    XNOR
};

void (*binds[])(struct LogicElement*) = {
    &not_comp,
    &and_comp,
    &nand_comp,
    &or_comp,
    &nor_comp,
    &xor_comp,
    &xnor_comp
};




typedef struct {
    GraphicElement g;
    struct LogicElement   l;
    Type t;
} LogicGate;



LogicGate* create(Type t);


#endif
