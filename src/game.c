#include "game.h"
#include "renderer.h"
#include <math.h>

inline f64 getDistance(f64 x0, f64 y0, f64 x1, f64 y1)
{

  f64 xDiff = x0 - x1;
  f64 yDiff = y0 - y1;
  return sqrt(xDiff * xDiff + yDiff * yDiff);
}

static inline f64 normalizeExpected(f64 x, f64 dim)
{
  return (x / dim) * 200.0f - 100.0f;
}

f64 playerX;
f64 playerY;

int cmpEnemy(const void* a_, const void* b_)
{
  Enemy* a = (Enemy*)a_;
  Enemy* b = (Enemy*)b_;

  return getDistance(playerX, playerY, b->x, b->y) - getDistance(playerX, playerY, a->x, a->y);
}

#define COORDINATE_TO_INDEX_2D(x, y, width) (((y) * width) + (x))

static Color colors[7] = {CYAN, GREEN, RED, YELLOW, BLUE, PURPLE, WHITE};

inline f64   degreesToRadians(f64 degrees)
{
  return degrees * PI / 180.0f;
}

void initMap(Arena* arena, Map* map, u8 width, u8 height)
{
  map->height         = height;
  map->width          = width;

  map->playerX        = 3.456f;
  map->playerY        = 2.345f;
  map->playerA        = 1.523f;
  map->fov            = PI / 3.0f;

  u64        mapSize  = map->height * map->width;
  const char mapstr[] = "0000222222220000"
                        "1              0"
                        "1      11111   0"
                        "1     0        0"
                        "0     0  1110000"
                        "0     3        0"
                        "0   10000      0"
                        "0   3   11100  0"
                        "5   4   0      0"
                        "5   4   1  00000"
                        "0       1      0"
                        "2       1      0"
                        "0       0      0"
                        "0 0000000      0"
                        "0              0"
                        "0002222222200000";
  map->tiles          = ArenaPushArray(arena, u8, mapSize);
  for (u64 y = 0; y < height; y++)
  {
    for (u64 x = 0; x < width; x++)
    {
      u64 idx         = COORDINATE_TO_INDEX_2D(x, y, width);
      map->tiles[idx] = mapstr[idx];
    }
  }

  map->enemyCount   = 4;
  map->enemies      = ArenaPushArray(arena, Enemy, map->enemyCount);
  Enemy* enemy0     = &map->enemies[0];
  enemy0->x         = 1.834;
  enemy0->y         = 8.765;
  enemy0->textureId = 0;

  Enemy* enemy1     = &map->enemies[1];
  enemy1->x         = 5.323;
  enemy1->y         = 5.365;
  enemy1->textureId = 1;

  Enemy* enemy2     = &map->enemies[2];
  enemy2->x         = 4.123;
  enemy2->y         = 10.265;
  enemy2->textureId = 1;

  Enemy* enemy3     = &map->enemies[3];
  enemy3->x         = 3.023;
  enemy3->y         = 3.812;
  enemy3->textureId = 2;
}

inline f64 getDiff(f64 hitX, f64 hitY)
{
  f64 xDiff    = (hitX - (u64)hitX);
  f64 yDiff    = (hitY - (u64)hitY);

  f64 oneXDiff = 1 - xDiff;
  f64 oneYDiff = 1 - yDiff;

  f64 maxX     = MAX(xDiff, oneXDiff);
  f64 maxY     = MAX(yDiff, oneYDiff);

  return maxX > maxY ? yDiff : xDiff;
}
void render3DMap(Renderer* renderer, Map* map)
{
  f64 fovStep           = map->fov / 512.0f;
  i64 screenWidth       = 1024 / 2;
  i64 halvedScreenWidth = screenWidth / 2;
  for (i64 i = -halvedScreenWidth; i < halvedScreenWidth; i++)
  {
    f64 r       = map->playerA + i * fovStep;
    f64 step    = 0.1f;
    f64 maxStep = MAX(map->height, map->width);
    while (step < maxStep)
    {
      f64 currX = map->playerX + step * cos(r);
      f64 currY = map->playerY + step * sin(r);
      u64 tileX = currX;
      u64 tileY = currY;

      u8  tile  = map->tiles[COORDINATE_TO_INDEX_2D(tileX, tileY, map->width)];
      if (tile != ' ')
      {
        f64 heightScale = cos(r - map->playerA) * (1 - step / MAX(map->height, map->width)) * 0.25f;

        f64 x           = ((f64)i / (f64)halvedScreenWidth) * 50.0f + 50.0f;

        f64 width       = 100.0f / 512.0f;
        f64 height      = 200.0f * heightScale;
        f64 z           = getDistance(currX, currY, map->playerX, map->playerY);

        u32 splitBy     = 512;
        u32 splitIdx    = (u64)((f64)(getDiff(currX, currY)) * splitBy);

        sta_renderTextureTilePartOfCol(renderer, x, 0, z, width, height, TEXTURE_WALLS, tile - '0', splitBy, splitIdx);
        break;
      }
      step += 0.05f;
    }
  }

  u64    enemyCount = map->enemyCount;
  Enemy* enemies    = map->enemies;
  for (u64 i = 0; i < enemyCount; i++)
  {
    Enemy* enemy          = &enemies[i];

    f64    xDiff          = enemy->x - map->playerX;
    f64    yDiff          = enemy->y - map->playerY;

    f64    enemyDirection = atan2(yDiff, xDiff);
    while (enemyDirection - map->playerA > PI)
    {
      enemyDirection -= PI * 0.5f;
    }
    while (enemyDirection - map->playerA < -PI)
    {
      enemyDirection += PI * 0.5f;
    }

    f64 z       = getDistance(enemy->x, enemy->y, map->playerX, map->playerY);
    f64 height  = 100.0f / z;
    f64 dirDiff = enemyDirection - map->playerA;

    f64 centerX = 50.0f;
    f64 x       = centerX + (dirDiff / map->fov) * 100.0f;
    f64 centerY = 0.0f;

    f64 y       = centerY;
    f64 width   = height / 2;
    sta_renderTextureTile(renderer, x, y, z, width, height, TEXTURE_MONSTERS, i);
  }
}

