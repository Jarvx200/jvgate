#include "gates.h"
#include "logic_elements.h"
#include "graphic_elements.h"
#include "settings.h"
#include "raylib.h"
#include "raymath.h"

#include    <malloc.h>
#include    <stddef.h>

LogicGate* create_gate(enum GateType t, Vector2 coords){
    LogicGate* new_logic_gate = (LogicGate*)(malloc(sizeof(LogicGate)));

    new_logic_gate->l.type = GATE;

    new_logic_gate->l.compute = gateBinds[t].comp;
    new_logic_gate->l.i = (LogicElement**)(malloc(sizeof(LogicElement*)*gateBinds[t].input_size));
    new_logic_gate->l.max_input = gateBinds[t].input_size;
    new_logic_gate->l.input_size = 0;

    new_logic_gate->t = t;

    new_logic_gate->g.pos.x = coords.x;
    new_logic_gate->g.pos.y = coords.y;
    new_logic_gate->g.draw_element = draw_gate;
    new_logic_gate->g.connection_points = (ConnectionPoint*)malloc(sizeof(ConnectionPoint)*gateBinds[t].input_size);
    new_logic_gate->g.max_connection_points = new_logic_gate->l.max_input;
    new_logic_gate->g.connection_points_size=0;

    for(size_t i=0 ; i < new_logic_gate->g.max_connection_points; i++){
        new_logic_gate->g.connection_points[i].coords.x = coords.x;
        new_logic_gate->g.connection_points[i].coords.y = coords.y+i*30+10;
    }

    new_logic_gate->g.connection_output_point.coords.x = coords.x+100;
    new_logic_gate->g.connection_output_point.coords.y = coords.y+45;
    new_logic_gate->g.selected = FALSE;

    return new_logic_gate;
}

// y input x output
void connect_gate(LogicGate* x, LogicGate* y){
    if(y->l.input_size == y->l.max_input  || x->g.connection_points_size == x->g.max_connection_points )
        return;
    y->l.i[y->l.input_size++] = &(x->l);
    x->corespondence = y;
    

    y->g.connection_points[y->g.connection_points_size++].corespondence=&x->g.connection_output_point;
}

void disconnect_gate(LogicGate* x, LogicGate* y){
    for(size_t k=0; k < x->l.input_size; k++){
        if(x->l.i[k] ==  &y->l) { x->l.i[k]=NULL; x->l.input_size-=1; y->corespondence = NULL; return;}
    }
}