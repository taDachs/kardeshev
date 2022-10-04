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

int loadAssets(void *data) {
  kardeshev::UI::logger->logInfo("Loading assets");
  kardeshev::UI::assets->addTokenList("english", "assets/words.txt");
  kardeshev::UI::assets->addTexture(
    "planet_portrait", "assets/test_planet.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture(
    kardeshev::PlanetEntity::PLANET_SPRITE, "assets/planet_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture(
    kardeshev::SystemEntity::SYSTEM_SPRITE, "assets/system_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("orbit_ring", "assets/orbit_ring.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("main_view_border", "assets/main_view_border.png");
  kardeshev::UI::assets->addTexture(
    kardeshev::StarEntity::STAR_SPRITE, "assets/star_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("galaxy_button", "assets/galaxy_button.png", 128, 64, 2);

  kardeshev::UI::logger->logInfo("Setting up generators");
  setupGenerators();

  kardeshev::UI::logger->logInfo("Generating galaxy");
  std::shared_ptr<kardeshev::Galaxy> g = gg->generateGalaxy();
  kardeshev::UI::game->setGalaxy(g);

  kardeshev::ResourceType potatoes("potato_resource", "Potatoes");
  kardeshev::Need food_req(potatoes, 1);
  std::shared_ptr<kardeshev::Building> potato_farm = std::make_shared<kardeshev::Farm>(potatoes);
  kardeshev::Species humans("human_species", "Humans", 1.01, {food_req});
  kardeshev::Population pop(humans, {});
  std::shared_ptr<kardeshev::SolarSystem> ss = g->getSystems()[0];
  ss->getPlanets()[0]->addPop(pop);
  ss->getPlanets()[0]->addBuilding(potato_farm);

  kardeshev::UI::logger->logInfo("Finished initialization");
  kardeshev::UI::done_initializing = true;

  return 0;
}

int gameThread(void* data) {
  while (!kardeshev::UI::done_initializing) {
    // poll until game is done initializing
    // TODO: put the thread to sleep and wake it up you piece of shit, this wastes cpu cycles
    //       and brings us close to the heat death of the universe
  }
  long framedelay = 100;
  while (kardeshev::UI::running) {
    long framestart = SDL_GetTicks();
    kardeshev::UI::game->step(kardeshev::Duration(1));
    long frametime = SDL_GetTicks() - framestart;
    if (framedelay > frametime)
    {
      SDL_Delay(framedelay - frametime);
    }
  }
  return 0;
}

void setupGenerators()
{
  ng  = std::make_shared<kardeshev::TokenListNameGenerator>(kardeshev::UI::assets->getTokenList("english"));
  pg  = std::make_shared<kardeshev::NaivePlanetGenerator>(ng, PLANETS);
  sg  = std::make_shared<kardeshev::NaiveStarGenerator>(ng, STARS);
  ssg = std::make_shared<kardeshev::NaiveSolarSystemGenerator>(ng, pg, sg, 3, 10);
  gg  = std::make_shared<kardeshev::NaiveGalaxyGenerator>(ssg, 1000, 1001);
}

int main()
{
  kardeshev::UI::window_size.w = 1920;
  kardeshev::UI::window_size.h = 1080;
  kardeshev::UI::game = std::make_shared<kardeshev::Game>();;
  kardeshev::UI::logger = std::make_shared<kardeshev::Logger>();
  kardeshev::UI::logger->addLogger(std::make_shared<kardeshev::StdOutLogger>());

  kardeshev::UI::logger->logInfo("Init SDL");
  kardeshev::initSDL();
  kardeshev::UI::assets->addFont(kardeshev::Font::DEFAULT_FONT, "assets/kongtext.ttf", 12, 26, 52);


  kardeshev::UI::logger->logInfo("Creating window");
  auto main_window = std::make_shared<kardeshev::GameWindow>();

  SDL_Thread* asset_loader_thread_id = SDL_CreateThread(loadAssets, "asset_loading", nullptr);
  SDL_Thread* game_runner_thread_id = SDL_CreateThread(gameThread, "game_runner", nullptr);
  main_window->display();

  kardeshev::UI::logger->logInfo("killing");
  main_window->kill();
  SDL_WaitThread(asset_loader_thread_id, nullptr);
  SDL_WaitThread(game_runner_thread_id, nullptr);


  return 0;
}
