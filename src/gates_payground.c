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




#define MAX_GATES_SIZE 1024

#define CELLSIZE 25


#define max(a,b) ((a) > (b) ? a : b)



static float zoom_speed = 0.1f;

static Vector2 last_mouse_postion;
 

static Element* elements[MAX_GATES_SIZE];
static size_t elements_size = 0;

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
        && worldPosition.y < elements[i]->g.pos.y+element_sizes[GET_ELEMENT_SIZE(elements[i])]
        ){
            return elements[i];
        }
    }

    return NULL;
}


static void add_gate(enum ElementType t){
    if(elements_size < MAX_GATES_SIZE)
    elements[elements_size++] = create_element(t, GetScreenToWorld2D((Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},playground_camera)); 
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
    clicked->g.selected = selected_gate->g.selected = FALSE;
    clicked = NULL; selected_gate= NULL;
}

static void handle_click(Element* clicked){
        if(clicked == NULL) return;
            if(clicked->t == SWITCH){
                Switch* sw = (Switch*) clicked; // Kewl downcast 
                sw->on = sw->on ? FALSE : TRUE;
                clicked->l.compute(&sw->e.l, sw->on);
            }

}

static void render_gate_drop(){
    if(selected_gate == NULL) return;
    Rectangle element_drop = {
        .width  = element_sizes[GET_ELEMENT_SIZE(selected_gate)], 
        .height = element_sizes[GET_ELEMENT_SIZE(selected_gate)],
        .x  = (int)GetScreenToWorld2D(GetMousePosition(), playground_camera).x/CELLSIZE*CELLSIZE,
        .y  = (int)GetScreenToWorld2D(GetMousePosition(), playground_camera).y/CELLSIZE*CELLSIZE
    };

    DrawRectangleRec(element_drop, GRID_COLOR);
    DrawRectangleLinesEx(element_drop, 2, GRAY);
}

static void handle_controls(){

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

    if(IsKeyPressed(KEY_D));


    last_mouse_postion = current_mouse_position;
}


void render_grid(){
    for(int i=0; i < SCREEN_HEIGHT / CELLSIZE; i++)
        DrawLine(0, i*CELLSIZE, SCREEN_WIDTH, i*CELLSIZE, GRID_COLOR);
    for(int i=0 ; i < SCREEN_WIDTH / CELLSIZE; i++)
        DrawLine(i*CELLSIZE, 0,  i*CELLSIZE, SCREEN_HEIGHT, GRID_COLOR);

}

void render_gates(){
    for(size_t i=0 ; i < elements_size; i++){

        switch (elements[i]->t)
        {
        case NOT:
        case NOR:
        case XOR:
        case XNOR:
        case AND:
        case NAND:
        case OR:
            elements[i]->g.draw_element(elements[i]->t, &elements[i]->g);
            break;
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
    if(GuiButton((Rectangle){SCREEN_WIDTH-60, SCREEN_HEIGHT-60, 50,50}, "#131#")){ restore_graph(elements, elements_size); top_sort(elements, elements_size, TRUE);}
}
void start_graphics(){

    
     

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jvgate"); 
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(BGR_COLOR);
            display_creation_buttons();
            BeginMode2D(playground_camera);
            DrawText("JVGATE PLAYGROUND", 10, 10, 12, GRAY);
            render_grid();  
            render_gates();
            render_gate_drop();

            handle_controls();
        EndDrawing();
    }
    CloseWindow();
}