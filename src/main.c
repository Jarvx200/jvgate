#include    <stdio.h>
#include    "gates.h"
#include    "raylib.h"


int main(){


        printf("Starting program\n");
    fflush(stdout);
    
    printf("About to call InitWindow\n");
    fflush(stdout);
    
    InitWindow(800, 600, "Test");
    
    printf("After InitWindow call\n");
    fflush(stdout);
    
    if (!IsWindowReady()) {
        printf("Window is not ready!\n");
        return -1;
    }
    
    printf("Window is ready, entering main loop\n");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;

    return 0;
}