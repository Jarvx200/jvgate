#include    <pthread.h>
#include    <stdio.h>
#include    <stdint.h>
#include    <malloc.h>

#include    "raylib.h"
#include    "raymath.h"
#include    "settings.h"
#include    "gates.h"


#define max(a,b) ((a) > (b) ? a : b)

static float zoom_speed = 0.1f;

static Vector2 last_mouse_postion;
 

static LogicGate* gates[1024];
static size_t gates_size = 0;

static LogicGate* selected_gate = NULL;

Camera2D playground_camera = {
    .target =   (Vector2){0,0},
    .offset =   (Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},
    .zoom   =   1.f
};


LogicGate* gate_select(){
    for(size_t i=0 ; i < gates_size; i++){

        Vector2 worldPosition = GetScreenToWorld2D(GetMousePosition(), playground_camera);

        if(worldPosition.x > gates[i]->g.pos.x
        && worldPosition.y > gates[i]->g.pos.y
        && worldPosition.x < gates[i]->g.pos.x+100
        && worldPosition.y < gates[i]->g.pos.y+100
        ){
            return gates[i];
        }
    }

    return NULL;
}


static void add_gate(){
    gates[gates_size++] = create_gate((int)GetRandomValue(0,6), GetScreenToWorld2D(GetMousePosition(), playground_camera)); 
}

static void handle_controls(){

    zoom_speed += GetMouseWheelMove() != 0 ? 0.3f : -zoom_speed+0.1f; // zoom acceleration

    float zoom_ammount = GetMouseWheelMove()*zoom_speed;
    playground_camera.zoom = max(playground_camera.zoom+zoom_ammount, 1.0f);

    Vector2 current_mouse_position = GetMousePosition();

    if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
        Vector2 subtract = Vector2Subtract(last_mouse_postion, current_mouse_position);
        playground_camera.target = GetScreenToWorld2D(Vector2Add(subtract, playground_camera.offset),playground_camera);
    }


    if(IsKeyReleased(KEY_G)) add_gate();

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        LogicGate* clicked = gate_select();


        if(clicked != NULL){
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
                { selected_gate->g.selected = FALSE; selected_gate=NULL;}
        }
    }



    last_mouse_postion = current_mouse_position;
}

void render_gates(){
    for(size_t i=0 ; i < gates_size; i++){
        gates[i]->g.draw_element(gates[i]->t, &gates[i]->g);
        for(size_t j=0 ; j < gates[i]->g.connection_points_size; j++){
            DrawLineBezier(
                gates[i]->g.connection_points[j].corespondence->coords,
                gates[i]->g.connection_points[j].coords,
                2,
                BLACK
            );

            printf("%f\n", gates[i]->g.connection_points[j].corespondence->coords.x);
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
            DrawText("HELLO WORDL", 10, 10, 12, BLACK);
           
            render_gates();

            handle_controls();
        EndDrawing();
    }
    CloseWindow();
}