#include "graphic_elements.h"
#include "logic_elements.h"
#include "raylib.h"
#include "raymath.h"

char* nameBinds[] = {
   [NOT] = "NOT", 
   [AND] = "AND", 
   [NAND] = "NAND", 
   [OR] = "OR", 
   [NOR] = "NOR", 
   [XOR] = "XOR", 
   [XNOR] = "XNOR", 
};

void draw_gate(enum ElementType t,struct GraphicElement* self){
    DrawRectangleLines(self->pos.x, self->pos.y, 100, 100, self->selected ? GREEN : BLACK );
    DrawText(nameBinds[t],self->pos.x+10, self->pos.y+10, 12, BLACK);

    DrawCircle(self->connection_output_point.coords.x, self->connection_output_point.coords.y, 5, RED);
    for(size_t i=0 ; i < self->max_connection_points; i++){
        DrawCircle(self->connection_points[i].coords.x, self->connection_points[i].coords.y, 5, GREEN);
    }
}

void draw_switch(struct GraphicElement* self, GateBool on){
    DrawRectangleLines(self->pos.x, self->pos.y, 50, 50, self->selected ? GREEN : BLACK );
    DrawText(on == TRUE ? "ON" : "OFF",self->pos.x+10, self->pos.y+10, 12, BLACK);
    DrawCircle(self->connection_output_point.coords.x, self->connection_output_point.coords.y, 5, RED);
}


