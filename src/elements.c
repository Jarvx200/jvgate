#include "elements.h"
#include "logic_elements.h"
#include "graphic_elements.h"
#include "settings.h"
#include "raylib.h"
#include "raymath.h"

#include    <malloc.h>
#include    <stddef.h>



Switch* create_switch(Element e);
Gate* create_gate(Element e);
Output* create_output(Element e);


Element* create_element(enum ElementType t, Vector2 coords){
    Element e = {
        .t = t,
        .g.pos = coords,
        .l.compute = gateBinds[t].comp,
        .l.i = malloc(sizeof(LogicElement*) * gateBinds[t].input_size), 
        .l.max_input  = gateBinds[t].input_size,
        .l.max_input_copy = gateBinds[t].input_size,
        .l.input_size = 0,
        .corespondence = NULL,
        .corespondence_size = 0,
        .g.connection_points = (ConnectionPoint*)malloc(sizeof(ConnectionPoint)*gateBinds[t].input_size),
        .g.max_connection_points = gateBinds[t].input_size,
        .g.connection_points_size=0,
        .g.draw_element = (t < SWITCH ? graphicElementsMeta[0] : graphicElementsMeta[t]),
    };

    create_inputs_and_output(&e, coords);

    
    Element* wrapper;
    if(t >= NOT && t <= XNOR)
        wrapper = (Element*)create_gate(e);
    else if(t == SWITCH)
       wrapper =  (Element*)create_switch(e);
    else if (t == OUTPUT)
        wrapper = (Element*)create_output(e);


    return wrapper;
}

Output* create_output(Element e){
    Output* no = (Output*)(malloc(sizeof(Output)));
    no->e = e;

    no->powered = FALSE;

    return no;

}


Switch* create_switch(Element e){
    Switch* ns = (Switch*)(malloc(sizeof(Switch)));
    ns->e = e;
    ns->on = FALSE;
    ns->e.l.o = FALSE;

    return ns;
}

Gate* create_gate(Element e){
    Gate* nlg = (Gate*)(malloc(sizeof(Gate)));

    nlg->e = e;

    
  
    return nlg;

}

//FIX: Rendering based on actula object size
void create_inputs_and_output(Element* nlg, Vector2 coords){
      for(size_t i=0 ; i < nlg->g.max_connection_points; i++){
        nlg->g.connection_points[i].coords.x = coords.x;
        nlg->g.connection_points[i].coords.y = coords.y+i*30+10;
    }

    nlg->g.connection_output_point.coords.x = coords.x+element_sizes[nlg->t >= SWITCH ? nlg->t : 0];
    nlg->g.connection_output_point.coords.y = coords.y+element_sizes[nlg->t >= SWITCH ? nlg->t : 0]/2-5;
    nlg->g.selected = FALSE;

}


static GateBool check_corespondence(Element *x, Element *y){
    for(size_t i=0; i < x->corespondence_size; i++)
        if(x->corespondence[i] == y) return TRUE;
    return FALSE;
}
// y input x output
void connect_gate(Element* x, Element* y){
    
    GateBool coresponding = check_corespondence(x,y);
    if(y->l.input_size == y->l.max_input || coresponding == TRUE){
        if(coresponding == TRUE)
            disconnect_gate(x,y);
        return;
    }
    y->l.i[y->l.input_size++] = &(x->l);

    x->corespondence[x->corespondence_size++] = y;
    
    

    y->g.connection_points[y->g.connection_points_size++].corespondence=&x->g.connection_output_point;
}

// x ouput from y input
void disconnect_gate(Element* x, Element* y){

    GateBool ok = FALSE;
    for(size_t i=0; i < y->l.input_size; i++){
        if(y->l.i[i] == &(x->l)) {y->l.i[i] = NULL; ok = TRUE; continue;}
        if(ok == TRUE) y->l.i[i-1] = y->l.i[i];
    }

    if(ok == TRUE)
    y->l.input_size-=1;

    ok = FALSE;
    for(size_t i=0; i < x->corespondence_size; i++){
        if(x->corespondence[i] == y){
            x->corespondence[i]=NULL;
            ok = TRUE;
            continue;
        } if (ok == TRUE){ x->corespondence[i-1] = x->corespondence[i]; }
    }
    if(ok == TRUE)
    x->corespondence_size-=1;

    ok = FALSE;
    for(size_t i=0; i < y->g.connection_points_size; i++){
        if(y->g.connection_points[i].corespondence == &(x->g.connection_output_point)) {y->g.connection_points[i].corespondence = NULL; ok = TRUE; continue;}
        if(ok == TRUE) y->g.connection_points[i-1] = y->g.connection_points[i];
    }
    if(ok == TRUE)
    y->g.connection_points_size-=1;
    x->g.connection_output_point.corespondence = NULL;

}