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

void switch_comp(LogicElement* self, GateBool on){
    self->o = on;
}
void output_comp(LogicElement* self, GateBool *powered){
    if(self->input_size > 0){
        *powered=self->i[0]->o;
        self->o=self->i[0]->o;
    }
    else
        *powered=FALSE;
}



//TODO: Output size as well
const LogicElementMeta gateBinds[] = {
    [NOT] = { (ComputeFunction)&not_comp, 1},
    [AND] = { (ComputeFunction)&and_comp, 2},
    [NAND] = { (ComputeFunction)&nand_comp, 2},
    [OR] = { (ComputeFunction)&or_comp, 2},
    [NOR] = { (ComputeFunction)&nor_comp, 2},
    [XOR] = { (ComputeFunction)&xor_comp, 2},
    [XNOR] = { (ComputeFunction)&xnor_comp, 2},
    [SWITCH] = {(ComputeFunction)&switch_comp, 0},
    [OUTPUT] = {(ComputeFunction)&output_comp, 1}
};
