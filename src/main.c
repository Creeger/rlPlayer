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


static void DrawWindow(float timePlayed, float totLength, float played);


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
        
        float playedSec = GetMusicTimePlayed(music);
        float totLengthSec = GetMusicTimeLength(music);
        timePlayed = playedSec / totLengthSec; 
        if (timePlayed > 1.0f) {
            timePlayed = 1.0f;
            StopMusicStream(music);
        }

        // Skip backwards or forwards in song
        if (IsKeyPressed(KEY_RIGHT)) {
            SeekMusicStream(music, playedSec + 5.0f);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            SeekMusicStream(music, playedSec - 5.0f);
        }
        if (IsKeyPressed(KEY_RIGHT) && IsKeyPressed(KEY_RIGHT_SHIFT)) {
            SeekMusicStream(music, playedSec + 10.0f);
        }
        if (IsKeyPressed(KEY_RIGHT) && IsKeyPressed(KEY_RIGHT_SHIFT)) {
            SeekMusicStream(music, playedSec - 10.0f);
        }


        DrawWindow(timePlayed, totLengthSec, playedSec);
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}


void DrawWindow(float timePlayed, float totLengthSec, float playedSec) {
    BeginDrawing();
        ClearBackground(RAYWHITE);
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
            float barWidth = screenWidth - 30;
            // Progress bar
            DrawRectangle(15, buttonCenter.y - 50, barWidth, 5, GREEN);
            DrawCircle( 15 + (timePlayed * barWidth), buttonCenter.y - 47, 4, BLACK);
            
        }

        char time[128];
        int playedMin = playedSec / 60;
        int playedRem = (int)playedSec % 60;

        int totalMin = totLengthSec / 60;
        int totalRem = (int)totLengthSec % 60;
        sprintf(time, "%02d:%02d / %02d:%02d\n", playedMin, playedRem, totalMin, totalRem);
        DrawText(time, screenWidth - 15 - MeasureText(time, fontSize), buttonCenter.y - 40, fontSize, BLACK);

        EndDrawing();
}
