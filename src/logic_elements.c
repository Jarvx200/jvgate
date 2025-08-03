#include "logic_elements.h"


void not_comp(LogicElement* self){
    self->o = !self->i[0]->o; 
}

void and_comp(LogicElement* self){
    self->o = self->i[0]->o & self->i[1]->o;
}

void nand_comp(LogicElement* self){
    self->o = !(self->i[0]->o & self->i[1]->o);
}

void or_comp(LogicElement* self){
    self->o = self->i[0]->o | self->i[1]->o;
}

void nor_comp(LogicElement* self){
    self->o = !(self->i[0]->o | self->i[1]->o);
}

void xor_comp(LogicElement* self){
    self->o = self->i[0]->o ^ self->i[1]->o;
}

void xnor_comp(LogicElement* self){
    self->o = self->i[0]->o == self->i[1]->o;
}

//TODO: Output size as well
const LogicElementMeta gateBinds[] = {
    [NOT] = { &not_comp, 1},
    [AND] = { &and_comp, 2},
    [NAND] = { &nand_comp, 2},
    [OR] = { &or_comp, 2},
    [NOR] = { &nor_comp, 2},
    [XOR] = { &xor_comp, 2},
    [XNOR] = { &xnor_comp, 2},
    [SWITCH] = {NULL, 0},
    [OUTPUT] = {NULL, 1}
};

uint16_t elementSizes[] = {
    [0] = 100,
    [SWITCH] = 50,
    [OUTPUT] = 50
};