#include "ui/window.h"
#include "SDL_image.h"
#include "ui/render.h"
#include "ui/view.h"
#include <SDL_blendmode.h>
#include <SDL_timer.h>

using namespace kardeshev;

void kardeshev::initSDL()
{
  if (TTF_Init() < 0)
  {
    throw TTFException("TTF init failed");
  }
  if (IMG_Init(IMG_INIT_PNG) < 0)
  {
    throw IMGException("Image init failed");
  }
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw SDLException("SDL init failed");
  }
  UI::window = SDL_CreateWindow("Kardeshev",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                UI::window_size.w,
                                UI::window_size.h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (UI::window == nullptr)
  {
    throw SDLException("Window creation failed");
  }


  UI::render =
    SDL_CreateRenderer(UI::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void kardeshev::setupScreens() {
  UI::screen_list.main_screen = std::make_shared<MainScreen>();
  LoadingScreen::Ptr loading_screen = std::make_shared<LoadingScreen>();
  UI::logger->addLogger(std::static_pointer_cast<LoggerOutput>(loading_screen));
  UI::screen_list.loading_screen = loading_screen;
  UI::screen_list.settings_screen = std::make_shared<SettingsScreen>();
  UI::screen_list.main_menu_screen = std::make_shared<MainMenuScreen>();
  UI::state->current_screen = UI::screen_list.loading_screen;
}

void GameWindow::kill()
{
  if (UI::settings.ui_settings.scan_lines && m_scan_line_texture != nullptr)
  {
    SDL_DestroyTexture(m_scan_line_texture);
  }
  if (UI::settings.ui_settings.color_filter && m_color_filter_tex != nullptr)
  {
    SDL_DestroyTexture(m_color_filter_tex);
  }
  SDL_DestroyRenderer(UI::render);
  SDL_DestroyWindow(UI::window);
  SDL_Quit();
}

void GameWindow::generateColorFilterTex()
{
  m_color_filter_tex = SDL_CreateTexture(UI::render,
                                         SDL_PIXELFORMAT_RGBA32,
                                         SDL_TEXTUREACCESS_TARGET,
                                         UI::window_size.w,
                                         UI::window_size.h);
  if (m_color_filter_tex == nullptr)
  {
    throw SDLException("Creation of color filter texture failed");
  }
  if (SDL_SetTextureBlendMode(m_color_filter_tex, SDL_BLENDMODE_MOD))
  {
    throw SDLException("Failed setting texture blend mode");
  }

  if (SDL_SetRenderTarget(UI::render, m_color_filter_tex))
  {
    throw SDLException("Failed setting render to color filters texture");
  }

  SDL_SetRenderDrawColor(UI::render,
                         m_color_filter_color.r,
                         m_color_filter_color.g,
                         m_color_filter_color.b,
                         m_color_filter_alpha);
  SDL_RenderClear(UI::render);
  SDL_RenderPresent(UI::render);
  if (SDL_SetRenderTarget(UI::render, nullptr))
  {
    throw SDLException("Failed resetting render target from color filters texture");
  }
}

void GameWindow::generateScanLineTex()
{
  int texture_height  = UI::window_size.h + m_scan_line_thickness + m_distance_between_scan_lines;
  m_scan_line_texture = SDL_CreateTexture(UI::render,
                                          SDL_PIXELFORMAT_RGBA32,
                                          SDL_TEXTUREACCESS_TARGET,
                                          UI::window_size.w,
                                          texture_height);
  if (m_scan_line_texture == nullptr)
  {
    throw SDLException("Creation of scan line texture failed");
  }
  if (SDL_SetTextureBlendMode(m_scan_line_texture, SDL_BLENDMODE_BLEND))
  {
    throw SDLException("Failed setting texture blend mode");
  }

  if (SDL_SetRenderTarget(UI::render, m_scan_line_texture))
  {
    throw SDLException("Failed setting render to scan line texture");
  }

  SDL_SetRenderDrawColor(UI::render, 0, 0, 0, 0);
  SDL_RenderClear(UI::render);

  SDL_SetRenderDrawColor(UI::render,
                         m_scan_lines_color.r,
                         m_scan_lines_color.g,
                         m_scan_lines_color.b,
                         m_scan_lines_alpha);
  for (int i = 0; i < texture_height; i += m_distance_between_scan_lines + m_scan_line_thickness)
  {
    for (int j = 0; j < m_scan_line_thickness; j++)
    {
      SDL_RenderDrawLine(UI::render, 0, i + j, UI::window_size.w, i + j);
    }
  }
  SDL_RenderPresent(UI::render);
  if (SDL_SetRenderTarget(UI::render, nullptr))
  {
    throw SDLException("Failed resetting render target from scan line texture");
  }
}

void GameWindow::handleEvents()
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
    {
      UI::running = false;
    }
    else
    {
      if (e.type == SDL_WINDOWEVENT)
      {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          kardeshev::UI::logger->logDebug("Resizing window...");
          UI::window_size.x = UI::window_size.y = 0;
          UI::window_size.w                     = e.window.data1;
          UI::window_size.h                     = e.window.data2;
          UI::state->current_screen->resize();
          generateScanLineTex();
          generateColorFilterTex();
        }
      }
      UI::state->current_screen->handleEvent(&e);
    }
  }
}

void GameWindow::display()
{
  long framestart;
  long framedelay = 33; // 30 fps
  generateScanLineTex();
  generateColorFilterTex();
  UI::state->current_screen->resize();

  UI::running = true;
  while (UI::running)
  {
    framestart = SDL_GetTicks();

    handleEvents();

    if (m_last_screen != UI::state->current_screen) {
      m_last_screen = UI::state->current_screen;
      UI::state->current_screen->resize();
    }

    SDL_SetRenderDrawColor(UI::render, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(UI::render);

    UI::state->current_screen->draw();

    if (UI::settings.ui_settings.scan_lines)
    {
      SDL_Rect src;
      src.x = 0;
      src.y = static_cast<int>(static_cast<int>(m_scan_line_step * m_scan_line_speed) %
                               (m_scan_line_thickness + m_distance_between_scan_lines));
      src.w = UI::window_size.w;
      src.h = UI::window_size.h;

      SDL_RenderCopy(UI::render, m_scan_line_texture, &src, nullptr);
      m_scan_line_step++;
    }

    if (UI::settings.ui_settings.color_filter)
    {
      SDL_RenderCopy(UI::render, m_color_filter_tex, nullptr, nullptr);
    }
    SDL_RenderPresent(UI::render);

    long frametime = SDL_GetTicks() - framestart;
    if (framedelay > frametime)
    {
      SDL_Delay(framedelay - frametime);
    }
  }
}
