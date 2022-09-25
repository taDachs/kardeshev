#include "ui/window.h"

using namespace kardeshev;

void GameWindow::init()
{
  if (TTF_Init() < 0)
  {
    std::cout << "TTF init failed" << std::endl;
    throw SDLException();
  }

  Fonts::loadFonts();

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "SDL init failed" << std::endl;
    throw SDLException();
  }

  m_window = SDL_CreateWindow("Kardeshev",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              m_window_width,
                              m_window_height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (m_window == nullptr)
  {
    std::cout << "Window creation failed" << std::endl;
    throw SDLException();
  }


  m_renderer =
    SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (m_renderer == nullptr)
  {
    std::cout << "sidebar renderer creation failed" << std::endl;
    throw SDLException();
  }

  m_sidebar_viewport = std::make_shared<SDL_Rect>();
  m_sidebar_render   = std::make_shared<Render>(m_renderer, m_sidebar_viewport);

  m_main_view_viewport = std::make_shared<SDL_Rect>();
  m_main_view_render   = std::make_shared<Render>(m_renderer, m_main_view_viewport);

  m_bottom_bar_viewport = std::make_shared<SDL_Rect>();
  m_bottom_bar_render   = std::make_shared<Render>(m_renderer, m_bottom_bar_viewport);
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
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void GameWindow::display()
{
  SDL_Event e;
  bool quit = false;
  while (!quit)
  {
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
            m_window_width  = e.window.data1;
            m_window_height = e.window.data2;
            setViewports();
          }
          break;
        }
        if (m_sidebar_render->handleEvent(&e))
        {
          break;
        }
        if (m_main_view_render->handleEvent(&e))
        {
          break;
        }
        if (m_bottom_bar_render->handleEvent(&e))
        {
          break;
        }
      }
    }
    if (m_state->focused_system == nullptr)
    {
      m_main_view_render->setArtist(m_galaxy_view_artist);
    }
    else
    {
      m_main_view_render->setArtist(m_system_view_artist);
    }
    m_game->step(Duration(1));
    SDL_SetRenderDrawColor(m_renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(m_renderer);
    m_sidebar_render->display();
    m_main_view_render->display();
    m_bottom_bar_render->display();
    SDL_RenderPresent(m_renderer);
    /* SDL_Delay(10); */
  }
}
