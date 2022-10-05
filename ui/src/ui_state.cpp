#include "ui/ui_state.h"

using namespace kardeshev;

UIState::Ptr UI::state                         = std::make_shared<UIState>();
SDL_Renderer* UI::render                       = nullptr;
SDL_Window* UI::window                         = nullptr;
AssetHandler::Ptr UI::assets                   = std::make_shared<AssetHandler>();
std::shared_ptr<SDL_Rect> UI::current_viewport = nullptr;
lib::Game::Ptr UI::game                        = nullptr;
util::Logger::Ptr UI::logger                   = nullptr;
SDL_Rect UI::window_size;
bool UI::running = false;
Settings UI::settings;
ScreenList UI::screen_list;

SDL_Rect UI::getRenderSize()
{
  if (UI::current_viewport == nullptr)
  {
    return UI::window_size;
  }
  return *UI::current_viewport;
}
