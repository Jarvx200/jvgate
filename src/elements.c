#include "elements.h"
#include "logic_elements.h"
#include "graphic_elements.h"
#include "settings.h"
#include "raylib.h"
#include "raymath.h"

#include    <malloc.h>
#include    <stddef.h>



Element* elements[MAX_GATES_SIZE];
size_t elements_size = 0;

Switch* create_switch(Element e);
Gate* create_gate(Element e);
Output* create_output(Element e);
Compound* create_compound(Element e, Element** inner_graph, size_t inner_graph_size);

extern const size_t mem_size[] = {
    #define TRANS(X) [X] =  SIZEOF_##X,
    ELEMENTS
    #undef TRANS
};

void clone_graph(Element*** internal_graph,Element** elements, size_t g_size, Element* node, size_t* ig_size);
void clone_graph_loop(Element*** internal_graph,Element** elements, size_t g_size);

Element* create_element(enum ElementType t, Vector2 coords, Element** inner_graph, size_t inner_graph_size, GateBool graphic){
    Element e = {
        .t = t,
        .l.compute = gateBinds[t].comp,
        .l.i = malloc(sizeof(LogicElement*) * gateBinds[t].input_size), 
        .l.max_input  = gateBinds[t].input_size,
        .g_meta.max_input_copy = gateBinds[t].input_size,
        .g_meta.visited = FALSE,
        .l.input_size = 0,
        .corespondence = NULL,
        .corespondence_size = 0,
        .g_meta.clone = NULL
    };

    if(graphic == TRUE){
        GraphicElement* g = (GraphicElement*) malloc(sizeof(GraphicElement));

        g->pos = coords;
        g->connection_points = (ConnectionPoint*)malloc(sizeof(ConnectionPoint)*gateBinds[t].input_size),
        g->connection_points_size=0,
        g->draw_element = (t < SWITCH ? graphicElementsMeta[0] : graphicElementsMeta[t]),

        e.g = g;
    }




    
    Element* wrapper;
    if(t >= NOT && t <= XNOR)
        wrapper = (Element*)create_gate(e);
    else if(t == SWITCH)
       wrapper =  (Element*)create_switch(e);
    else if (t == OUTPUT)
        wrapper = (Element*)create_output(e);
    else if ( t == COMPOUND)
        wrapper = (Element*)create_compound(e, inner_graph, inner_graph_size);

    // Ref the heap (reffed stack a couple times :p )

    if(wrapper != NULL){
        if(graphic == TRUE){
            wrapper->g->wrapper = wrapper;
            wrapper->g->max_connection_points = &wrapper->l.max_input;
        }
        wrapper->l.wrapper = wrapper;

        create_inputs_and_output(wrapper, coords);
    }


    return wrapper;
}

Compound* create_compound(Element e, Element** inner_graph, size_t inner_graph_size){
    if(inner_graph_size == 0)
        return NULL;
    Compound* c = (Compound*)(malloc(sizeof(Compound)));

    free(e.l.i);
    if(e.g != NULL)
    free(e.g->connection_points);

    e.l.max_input= 0;
    for(size_t i=0; i < inner_graph_size; i++){
        if(inner_graph[i]->t == SWITCH)
        e.l.max_input+=1;
    }
    
    e.g_meta.max_input_copy = e.l.max_input;
    e.l.i = (LogicElement**) malloc(sizeof(LogicElement*) * e.l.max_input);
    if(e.g != NULL)
    e.g->connection_points = (ConnectionPoint*)malloc(sizeof(ConnectionPoint)*e.l.max_input);

    c->internal_graph = (Element**) malloc(sizeof(Element*)*inner_graph_size);

    clone_graph_loop(&c->internal_graph, inner_graph, inner_graph_size);
    
    // RESTORE HEAP GRAPH

    c->e = e;

    return c;
}

void clone_graph_loop(Element*** internal_graph,Element** elements, size_t g_size){
    size_t ig_size = 0;
    for(size_t i = 0; i < g_size; i++){
        if(elements[i]->l.max_input == 0){
            clone_graph(internal_graph, elements, g_size, elements[i],  &ig_size);
        }
    }

}

void clone_graph(Element*** internal_graph,Element** elements, size_t g_size, Element* node, size_t* ig_size){
        if(node->g_meta.clone == NULL){

            printf("\n ELEMENT: %s \n", nameBinds[node->t]);
            node->g_meta.clone =  create_element(node->t, (Vector2){0,0}, NULL, 0, FALSE); 
            Element* clone_details = node->g_meta.clone;
            clone_details->g = NULL;
            clone_details->corespondence_size = 0;


            (*internal_graph)[*ig_size]=clone_details;
            (*ig_size)++;
        } 
        for(size_t j = 0 ; j < node->corespondence_size; j++){
            if(node->corespondence[j]->g_meta.clone == NULL)
                clone_graph(internal_graph, elements, g_size, node->corespondence[j], ig_size);
            printf("\n DIRECTED CON: %s -> %s\n", nameBinds[node->t], nameBinds[node->corespondence[j]->t]);
            connect_gate(node->g_meta.clone, node->corespondence[j]->g_meta.clone);
        }
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
    if(nlg->g == NULL)
        return;
      for(size_t i=0 ; i < *(nlg->g->max_connection_points); i++){
        nlg->g->connection_points[i].coords.x = coords.x;
        nlg->g->connection_points[i].coords.y = coords.y+i*30+10;
    }

    nlg->g->connection_output_point.coords.x = coords.x+element_sizes[nlg->t >= SWITCH ? nlg->t : 0];
    nlg->g->connection_output_point.coords.y = coords.y+element_sizes[nlg->t >= SWITCH ? nlg->t : 0]/2-5;
    nlg->g->selected = FALSE;

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
    
    
    if(x->g == NULL || y->g == NULL) return;
    y->g->connection_points[y->g->connection_points_size++].corespondence=&x->g->connection_output_point;
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
    if(y->g == NULL) return;
    for(size_t i=0; i < y->g->connection_points_size; i++){
        if(y->g->connection_points[i].corespondence == &(x->g->connection_output_point)) {y->g->connection_points[i].corespondence = NULL; ok = TRUE; continue;}
        if(ok == TRUE) y->g->connection_points[i-1] = y->g->connection_points[i];
    }
    if(ok == TRUE)
    y->g->connection_points_size-=1;
    x->g->connection_output_point.corespondence = NULL;
}

// TODO:IMPORTANT: Extract array deletion logic in one method 

void delete_element(Element* e){
    /* sizet will overflow (unsigned), move to signed type*/
    int32_t i=e->l.input_size-1;
    while(i >= 0){ disconnect_gate((Element*) e->l.i[i--]->wrapper,e); }

    i=e->corespondence_size-1;
    while(i >= 0){ disconnect_gate(e, e->corespondence[i--]);}

    GateBool ok = FALSE;
    for(size_t i = 0 ; i < elements_size; i++){
        if(elements[i] == e){
            elements[i] = NULL; ok=TRUE;
            continue;
        }
        if(ok == TRUE){
            elements[i-1]=elements[i];
        }
    }
    if(ok == TRUE)
        elements_size-=1;


    
    free(e->g);

    free(e);
}
