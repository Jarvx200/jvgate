#ifndef GATE_FUNCTIONS_H
#define GATE_FUNCTIONS_H

#include "elements.h"

void not_comp(LogicElement* self);
void and_comp(LogicElement* self);
void nand_comp(LogicElement* self);
void or_comp(LogicElement* self);
void nor_comp(LogicElement* self);
void xor_comp(LogicElement* self);
void xnor_comp(LogicElement* self);

#endif