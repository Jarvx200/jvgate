#ifndef LOGIC_ELEMENTS_H 
#define LOGIC_ELEMENTS_H

#include    <stdint.h>
#include    <stddef.h>


#define ELEMENTS \
    TRANS(NOT) \
    TRANS(AND) \
    TRANS(NAND) \
    TRANS(OR) \
    TRANS(NOR) \
    TRANS(XOR) \
    TRANS(XNOR) \
    TRANS(SWITCH) \
    TRANS(OUTPUT) \
    TRANS(COMPOUND) \
    TRANS(LAST) 


#define TRUE    1
#define FALSE   0
typedef char GateBool; 

typedef struct LogicElement LogicElement;


void not_comp(LogicElement* self);
void and_comp(LogicElement* self);
void nand_comp(LogicElement* self);
void or_comp(LogicElement* self);
void nor_comp(LogicElement* self);
void xor_comp(LogicElement* self);
void xnor_comp(LogicElement* self);
void switch_comp(LogicElement* self, GateBool on);
void ouput_comp(LogicElement* self, GateBool powered);
void compound_comp(LogicElement* self);

typedef void (*ComputeFunction)(struct LogicElement*, ...);   

typedef struct {
 void (*comp)(struct LogicElement*, ...);   
 size_t input_size;
} LogicElementMeta;



extern const LogicElementMeta gateBinds[];


enum ElementType{
    #define TRANS(e) e,
    ELEMENTS
    #undef TRANS
};



// TODO: Refactor static commons between Logic and Graphic in parrent


struct LogicElement{
    struct LogicElement** i;
    size_t max_input;
    size_t input_size;
    ComputeFunction compute; 
    GateBool o;

    void* wrapper; // wrapper
};




#endif