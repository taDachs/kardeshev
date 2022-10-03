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
    throw SDLException("TTF init failed");
  }
  if (IMG_Init(IMG_INIT_PNG) < 0)
  {
    throw SDLException("Image init failed");
  }
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw SDLException("SDL init failed");
  }
}

void GameWindow::init()
{
  m_window = SDL_CreateWindow("Kardeshev",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              m_window_width,
                              m_window_height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (m_window == nullptr)
  {
    throw SDLException("Window creation failed");
  }


  UI::render =
    SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void GameWindow::setupViews()
{
  m_main_view_viewport = std::make_shared<SDL_Rect>();
  m_galaxy_view        = std::make_shared<GalaxyView>();
  m_galaxy_view->setViewport(m_main_view_viewport);
  m_system_view = std::make_shared<SystemView>();
  m_system_view->setViewport(m_main_view_viewport);

  m_sidebar_viewport = std::make_shared<SDL_Rect>();
  m_galaxy_info_view = std::make_shared<GalaxyInfoView>();
  m_galaxy_info_view->setViewport(m_sidebar_viewport);

  m_bottom_bar_viewport = std::make_shared<SDL_Rect>();
  m_bottom_bar_view     = std::make_shared<PlanetInfoView>();
  m_bottom_bar_view->setViewport(m_bottom_bar_viewport);

  setViewports();
}

void GameWindow::setViewports()
{
  m_sidebar_viewport->x = 0;
  m_sidebar_viewport->y = 0;
  m_sidebar_viewport->w = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_sidebar_viewport->h = m_window_height;

  m_main_view_viewport->x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->y = 0;
  m_main_view_viewport->w =
    m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->h =
    m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);

  m_bottom_bar_viewport->x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->y =
    m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
  m_bottom_bar_viewport->w =
    m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->h = static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
}

void GameWindow::kill()
{
  if (m_scan_lines && m_scan_line_texture != nullptr)
  {
    SDL_DestroyTexture(m_scan_line_texture);
  }
  if (m_color_filter && m_color_filter_tex != nullptr)
  {
    SDL_DestroyTexture(m_color_filter_tex);
  }
  SDL_DestroyRenderer(UI::render);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void GameWindow::renderScreen()
{
  if (UI::state->focused_system == nullptr)
  {
    m_galaxy_view->update();
    m_galaxy_view->draw();
  }
  else
  {
    m_system_view->update();
    m_system_view->draw();
  }

  m_galaxy_info_view->update();
  m_galaxy_info_view->draw();
  m_bottom_bar_view->update();
  m_bottom_bar_view->draw();
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
  int texture_height = UI::window_size.h + m_scan_line_thickness + m_distance_between_scan_lines;
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

void GameWindow::display()
{
  SDL_Event e;
  long framestart;
  long framedelay   = 33; // 30 fps
  UI::window_size.w = m_window_width;
  UI::window_size.h = m_window_height;
  generateScanLineTex();
  generateColorFilterTex();

  bool quit = false;
  while (!quit)
  {
    framestart = SDL_GetTicks();
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
      else
      {
        if (e.type == SDL_WINDOWEVENT)
        {
          if (e.window.event == SDL_WINDOWEVENT_RESIZED)
          {
            std::cout << "MESSAGE:Resizing window..." << std::endl;
            UI::window_size.x = UI::window_size.y = 0;
            UI::window_size.w                     = e.window.data1;
            UI::window_size.h                     = e.window.data2;
            m_window_width                        = e.window.data1;
            m_window_height                       = e.window.data2;
            setViewports();
            generateScanLineTex();
            generateColorFilterTex();
          }
          break;
        }
        bool handeled;
        if (UI::state->focused_system == nullptr)
        {
          handeled = m_galaxy_view->handleEvent(&e);
        }
        else
        {
          m_system_view->handleEvent(&e);
        }
        if (!handeled)
        {
          handeled = m_galaxy_info_view->handleEvent(&e);
        }
        if (!handeled)
        {
          handeled = m_bottom_bar_view->handleEvent(&e);
        }
      }
    }

    UI::game->step(Duration(1));

    SDL_SetRenderDrawColor(UI::render, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(UI::render);

    renderScreen();

    if (m_scan_lines)
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

    if (m_color_filter)
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
