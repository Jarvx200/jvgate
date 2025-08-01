#include    <stdio.h>
#include    <malloc.h>
#include    "gates.h"
#include    "logic_elements.h"

int main(){


    LogicGate* a = create(NOT);
    LogicGate* b = create(NOT);
    b->l.o = 0;
    connect(b, a);
    a->l.compute(&a->l);
    printf("%d",a->l.o);



    





    return 0;
}