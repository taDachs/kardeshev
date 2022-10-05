#include "main.h"
#include "generation/galaxy_generation.h"
#include "generation/name_generation.h"
#include "generation/planet_generation.h"
#include "generation/solar_system_generation.h"
#include "generation/star_generation.h"
#include "lib/buildings.h"
#include "lib/planets.h"
#include "lib/population.h"
#include "lib/solar_systems.h"
#include "lib/stars.h"
#include "ui/assets.h"
#include "ui/entities.h"
#include "ui/render.h"
#include "ui/window.h"
#include "util/logger.h"
#include <iostream>
#include <map>
#include <memory>

int loadAssets(void* data)
{
  kardeshev::UI::logger->logInfo("Loading assets");
  kardeshev::UI::assets->addTokenList("english", "assets/words.txt");
  kardeshev::UI::assets->addTexture("planet_portrait", "assets/test_planet.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture(
    kardeshev::PlanetEntity::PLANET_SPRITE, "assets/planet_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture(
    kardeshev::SystemEntity::SYSTEM_SPRITE, "assets/system_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("orbit_ring", "assets/orbit_ring.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("main_view_border", "assets/main_view_border.png");
  kardeshev::UI::assets->addTexture(
    kardeshev::StarEntity::STAR_SPRITE, "assets/star_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("galaxy_button", "assets/galaxy_button.png", 128, 64, 2);
  kardeshev::UI::assets->addTexture(
    kardeshev::CheckBoxOptionEntity::CHECKBOX_SPRITE, "assets/checkbox.png", 64, 64, 4);
  kardeshev::UI::assets->addTexture("settings_icon", "assets/settings_icon.png", 128, 64, 2);

  kardeshev::UI::logger->logInfo("Setting up generators");
  setupGenerators();

  kardeshev::UI::logger->logInfo("Generating galaxy");
  std::shared_ptr<kardeshev::lib::Galaxy> g = gg->generateGalaxy();
  kardeshev::UI::game->setGalaxy(g);

  kardeshev::lib::ResourceType potatoes("potato_resource", "Potatoes");
  kardeshev::lib::Need food_req(potatoes, 1);
  std::shared_ptr<kardeshev::lib::Building> potato_farm =
    std::make_shared<kardeshev::lib::Farm>(potatoes);
  kardeshev::lib::Species humans("human_species", "Humans", 1.01, {food_req});
  kardeshev::lib::Population pop(humans, {});
  std::shared_ptr<kardeshev::lib::SolarSystem> ss = g->getSystems()[0];
  ss->getPlanets()[0]->addPop(pop);
  ss->getPlanets()[0]->addBuilding(potato_farm);

  kardeshev::UI::logger->logInfo("Finished initialization");
  // kardeshev::UI::state->current_screen = kardeshev::UI::screen_list.main_screen;
  kardeshev::UI::state->current_screen = kardeshev::UI::screen_list.main_menu_screen;

  return 0;
}

int gameThread(void* data)
{
  while (kardeshev::UI::state->current_screen == kardeshev::UI::screen_list.loading_screen)
  {
    // poll until game is done initializing
    // TODO: put the thread to sleep and wake it up you piece of shit, this wastes cpu cycles
    //       and brings us close to the heat death of the universe
  }
  long framedelay = 100;
  while (kardeshev::UI::running)
  {
    if (kardeshev::UI::state->paused)
    {
      continue;
    }
    long framestart = SDL_GetTicks();
    kardeshev::UI::game->step(kardeshev::lib::Duration(1));
    long frametime = SDL_GetTicks() - framestart;
    if (framedelay > frametime)
    {
      SDL_Delay(framedelay - frametime);
    }
  }
  return 0;
}

int displayThread(void* data)
{
  main_window->display();
  kardeshev::UI::logger->logInfo("killing");
  main_window->kill();
  return 0;
}

void setupGenerators()
{
  ng = std::make_shared<kardeshev::generation::TokenListNameGenerator>(
    kardeshev::UI::assets->getTokenList("english"));
  pg  = std::make_shared<kardeshev::generation::NaivePlanetGenerator>(ng, PLANETS);
  sg  = std::make_shared<kardeshev::generation::NaiveStarGenerator>(ng, STARS);
  ssg = std::make_shared<kardeshev::generation::NaiveSolarSystemGenerator>(ng, pg, sg, 3, 10);
  gg  = std::make_shared<kardeshev::generation::NaiveGalaxyGenerator>(ssg, 1000, 1001);
}


int main()
{
  kardeshev::UI::window_size.w = 1920;
  kardeshev::UI::window_size.h = 1080;
  kardeshev::UI::game          = std::make_shared<kardeshev::lib::Game>();

  kardeshev::UI::logger = std::make_shared<kardeshev::util::Logger>();
  kardeshev::UI::logger->addLogger(std::make_shared<kardeshev::util::StdOutLogger>());

  kardeshev::UI::logger->logInfo("Init SDL");
  kardeshev::initSDL();
  kardeshev::setupScreens();
  kardeshev::UI::assets->addFont(kardeshev::Font::DEFAULT_FONT, "assets/kongtext.ttf", 12, 26, 52);

  kardeshev::UI::settings.ui_settings.toggle_options.insert(
    {"Scan Lines", &kardeshev::UI::settings.ui_settings.scan_lines});
  kardeshev::UI::settings.ui_settings.toggle_options.insert(
    {"Color Filter", &kardeshev::UI::settings.ui_settings.color_filter});

  kardeshev::UI::logger->logInfo("Creating window");
  main_window = std::make_shared<kardeshev::GameWindow>();

  SDL_Thread* asset_loader_thread_id = SDL_CreateThread(loadAssets, "asset_loading", nullptr);
  SDL_Thread* game_runner_thread_id  = SDL_CreateThread(gameThread, "game_runner", nullptr);
  SDL_Thread* display_thread_id      = SDL_CreateThread(displayThread, "game_runner", nullptr);

  SDL_WaitThread(asset_loader_thread_id, nullptr);
  SDL_WaitThread(game_runner_thread_id, nullptr);
  SDL_WaitThread(display_thread_id, nullptr);
  return 0;
}
