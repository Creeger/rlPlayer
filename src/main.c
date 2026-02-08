#include "raylib.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "playlist.h"

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
Music music = (Music){0};
float volume = 1.0f;

static void DrawWindow(float timePlayed, float totLength, float played, struct playListNode currentSong);
static struct playListList *lookUpSongs(const char* musicPath);
static void playSong(struct playListNode *currentSong);

static void debugPlaylist(struct playListList *pl, const char *where); 
void printPlayListNode(const struct playListNode *node);


int main(void) {
    InitWindow(screenWidth, screenHeight, "rlPlayer");
    InitAudioDevice();

    struct playListList *playlist = lookUpSongs(musicPath);
    if (playlist == NULL) {
        printf("Could not retireve playlist\n");
        return 1;
    }
    struct playListNode *currentSong = playlist->head;
    playSong(currentSong);

    float timePlayed = 0.0f;

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
        if (IsKeyPressed(KEY_N)) {
            if (currentSong->next) {
                currentSong = currentSong->next;
            } else {
                currentSong = playlist->head;
            }
            pause = false;
            playSong(currentSong);
        }
        if (IsKeyPressed(KEY_B)) {
            if (currentSong->prev) {
                currentSong = currentSong->prev;
            } else {
                currentSong = playlist->tail;
            }
            pause = false;
            playSong(currentSong);
        }
        DrawWindow(timePlayed, totLengthSec, playedSec, *currentSong);
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}


void DrawWindow(float timePlayed, float totLengthSec, float playedSec, struct playListNode currentSong) {
    float barWidth = screenWidth - 30;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (pause) {
        DrawCircle(buttonCenter.x, buttonCenter.y, pauseButtonRadius, BLUE);
        DrawTriangle(
            (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y - triangleSize },  
            (Vector2){ buttonCenter.x - triangleSize * 0.4f, buttonCenter.y + triangleSize }, 
            (Vector2){ buttonCenter.x + triangleSize, buttonCenter.y }, 
            BLACK
        );

        // Progress bar
        DrawRectangle(15, buttonCenter.y - 50, screenWidth - 30, 5, GREEN);
        DrawCircle( 15 + (timePlayed * barWidth), buttonCenter.y - 47, 4, BLACK);
    } else {
        DrawCircle(buttonCenter.x, buttonCenter.y, pauseButtonRadius, RED);
        DrawRectangle(buttonCenter.x + 5, buttonCenter.y - 20 , 10, 40, WHITE);
        DrawRectangle(buttonCenter.x - 15, buttonCenter.y - 20, 10, 40, WHITE);

        // Progress bar
        DrawRectangle(15, buttonCenter.y - 50, barWidth, 5, GREEN);
        DrawCircle(15 + (timePlayed * barWidth), buttonCenter.y - 47, 4, BLACK);
    }

    char time[16];
    int playedMin = playedSec / 60;
    int playedRem = (int)playedSec % 60;

    int totalMin = totLengthSec / 60;
    int totalRem = (int)totLengthSec % 60;
    sprintf(time, "%02d:%02d / %02d:%02d\n", playedMin, playedRem, totalMin, totalRem);
    DrawText(time, screenWidth - 15 - MeasureText(time, fontSize), buttonCenter.y - 40, fontSize, BLACK);

    int tempFontSize = fontSize;
    while(MeasureText(currentSong.songName, fontSize) > barWidth && fontSize > 5) {
        tempFontSize--;
    }

    int songTitleLen = MeasureText(currentSong.songName, tempFontSize);
    DrawText(currentSong.songName, (screenWidth - songTitleLen)/2, screenHeight/2, fontSize, BLACK); 

    EndDrawing();
}


struct playListList *lookUpSongs(const char* musicPath) {
    DIR *musicDir = opendir(musicPath);

    if (musicDir == NULL) {
        perror("opendir failed");
        return NULL;
    }

    struct playListList *playlist = createPlaylist("default");
    if (playlist == NULL) {
        perror("failed to create playlist");
        closedir(musicDir);
        return NULL;
    }
    
    struct dirent *entry; 
    int idx = 0;
    while ((entry = readdir(musicDir)) != NULL) {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
            continue;
        }   
        if (playlist->head == NULL) {
            prependSong(playlist, entry->d_name);
        } else {
            insertSong(playlist, playlist->tail, entry->d_name);
        }
        idx++;
    }

    closedir(musicDir);
    return playlist;
}


void playSong( struct playListNode *currentSong) {
    if (currentSong == NULL) {
        printf("(playNextSong): no song provided\n");
        return;
    }
  
    char songPath[256];
    snprintf(songPath, sizeof(songPath), "%s%s", musicPath, currentSong->songName);
 
    StopMusicStream(music);
    UnloadMusicStream(music);
   
    printf("Playing: %s\n", songPath);

    Music newMusicStream = LoadMusicStream(songPath);
    SetMusicVolume(newMusicStream, volume);
    PlayMusicStream(newMusicStream);
    music = newMusicStream;
}


void debugPlaylist(struct playListList *pl, const char *where) {
    printf("\n[%s]\n", where);
    printf("  head: %p\n", (void*)pl->head);
    printf("  tail: %p\n", (void*)pl->tail);
    printf("  length: %d\n", pl->length);

    struct playListNode *cur = pl->head;
    int i = 0;
    while (cur) {
        printf("    [%d] node=%p prev=%p next=%p name=%s\n",
               i,
               (void*)cur,
               (void*)cur->prev,
               (void*)cur->next,
               cur->songName);
        cur = cur->next;
        i++;
    }
}

void printPlayListNode(const struct playListNode *node) {
    if (!node) {
        printf("playListNode: (NULL)\n");
        return;
    }

    printf("playListNode @ %p\n", (void *)node);
    printf("  songName: %s\n", node->songName);
    printf("  prev: %p\n", (void *)node->prev);
    printf("  next: %p\n", (void *)node->next);

    if (node->prev) {
        printf("    prev->songName: %s\n", node->prev->songName);
    }
    if (node->next) {
        printf("    next->songName: %s\n", node->next->songName);
    }
}
