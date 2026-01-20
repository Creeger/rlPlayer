#include "raylib.h"
#include <stdio.h>

static const int screenHeight = 500;
static const int screenWidth = 550;
static const char *songName = "[Manganime]_Kyoumen_no_Nami.mp3";
static const char *musicPath = "/home/Creeger/Music/";
static bool pause = false;
static const int pauseButtonRadius = 30;
static const Vector2 buttonCenter = { (float)screenWidth/2, screenHeight - (float)screenHeight/10 };
static const float triangleSize = pauseButtonRadius * 0.8f;
static Vector2 progButPos = { 10, screenHeight - 15 };
static const int fontSize = 5;


static void DrawWindow(float timePlayed, float totLength);


int main(void) {
    InitWindow(screenWidth, screenHeight, "rlPlayer");
    InitAudioDevice();
    char songPath[256];
    snprintf(songPath, sizeof(songPath), "%s%s", musicPath, songName);
    

    Music music = LoadMusicStream(songPath);
    SetMusicVolume(music, 1.0);
    PlayMusicStream(music);

    float timePlayed = 0.0f;

    float volume = 0.8f;
    SetMusicVolume(music, volume);
    

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);

    if (IsKeyPressed(KEY_R)) {
        StopMusicStream(music);
        PlayMusicStream(music);
        pause = false;
    }
    
       if (IsKeyPressed(KEY_P)) {
            pause = !pause;

            if (pause) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            } 
        }
        
        
        float played = GetMusicTimePlayed(music) / 60;
        float totLength = GetMusicTimeLength(music) / 60;
        timePlayed = played / totLength; 
        if (timePlayed > 1.0f) timePlayed = 1.0f;


        DrawWindow(timePlayed, totLength);
        }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}


void DrawWindow(float timePlayed, float totLength) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        char time[128];
        sprintf(time, "Duration:  %0.2f:%0.2f\n", timePlayed, totLength);
        if (pause) {
            DrawCircle( buttonCenter.x, buttonCenter.y, pauseButtonRadius, RED);
            DrawRectangle(buttonCenter.x + 5, buttonCenter.y - 20 , 10, 40, WHITE);
            DrawRectangle(buttonCenter.x - 15, buttonCenter.y - 20, 10, 40, WHITE);

            // Progress bar
            DrawRectangle(15, buttonCenter.y - 50, screenWidth - 30, 5, GREEN);
            DrawCircle( 15 + (timePlayed * 400.0f), buttonCenter.y - 47, 4, BLACK);
           

        } else {
            DrawCircle( buttonCenter.x, buttonCenter.y, pauseButtonRadius, BLUE);
            DrawTriangle(
                (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y - triangleSize },  
                (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y + triangleSize }, 
                (Vector2){ buttonCenter.x + triangleSize, buttonCenter.y }, 
                BLACK
            );

            // Progress bar
            DrawRectangle(15, buttonCenter.y - 50, screenWidth - 30, 5, GREEN);
            DrawCircle( 15 + (timePlayed * 400.0f), buttonCenter.y - 47, 4, BLACK);
            
        }
        DrawText(time, screenWidth - 15 - MeasureText(time, fontSize), buttonCenter.y - 40, fontSize, BLACK);

        EndDrawing();
}
