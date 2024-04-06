#include "logging.h"
#include "renderer.h"
#include "sdl.h"

int main()
{
  Logger logger;
  logger.filePtr         = 0;

  const u64 screenWidth  = 620;
  const u64 screenHeight = 480;

  Font      font = {};
  Renderer  renderer = {};
  sta_initRenderer(&renderer, &font, screenWidth, screenHeight);

  InputState input = {};
  initInputState(&input);

  while (true)
  {

    if (queryInputEvents(&input))
    {
      break;
    }

    sta_initNewFrame(WHITE);

    sta_renderTextCentered(&font, "Hello Sailor!", &YELLOW, 0, 0, 20.0f, 5.0f);

    SDL_GL_SwapWindow(renderer.window);
  }
}
