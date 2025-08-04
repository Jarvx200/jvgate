#ifndef GRAPH_H
#define GRAPH_H

#include "elements.h"
#include "logic_elements.h"
#include "graphic_elements.h"

void restore_graph(Element** elements, size_t elemets_size);
void top_sort(Element** elements, size_t elements_size, GateBool going);



#endif