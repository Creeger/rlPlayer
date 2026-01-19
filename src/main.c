#include "raylib.h"
#include <stdio.h>

static const int screenHeight = 500;
static const int screenWidth = 550;
static const char *songName = "[Manganime]_Kyoumen_no_Nami.mp3";
static const char *musicPath = "/home/Creeger/Music/";
static bool pause = false; // 0 = play,  1 = pause
static const int pauseButtonRadius = 30;
static const Vector2 buttonCenter = { (float)screenWidth/2, screenHeight - (float)screenHeight/5 };
static const float triangleSize = pauseButtonRadius * 0.8f;


int main(void) {
    InitWindow(screenWidth, screenHeight, "rlPlayer");
    InitAudioDevice();
    char songPath[256];
    snprintf(songPath, sizeof(songPath), "%s%s", musicPath, songName);
    printf("Full path is: %s\n", songPath);

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
        
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
        if (timePlayed > 1.0f) timePlayed = 1.0f;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (pause) {
                DrawCircle( buttonCenter.x, buttonCenter.y, pauseButtonRadius, RED);
                DrawRectangle(buttonCenter.x + 5, buttonCenter.y - 20 , 10, 40, WHITE);
                DrawRectangle(buttonCenter.x - 15, buttonCenter.y - 20, 10, 40, WHITE);

                // Progress bar
                DrawRectangle( 15, buttonCenter.y - 50, screenWidth - 15, 5, GREEN);

            } else {
                DrawCircle( buttonCenter.x, buttonCenter.y, pauseButtonRadius, BLUE);
                DrawTriangle(
                    (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y - triangleSize },  
                    (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y + triangleSize }, 
                    (Vector2){ buttonCenter.x + triangleSize, buttonCenter.y }, 
                    BLACK
                );
            } 
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
