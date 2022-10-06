#include "ui/ui_state.h"
#include "ui/screen.h"

using namespace kardeshev;
using namespace ui;

UIState::Ptr UI::state                         = std::make_shared<UIState>();
Render::Ptr UI::render                         = nullptr;
SDL_Window* UI::window                         = nullptr;
AssetHandler::Ptr UI::assets                   = std::make_shared<AssetHandler>();
lib::Game::Ptr UI::game                        = nullptr;
SDL_Rect UI::window_size;
bool UI::running = false;
Settings UI::settings;
ScreenList UI::screen_list;

SDL_Rect UI::getRenderSize()
{
  if (UI::render->getViewport() == nullptr)
  {
    return UI::window_size;
  }
  return *UI::render->getViewport();
}

void ScreenStack::push(const Screen::Ptr& screen) {
  m_stack.push(screen);
  screen->resize();
}

Screen::Ptr ScreenStack::pop()
{
    Screen::Ptr top = m_stack.top();
    m_stack.pop();
    m_stack.top()->resize();
    return top;
  }
