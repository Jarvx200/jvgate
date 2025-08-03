#ifndef LOGIC_ELEMENTS_H 
#define LOGIC_ELEMENTS_H

#include    <stdint.h>
#include    <stddef.h>

#define TRUE    1
#define FALSE   0
typedef char GateBool; 

typedef struct LogicElement LogicElement;

enum LogicElementType {
    GATE,
    SWITCH,
    OUTPUT
};

void not_comp(LogicElement* self);
void and_comp(LogicElement* self);
void nand_comp(LogicElement* self);
void or_comp(LogicElement* self);
void nor_comp(LogicElement* self);
void xor_comp(LogicElement* self);
void xnor_comp(LogicElement* self);

typedef struct {
 void (*comp)(struct LogicElement*);   
 size_t input_size;
} LogicElementMeta;



extern const LogicElementMeta gateBinds[];


enum GateType{
    NOT,
    AND,
    NAND,
    OR,
    NOR,
    XOR,
    XNOR
};



struct LogicElement{
    struct LogicElement** i;
    enum LogicElementType type;
    size_t max_input;
    size_t input_size;

    void (*compute)(struct LogicElement* self);
    GateBool o;

};




#endif