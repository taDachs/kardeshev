#include "ui/window.h"
#include "SDL_image.h"
#include "ui/render.h"
#include "ui/view.h"
#include <SDL_blendmode.h>
#include <SDL_timer.h>

using namespace kardeshev;
using namespace ui;

void kardeshev::ui::setupScreens()
{
  UI::screen_list.main_screen       = std::make_shared<MainScreen>();
  LoadingScreen::Ptr loading_screen = std::make_shared<LoadingScreen>();
  UI::logger->addLogger(std::static_pointer_cast<util::LoggerOutput>(loading_screen));
  UI::screen_list.loading_screen   = loading_screen;
  UI::screen_list.settings_screen  = std::make_shared<SettingsScreen>();
  UI::screen_list.main_menu_screen = std::make_shared<MainMenuScreen>();
  UI::state->current_screen        = UI::screen_list.loading_screen;
}

void GameWindow::kill()
{
  if (m_scan_lines_tex != nullptr)
  {
    m_scan_lines_tex->closeTexture();
  }
  if (m_color_filter_tex != nullptr)
  {
    m_color_filter_tex->closeTexture();
  }
  SDL_DestroyWindow(UI::window);
  SDL_Quit();
}

void GameWindow::generateColorFilterTex()
{
  m_color_filter_tex =
    std::make_shared<Texture>(UI::render->createBlankTexture(UI::window_size.w, UI::window_size.h));
  m_color_filter_tex->setTextureBlendMode(SDL_BLENDMODE_MOD);

  UI::render->setTarget(*m_color_filter_tex);

  UI::render->setColor(
    {m_color_filter_color.a, m_color_filter_color.g, m_color_filter_color.b, m_color_filter_alpha});
  UI::render->clear();
  UI::render->renderPresent();
  UI::render->resetTarget();
}

void GameWindow::generateScanLineTex()
{
  int texture_height = UI::window_size.h + m_scan_lines_thickness + m_distance_between_scan_lines;
  m_scan_lines_tex =
    std::make_shared<Texture>(UI::render->createBlankTexture(UI::window_size.w, texture_height));
  m_scan_lines_tex->setTextureBlendMode(SDL_BLENDMODE_BLEND);
  UI::render->setTarget(*m_scan_lines_tex);

  UI::render->setColor(TRANSPARENT);

  UI::render->clear();
  UI::render->setColor(
    {m_scan_lines_color.r, m_scan_lines_color.g, m_scan_lines_color.b, m_scan_lines_alpha});


  for (int i = 0; i < texture_height; i += m_distance_between_scan_lines + m_scan_lines_thickness)
  {
    for (int j = 0; j < m_scan_lines_thickness; j++)
    {
      UI::render->drawLine(0, i + j, UI::window_size.w, i + j);
    }
  }
  UI::render->renderPresent();
  UI::render->resetTarget();
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
          UI::logger->logDebug("Resizing window...");
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

    if (m_last_screen != UI::state->current_screen)
    {
      m_last_screen = UI::state->current_screen;
      UI::state->current_screen->resize();
    }

    UI::render->setColor(BLACK);
    UI::render->clear();

    UI::state->current_screen->draw();

    if (UI::settings.ui_settings.scan_lines)
    {
      SDL_Rect src;
      src.x = 0;
      src.y = static_cast<int>(static_cast<int>(m_scan_lines_step * m_scan_lines_speed) %
                               (m_scan_lines_thickness + m_distance_between_scan_lines));
      src.w = UI::window_size.w;
      src.h = UI::window_size.h;

      UI::render->copyTexture(*m_scan_lines_tex, &src, nullptr);
      m_scan_lines_step++;
    }

    if (UI::settings.ui_settings.color_filter)
    {
      UI::render->copyTexture(*m_color_filter_tex, nullptr, nullptr);
    }
    UI::render->renderPresent();

    long frametime = SDL_GetTicks() - framestart;
    if (framedelay > frametime)
    {
      SDL_Delay(framedelay - frametime);
    }
  }
}
