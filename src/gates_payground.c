#include    <pthread.h>
#include    <stdio.h>
#include    <stdint.h>
#include    <malloc.h>

#include    "raylib.h"
#include    "raymath.h"
#include    "settings.h"
#include    "elements.h"
#include    "logic_elements.h"
#include    "graphic_elements.h"
#include    "graph.h"
#include    "../lib/raygui/src/raygui.h"


#define CELLSIZE 25


#include "utils.h"
#define GET_REL_MOUSE() GetScreenToWorld2D(GetMousePosition(), playground_camera)




static float zoom_speed = 0.1f;

struct{
    Vector2 start_drag,stop_drag;
    GateBool dragging;
    Element* selected_elements[MAX_GATES_SIZE];
    size_t selected_size;
} drag_select;


static Vector2 last_mouse_postion;
 
static const pthread_t input_thread;


static Element* selected_gate = NULL;

Camera2D playground_camera = {
    .target =   (Vector2){SCREEN_WIDTH/2,SCREEN_HEIGHT/2},
    .offset =   (Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},
    .zoom   =   1.f
};


Element* gate_select(){
    for(size_t i=0 ; i < elements_size; i++){

        Vector2 worldPosition = GetScreenToWorld2D(GetMousePosition(), playground_camera);

        if(worldPosition.x > elements[i]->g.pos.x
        && worldPosition.y > elements[i]->g.pos.y
        && worldPosition.x < elements[i]->g.pos.x+element_sizes[GET_ELEMENT_SIZE(elements[i])]
        && worldPosition.y < elements[i]->g.pos.y+ ( elements[i]->t == COMPOUND ? COMPOUND_SIZE(*elements[i]->g.max_connection_points) : 
        element_sizes[GET_ELEMENT_SIZE(elements[i])])
        ){
            return elements[i];
        }
    }

    return NULL;
}

static void reset_drag_array(){
    for(int i=0 ; i < drag_select.selected_size; i++)
        drag_select.selected_elements[i]->g.selected = FALSE;
    drag_select.selected_size=0;
}

void elements_in_rect(){
    reset_drag_array();
    for(size_t i=0 ; i < elements_size; i++){
        if(elements[i]->g.pos.x >= drag_select.start_drag.x &&
        elements[i]->g.pos.x+element_sizes[GET_ELEMENT_SIZE(elements[i])] <= drag_select.stop_drag.x  &&
        elements[i]->g.pos.y >= drag_select.start_drag.y &&
        elements[i]->g.pos.y+element_sizes[GET_ELEMENT_SIZE(elements[i])] <= drag_select.stop_drag.y
        )
        { drag_select.selected_elements[drag_select.selected_size++]=elements[i]; elements[i]->g.selected=TRUE;}

    }
}


static void add_gate(enum ElementType t){
    if(elements_size < MAX_GATES_SIZE){
        Element* ce = create_element(t, GetScreenToWorld2D((Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},playground_camera), 
        t == COMPOUND ? drag_select.selected_elements : NULL, 
        t == COMPOUND ? drag_select.selected_size   :   0
        ); 
        if(ce != NULL)
            elements[elements_size++]=ce; 
        
    }
}

static void handle_select(Element* clicked){
    if(clicked == NULL || clicked == selected_gate){
        if(selected_gate != NULL){
            selected_gate->g.pos = GetScreenToWorld2D((Vector2){(int)GetMousePosition().x/CELLSIZE*CELLSIZE, (int)GetMousePosition().y/CELLSIZE*CELLSIZE}, playground_camera);
            create_inputs_and_output(selected_gate, selected_gate->g.pos);
            selected_gate->g.selected = FALSE; selected_gate=NULL;
        }
        return;
    } 

    clicked->g.selected = TRUE;
    
    if(selected_gate == NULL) { 
        selected_gate = clicked;
        return;
    } 

    connect_gate(selected_gate, clicked);
    top_sort(elements,elements_size, TRUE);
    clicked->g.selected = selected_gate->g.selected = FALSE;
    clicked = NULL; selected_gate= NULL;
}

static void handle_click(Element* clicked){
        
        if(clicked == NULL){ drag_select.dragging=TRUE;
            reset_drag_array();
            drag_select.start_drag = GET_REL_MOUSE();
            return;
        };
        if(clicked->t == SWITCH){
            Switch* sw = (Switch*) clicked; // Kewl downcast 
            sw->on = sw->on ? FALSE : TRUE;
            clicked->l.compute(&sw->e.l, sw->on);
            if(acyclic(elements, elements_size) == TRUE)
                top_sort(elements, elements_size, TRUE);
        }

}

