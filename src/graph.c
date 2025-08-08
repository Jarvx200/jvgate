#include "graph.h"

#include <stdio.h>

void restore_graph(Element** elements, size_t elemets_size){
    for(size_t i=0 ; i < elemets_size; i++){
        elements[i]->g_meta.max_input_copy = elements[i]->l.input_size;
        elements[i]->g_meta.visited=FALSE;
        elements[i]->g_meta.in_stack=FALSE;
    }
}


void compute_output(Element* e){
    switch (e->t)
    {
        case SWITCH:
            e->l.compute(&e->l, ((Switch*)e)->on);
            break;
        case OUTPUT:
            e->l.compute(&e->l, &(((Output*)e)->powered));
            break;
        default:
            e->l.compute(&e->l);
            break;
    }


    for(int i=0 ; i < e->corespondence_size; i++)
        e->corespondence[i]->g_meta.max_input_copy-=1;

    e->g_meta.max_input_copy= -1;
}


static GateBool dfs_v(Element* root){

    if(root->g_meta.visited == TRUE) return FALSE;
    if(root->g_meta.in_stack == TRUE) return TRUE;

    root->g_meta.in_stack = TRUE;
    root->g_meta.visited = TRUE;
    
    for(int i=0 ; i < root->corespondence_size; i++){
        if(dfs_v(root->corespondence[i]))
            return TRUE;
    }

    root->g_meta.in_stack = FALSE;


    return FALSE;
}

GateBool acyclic(Element** elements, size_t element_sizes){
    restore_graph(elements, element_sizes);
    for(size_t i=0; i < element_sizes; i++){
        if(elements[i]->g_meta.visited == FALSE)
            if(dfs_v(elements[i]) == TRUE)
                return FALSE;
    }

    return TRUE;

}

void top_sort(Element** elements, size_t elements_size, GateBool going){
    if(going == FALSE){
        restore_graph(elements, elements_size);
        return;
    }
    GateBool changed = FALSE;
    for(size_t i=0; i < elements_size; i++){
        if(elements[i]->l.input_size != elements[i]->l.max_input) { puts("INVALID CIRCUIT\n"); going = FALSE; break;}
        if(elements[i]->g_meta.max_input_copy == 0) { compute_output(elements[i]), changed = TRUE;}
    }
    top_sort(elements, elements_size, changed);
}
