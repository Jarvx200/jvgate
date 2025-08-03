#ifndef GATES_H
#define GATES_H

#include "logic_elements.h"
#include "graphic_elements.h"

#include "raylib.h"
#include "raymath.h"

typedef struct LogicGate LogicGate;

struct LogicGate{
    GraphicElement g;
    LogicElement   l;
    enum GateType t;
    LogicGate* corespondence;
} ;



LogicGate* create_gate(enum GateType t, Vector2 coords);
void connect_gate(LogicGate* x, LogicGate* y);
void disconnect_gate(LogicGate* x, LogicGate* y);



#endif
