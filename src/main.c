#include "common.h"
#include "game.h"
#include "logging.h"
#include "renderer.h"
#include "sdl.h"

static inline bool shouldUpdate(Timer* timer)
{

  sta_updateTimer(timer);
  static u32 lastUpdated = 0;
  if (timer->lastTick >= lastUpdated + 300)
  {
    lastUpdated = timer->lastTick + 300;
    return true;
  }
  return false;
}

int main()
{
  Logger logger;
  logger.filePtr         = 0;

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

  while (true)
  {

    if (queryInputEvents(&input) || input.keyboardStateRelease[ASCII_ESCAPE])
    {
      break;
    }

    if (shouldUpdate(&gameTimer))
    {
      map.playerA += 0.1f;
      if (map.playerA >= PI)
      {
        map.playerA = -PI;
      }
    }

    sta_initNewFrame(WHITE);

    render2DMap(&renderer, &map);
    render3DMap(&renderer, &map);

    SDL_GL_SwapWindow(renderer.window);
  }

}
