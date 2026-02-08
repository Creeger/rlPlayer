#include "playlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct playListList* createPlaylist(char* playlistName) {
    struct playListList *playlist = malloc(sizeof(struct playListList));
    if (!playlist) { perror("malloc of creating playlist failed\n"); return NULL; }

    playlist->name = strdup(playlistName);
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->length = 0;
    return playlist;
}

// Searches the playlist for song
struct playListNode* listSearch (struct playListList *playList, char* searchkey) {
    struct playListNode *x = playList->head;
    while (x != NULL && strcmp(x->songName, searchkey) != 0) {
        x = x->next;
    }
    return x;
}

// Prepends a song
void prependSong(struct playListList *playList, char* songName) {
    if (songName == NULL) {
        perror("(prependSong): songName is empty");
    }
    struct playListNode *x = malloc(sizeof(struct playListNode));
    if (!x) { perror("malloc of prepending new song failed\n"); return; }

    x->next = playList->head;
    x->prev = NULL;
    x->songName = strdup(songName);
    playList->length++;

  
    if (playList->head != NULL) {
        playList->head->prev = x;
    } else {
        playList->tail = x;
    }
    playList->head = x;
}

// Inserts song in the middle of playlist, i.e. insert y comes before x
void insertSong(struct playListList *playList, struct playListNode *y, char* songName) {
    struct playListNode *x = malloc(sizeof(struct playListNode));
    if (!x) { perror("malloc of inserting new song failed\n"); return; }

    x->songName = strdup(songName);
    x->next = y->next; 
    x->prev = y;

    if (y->next != NULL) {
        y->next->prev = x;
    } else {
        playList->tail = x;
    }

    y->next = x;
    playList->length++;
}

void deleteSong(struct playListList *playList, struct playListNode *x) {
    if (x->prev != NULL) {
        x->prev->next = x->next;
    } else {
        playList->head = x->next;    
    }
    if (x->next != NULL) {
        x->next->prev = x->next;
    } 
    playList->length--;
    free(x->songName);
    free(x);
}


