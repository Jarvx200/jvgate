#include    "raylib.h"
#include    "settings.h"


void initialize_window(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "jvgate"); 
    SetTargetFPS(60);


    while(!WindowShouldClose()){
          BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
    CloseWindow();
}