#include "settings.h"
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
   [SWITCH] = "SWITCH", 
   [OUTPUT] = "OUTPUT",
   [LAST]   = NULL
};

uint16_t element_sizes[] = {
    [0] = 100,
    [SWITCH] = 50,
    [OUTPUT] = 50
};

typedef void (*DrawFunction)(enum ElementType,struct GraphicElement*, ...);

void (*graphicElementsMeta[])(enum ElementType,struct GraphicElement*, ...) = {
    [0] = (DrawFunction)&draw_gate,
    [SWITCH] = (DrawFunction)&draw_switch,
    [OUTPUT] = (DrawFunction)&draw_output

};   

//TODO: Split drawing by common parts

void draw_gate(enum ElementType t,struct GraphicElement* self ){
    DrawRectangleLines(self->pos.x, self->pos.y, 100, 100, self->selected ? GREEN : FGR_COLOR);
    DrawText(nameBinds[t],self->pos.x+10, self->pos.y+10, 12, FGR_COLOR);

    DrawCircle(self->connection_output_point.coords.x, self->connection_output_point.coords.y, 5, RED);
    
    for(size_t i=0 ; i < self->max_connection_points; i++){
        DrawCircle(self->connection_points[i].coords.x, self->connection_points[i].coords.y, 5, GREEN);
    }
}

void draw_switch(enum ElementType t, struct GraphicElement* self, GateBool on){
    DrawRectangleLines(self->pos.x, self->pos.y, 50, 50, self->selected ? GREEN : FGR_COLOR);
    DrawText(on == TRUE ? "ON" : "OFF" ,self->pos.x+10, self->pos.y+10, 12, FGR_COLOR);
    DrawCircle(self->connection_output_point.coords.x, self->connection_output_point.coords.y, 5, RED);
}

void draw_output(enum ElementType t, struct GraphicElement* self, GateBool powered )
{
    DrawRectangleLines(self->pos.x, self->pos.y, 50, 50, self->selected ? GREEN : FGR_COLOR);
    DrawRectangle(self->pos.x+10, self->pos.y+10, 30, 30, powered == TRUE ? RED : FGR_COLOR);
    for(size_t i=0 ; i < self->max_connection_points; i++){
        DrawCircle(self->connection_points[i].coords.x, self->connection_points[i].coords.y, 5, GREEN);
    }
}
