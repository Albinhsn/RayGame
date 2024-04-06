#ifndef GAME_H
#define GAME_H

#include "arena.h"
#include "common.h"
#include "renderer.h"
#include "vector.h"
struct Enemy
{
  f64 x, y;
  u64 textureId;
};
typedef struct Enemy Enemy;

struct Framebuffer
{
  u64            width, height;
  unsigned char* data;
  f64*           zBuffer;
};
typedef struct Framebuffer Framebuffer;

struct Map
{
  u8*    tiles;
  Enemy* enemies;
  u64    enemyCount;
  f64    playerX;
  f64    playerY;
  f64    playerA;
  f64    fov;
  u8     width;
  u8     height;
};
typedef struct Map Map;
void               initMap(Arena* arena, Map* map, u8 width, u8 height);
void               render3DMap(Renderer* renderer, Map* map);
void               render2DMap(Renderer* renderer, Map* map);

#endif
