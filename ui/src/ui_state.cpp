#include "ui/ui_state.h"

using namespace kardeshev;

UIState::Ptr UI::state                         = std::make_shared<UIState>();
SDL_Renderer* UI::render                       = nullptr;
AssetHandler::Ptr UI::assets                   = std::make_shared<AssetHandler>();
std::shared_ptr<SDL_Rect> UI::current_viewport = nullptr;
Game::Ptr UI::game                             = nullptr;
SDL_Rect UI::window_size;
Context::Ptr UI::current_context = nullptr;
double UI::zoom_level            = 1;

SDL_Rect UI::getRenderSize() {
  if (UI::current_viewport == nullptr) {
    return UI::window_size;
  }
  return *UI::current_viewport;
}
