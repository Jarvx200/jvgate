#include "gates.h"
#include "logic_elements.h"
#include "graphic_elements.h"

#include    <malloc.h>
#include    <stddef.h>

LogicGate* create_gate(enum GateType t){
    LogicGate* new_logic_gate = (LogicGate*)(malloc(sizeof(LogicGate)));
    new_logic_gate->l.compute = gateBinds[t].comp;
    new_logic_gate->l.i = (LogicElement**)(malloc(sizeof(LogicElement*)*gateBinds[t].input_size));
    new_logic_gate->l.max_input = gateBinds[t].input_size;
    new_logic_gate->l.input_size = 0;

    return new_logic_gate;
}

void connect_gate(LogicGate* x, LogicGate* y){
    if(y->l.input_size == y->l.max_input)
        return;
    y->l.i[y->l.input_size] = &(x->l);
    y->l.input_size+=1;
}

void disconnect_gate(LogicGate* x, LogicGate* y){
    for(size_t k=0; k < x->l.input_size; k++){
        if(x->l.i[k] ==  &y->l) { x->l.i[k]=NULL; x->l.input_size-=1; return;}
    }
}