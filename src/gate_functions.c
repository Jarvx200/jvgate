#include "elements.h"


//TODO: Error recovery in case of insufficient i sources

void not_comp(LogicElement* self){
    self->o = !self->i[0]->o; // Using logical not to avoid bit flipping problems
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