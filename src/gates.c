#include "gates.h"
#include "logic_elements.h"
#include "graphic_elements.h"

#include    <malloc.h>
#include    <stddef.h>

LogicGate* create(enum Type t){
    LogicGate* new_logic_gate = (LogicGate*)(malloc(sizeof(LogicGate)));
    new_logic_gate->l.compute = binds[t].comp;
    new_logic_gate->l.i = (LogicElement**)(malloc(sizeof(LogicElement*)*binds[t].input_size));
    new_logic_gate->l.max_input = binds[t].input_size;
    new_logic_gate->l.input_size = 0;

    return new_logic_gate;
}

void connect(LogicGate* x, LogicGate* y){
    if(y->l.input_size == y->l.max_input)
        return;
    y->l.i[y->l.input_size] = &(x->l);
    y->l.input_size+=1;
}