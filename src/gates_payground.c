#include    <pthread.h>
#include    <stdio.h>
#include    <stdint.h>
#include    <malloc.h>

#include    "raylib.h"
#include    "raymath.h"
#include    "settings.h"
#include    "elements.h"
#include    "logic_elements.h"
#include    "graph.h"

#define MAX_GATES_SIZE 1024

#define CELLSIZE 25


#define max(a,b) ((a) > (b) ? a : b)

static const Color GRID_COLOR = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 32
};

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
        && worldPosition.x < elements[i]->g.pos.x+100
        && worldPosition.y < elements[i]->g.pos.y+100
        ){
            return elements[i];
        }
    }

    return NULL;
}


static void add_gate(enum ElementType t){
    if(elements_size < MAX_GATES_SIZE)
    elements[elements_size++] = create_element(t, GetScreenToWorld2D(GetMousePosition(), playground_camera)); 
}

static void handle_controls(){

    zoom_speed += GetMouseWheelMove() != 0 ? 0.3f : -zoom_speed+0.1f; // zoom acceleration

    float zoom_ammount = GetMouseWheelMove()*zoom_speed;
    playground_camera.zoom = max(playground_camera.zoom+zoom_ammount, 1.0f);

    Vector2 current_mouse_position = GetMousePosition();

    if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)){
        Vector2 subtract = Vector2Subtract(last_mouse_postion, current_mouse_position);
        playground_camera.target = GetScreenToWorld2D(Vector2Add(subtract, playground_camera.offset),playground_camera);
    }


    if(IsKeyReleased(KEY_G)) add_gate(SWITCH);
    if(IsKeyReleased(KEY_H)) add_gate(NOT);
    if(IsKeyReleased(KEY_J)) add_gate(OUTPUT);
    if(IsKeyReleased(KEY_K)) top_sort(elements, elements_size);

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ //Extract logic, decouple drawin/handling from general prgogram
        Element* clicked = gate_select();


        if(clicked != NULL){
            if(clicked->t == SWITCH){
                Switch* sw = (Switch*) clicked; // Kewl downcast 
                sw->on = sw->on ? FALSE : TRUE;
                clicked->l.compute(&sw->e.l, sw->on);
            }
            clicked->g.selected = TRUE;
            if(selected_gate == NULL)
                selected_gate = clicked;
            else {
                if(clicked != selected_gate){
                connect_gate(selected_gate, clicked);
                clicked->g.selected = selected_gate->g.selected = FALSE;
                clicked = NULL; selected_gate= NULL;
                }
            }
        } else { 
            if(selected_gate != NULL)
                {
                    selected_gate->g.pos = GetScreenToWorld2D(GetMousePosition(), playground_camera);
                    create_inputs_and_output(selected_gate, selected_gate->g.pos);
                    selected_gate->g.selected = FALSE; selected_gate=NULL;
                }
        }
    }



    last_mouse_postion = current_mouse_position;
}


void render_grid(){
    for(int i=0 ; i < SCREEN_HEIGHT / CELLSIZE; i++)
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
                BLACK
            );


        }
        
            
    }
}

void start_graphics(){

    
     

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jvgate"); 
    SetTargetFPS(60);


    while(!WindowShouldClose()){
        BeginDrawing();
            BeginMode2D(playground_camera);
            ClearBackground(RAYWHITE);
            DrawText("JVGATE PLAYGROUND", 10, 10, 12, GRAY);
            render_grid(); 
            render_gates();

            handle_controls();
        EndDrawing();
    }
    CloseWindow();
}