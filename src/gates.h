#ifndef GATES_H
#define GATES_H

#include "logic_elements.h"
#include "graphic_elements.h"



typedef struct {
    GraphicElement g;
    struct LogicElement   l;
    enum Type t;
} LogicGate;



LogicGate* create(enum Type t);
void connect(LogicGate* x, LogicGate* y);



#endif
