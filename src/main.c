#include "common.h"
#include "game.h"
#include "renderer.h"
#include "sdl.h"

static inline void renderInfoString(Font* font, Timer* timer, u64* lastTick)
{
  char msString[16], fpsString[16];
  getInfoStrings(msString, fpsString, &timer->lastTick, lastTick);

  sta_renderTextEndsAt(font, msString, &RED, 100.0f, 85.0f, 7.0f, 2.0f);
  sta_renderTextEndsAt(font, fpsString, &RED, 70.0f, 85.0f, 7.0f, 2.0f);

  *lastTick = timer->lastTick;
}

static inline bool shouldUpdate(Timer* timer)
{

  sta_updateTimer(timer);
  static u32 lastUpdated = 0;
  if (timer->lastTick >= lastUpdated + 16)
  {
    lastUpdated = timer->lastTick + 16;
    return true;
  }
  return false;
}

static void updatePlayer(Timer* timer, Map* map, InputState* input)
{
  if (shouldUpdate(timer))
  {
    if (input->keyboardStateDown['a'])
    {
      map->playerA -= 0.1f;
    }
    if (input->keyboardStateDown['d'])
    {
      map->playerA += 0.1f;
    }

    if (input->keyboardStateDown['w'])
    {
      map->playerX += cos(map->playerA) * 0.1f;
      map->playerY += sin(map->playerA) * 0.1f;
    }
    if (input->keyboardStateDown['s'])
    {
      map->playerX -= cos(map->playerA) * 0.1f;
      map->playerY -= sin(map->playerA) * 0.1f;
    }
  }
}

int main()
{
  const u64 screenWidth  = 1024;
  const u64 screenHeight = 512;

  Arena     arena        = {};
  arena.maxSize          = 1024 * 1024 * 4;
  arena.memory           = (u64)malloc(arena.maxSize);

  Map map                = {};
  initMap(&arena, &map, 16, 16);

  Font     font     = {};
  Renderer renderer = {};
  sta_initRenderer(&renderer, &font, screenWidth, screenHeight);

  InputState input = {};
  initInputState(&input);

  Timer gameTimer = {};
  sta_startTimer(&gameTimer);

  u64 lastTick = 0;

  while (true)
  {

    if (queryInputEvents(&input) || input.keyboardStateRelease[ASCII_ESCAPE])
    {
      break;
    }

    updatePlayer(&gameTimer, &map, &input);

    sta_initNewFrame(WHITE);

    render2DMap(&renderer, &map);
    render3DMap(&renderer, &map);

    renderInfoString(&font, &gameTimer, &lastTick);

    SDL_GL_SwapWindow(renderer.window);
  }
}