void render2DMap(Renderer* renderer, Map* map)
{

  sta_renderQuad(renderer->quadProgramId, renderer->quadVertexId, renderer->quadBufferId, &WHITE, -50.0, 0.0f, 1.0f, 100.0f, 100.0f);
  u64 height     = map->height;
  u64 width      = map->width;

  f64 tileWidth  = 50.0f / width;
  f64 tileHeight = 100.0f / height;
  for (u64 y = 0; y < height; y++)
  {
    for (u64 x = 0; x < width; x++)
    {
      u64 coordIdx = COORDINATE_TO_INDEX_2D(x, y, width);
      if (map->tiles[coordIdx] != ' ')
      {
        f64 imageX = (normalizeExpected(x, width) - 100.0f) / 2 + tileWidth;
        f64 imageY = -normalizeExpected(y, height) - tileHeight;
        sta_renderTextureTile(renderer, imageX, imageY, 0.0f, tileWidth, tileHeight, TEXTURE_WALLS, map->tiles[coordIdx] - '0');
      }
    }
  }

  // draw player
  f64 playerX = (normalizeExpected(map->playerX, width) - 100.0f) / 2;
  f64 playerY = -normalizeExpected(map->playerY, height);
  f64 dim     = 1;

  sta_renderQuad(renderer->quadProgramId, renderer->quadVertexId, renderer->quadBufferId, &GRAY, playerX, playerY, 0.0f, dim, dim);

  // draw player fov
  f64 fovStep = map->fov / 512.0f;
  for (i64 i = -256; i < 256; i++)
  {
    f64 r    = map->playerA + i * fovStep;
    f64 step = 0.1f;
    while (true)
    {
      f32 x     = map->playerX + step * cos(r);
      f32 y     = map->playerY + step * sin(r);
      u64 tileX = x;
      u64 tileY = y;
      if (map->tiles[COORDINATE_TO_INDEX_2D(tileX, tileY, map->width)] != ' ')
      {
        Vec2f32 start = {};
        start.x       = (normalizeExpected(map->playerX, map->width) - 100.0f) / 2;
        start.y       = -normalizeExpected(map->playerY, map->height);
        Vec2f32 end   = {};
        end.x         = (normalizeExpected(x, map->width) - 100.0f) / 2;
        end.y         = -normalizeExpected(y, map->height);
        sta_renderLine(renderer->lineProgramId, renderer->lineVertexId, renderer->lineBufferId, start, end, 1, &GRAY);
        break;
      }
      step += 0.05f;
    }
  }

  // draw enemys
  u64    enemyCount = map->enemyCount;
  Enemy* enemies    = map->enemies;
  for (u64 i = 0; i < enemyCount; i++)
  {
    f64 enemyX = (normalizeExpected(enemies[i].x, map->width) - 100.0f) / 2;
    f64 enemyY = -normalizeExpected(enemies[i].y, map->height);

    f64 dim    = 1;

    sta_renderQuad(renderer->quadProgramId, renderer->quadVertexId, renderer->quadBufferId, &RED, enemyX, enemyY, 0.0f, dim, dim);
  }
}
