#include "graph.h"

#include <stdio.h>

void restore_graph(Element** elements, size_t elemets_size){
    for(size_t i=0 ; i < elemets_size; i++)
        elements[i]->l.max_input_copy = elements[i]->l.max_input;
}


void compute_output(Element* e){
    switch (e->t)
    {
        case NOT:
        case NOR:
        case XOR:
        case XNOR:
        case AND:
        case NAND:
        case OR:
            e->l.compute(&e->l);
            break;
        case SWITCH:
            e->l.compute(&e->l, ((Switch*)e)->on);
            break;
        case OUTPUT:
            ((Output*)e)->powered = e->l.i[0]->o; 
            break;
        default:
            break;
    }


    printf("%s - %d \n", nameBinds[e->t], e->l.o);
    if(e->corespondence)
    e->corespondence->l.max_input_copy-=1;
    e->l.max_input_copy = -1;
}

void top_sort(Element** elements, size_t elements_size, GateBool going){
    if(going == FALSE){
        restore_graph(elements, elements_size);
        return;
    }
    GateBool changed = FALSE;
    for(size_t i=0; i < elements_size; i++){
        if(elements[i]->l.max_input_copy == 0) { compute_output(elements[i]), changed = TRUE;}
    }
    top_sort(elements, elements_size, changed);
}