static void render_gate_drop(){
    if(selected_gate == NULL) return;
    Rectangle element_drop = {
        .width  = element_sizes[GET_ELEMENT_SIZE(selected_gate)], 
        .height = selected_gate->t != COMPOUND ? element_sizes[GET_ELEMENT_SIZE(selected_gate)] : COMPOUND_SIZE(*selected_gate->g.max_connection_points),
        .x  = (int)GetScreenToWorld2D(GetMousePosition(), playground_camera).x/CELLSIZE*CELLSIZE,
        .y  = (int)GetScreenToWorld2D(GetMousePosition(), playground_camera).y/CELLSIZE*CELLSIZE
    };

    DrawRectangleRec(element_drop, GRID_COLOR);
    DrawRectangleLinesEx(element_drop, 2, GRAY);


  
}

void handle_controls(){

    zoom_speed += GetMouseWheelMove() != 0 ? 0.3f : -zoom_speed+0.1f; // zoom acceleration

    float zoom_ammount = GetMouseWheelMove()*zoom_speed;
    playground_camera.zoom = max(playground_camera.zoom+zoom_ammount, 0.5f);

    Vector2 current_mouse_position = GetMousePosition();

    if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
        Vector2 subtract = Vector2Subtract(last_mouse_postion, current_mouse_position);
        playground_camera.target = GetScreenToWorld2D(Vector2Add(subtract, playground_camera.offset),playground_camera);
    }



    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) handle_click(gate_select());
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) handle_select(gate_select());
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){ drag_select.stop_drag = GET_REL_MOUSE();}
    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)){ if(drag_select.dragging) elements_in_rect(); drag_select.dragging = FALSE;} 
    if(IsKeyPressed(KEY_G)){ add_gate(COMPOUND);} 

    if(IsKeyPressed(KEY_D)){
        if(selected_gate){
            delete_element(selected_gate);
            selected_gate = NULL;
        }

        for(size_t i=0; i < drag_select.selected_size; i++)
            delete_element(drag_select.selected_elements[i]);
    };


    last_mouse_postion = current_mouse_position;
}


void render_grid(){
    for(int i=0; i < SCREEN_HEIGHT / CELLSIZE; i++)
        DrawLine(0, i*CELLSIZE, SCREEN_WIDTH, i*CELLSIZE, GRID_COLOR);
    for(int i=0 ; i < SCREEN_WIDTH / CELLSIZE; i++)
        DrawLine(i*CELLSIZE, 0,  i*CELLSIZE, SCREEN_HEIGHT, GRID_COLOR);
    
    if(drag_select.dragging == TRUE)
    DrawRectangleLines(
        drag_select.start_drag.x,
        drag_select.start_drag.y,
        drag_select.stop_drag.x-drag_select.start_drag.x,
        drag_select.stop_drag.y-drag_select.start_drag.y,
        FGR_COLOR
    );
}

void render_gates(){
    for(size_t i=0 ; i < elements_size; i++){

        switch (elements[i]->t)
        {
        case SWITCH:
            Switch *sw = (Switch*) elements[i];
            elements[i]->g.draw_element(elements[i]->t, &elements[i]->g, (GateBool)sw->on);
            break;
        case OUTPUT:
            Output *o = (Output*) elements[i];
            elements[i]->g.draw_element(elements[i]->t, &elements[i]->g, (GateBool)o->powered);
            break;
        default:
            elements[i]->g.draw_element(elements[i]->t, &elements[i]->g);
            break;
        }
        for(size_t j=0 ; j < elements[i]->g.connection_points_size; j++){
            DrawLineBezier(
                elements[i]->g.connection_points[j].corespondence->coords,
                elements[i]->g.connection_points[j].coords,
                2,
                FGR_COLOR
            );
        }
    }
}

void display_creation_buttons(){
    
    for(int i=0 ; i < LAST; i++){
        if (GuiButton((Rectangle){10+60*i, SCREEN_HEIGHT-60, 50, 50}, nameBinds[i])){
            add_gate(i);
        };
    }
    if(GuiButton((Rectangle){SCREEN_WIDTH-60, SCREEN_HEIGHT-60, 50,50}, "#131#")){ 
        if(acyclic(elements, elements_size) == TRUE)
            top_sort(elements, elements_size, TRUE);
        }
}
void start_graphics(){

     

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jvgate"); 
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();

            ClearBackground(BGR_COLOR);
            display_creation_buttons();
            BeginMode2D(playground_camera);
            handle_controls();
            DrawText("JVGATE PLAYGROUND", 10, 10, 12, GRAY);
            render_grid();  
            render_gates();
            render_gate_drop();
        EndDrawing();
    }
    CloseWindow();
}