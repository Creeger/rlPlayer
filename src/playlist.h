#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdio.h>

struct playListNode {
    char *songName;
    struct playListNode *next;
    struct playListNode *prev;
};

struct playListList {
    char *name;
    struct playListNode *head;
    struct playListNode *tail;
    int length;
};

struct playListList* createPlaylist(char* playlistName);
struct playListNode* listSearch(struct playListList *playList, char* searchKey);
void prependSong(struct playListList *playList, char* songName);
void insertSong(struct playListList *playList, struct playListNode *x, char* songName);
void deleteSong(struct playListList *playList, struct playListNode *x);

#endif
