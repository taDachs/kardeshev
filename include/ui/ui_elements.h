#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H
#include "ui/render.h"

namespace kardeshev {

class UIElement
{
protected:
  std::shared_ptr<UIState> m_state = nullptr;

public:
  UIElement(std::shared_ptr<UIState> state)
    : m_state(std::move(state))
  {
  }

  virtual bool handleEvent(SDL_Event* e)       = 0;
  virtual void display(Render& renderer) const = 0;
};

class PlanetUI : public UIElement
{
private:
  std::shared_ptr<Planet> m_planet;
  int m_x;
  int m_y;
  int m_total_x;
  int m_total_y;
  bool m_selected     = false;
  double m_zoom_level = 1;


public:
  PlanetUI(std::shared_ptr<UIState> state, std::shared_ptr<Planet> planet)
    : UIElement(std::move(state))
    , m_planet(std::move(planet))
  {
  }
  std::shared_ptr<Planet> getPlanet() const { return m_planet; }
  bool handleEvent(SDL_Event* e) override;
  void display(Render& renderer) const override;
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  void setTotalX(int x) { m_total_x = x; }
  void setTotalY(int y) { m_total_y = y; }
  void setZoomLevel(double zoom_level) { m_zoom_level = zoom_level; }
};

class SystemUI : public UIElement
{
private:
  std::shared_ptr<SolarSystem> m_system;
  int m_x;
  int m_y;
  int m_total_x;
  int m_total_y;
  bool m_selected     = false;
  double m_zoom_level = 1;

public:
  SystemUI(std::shared_ptr<UIState> state, std::shared_ptr<SolarSystem> system)
    : UIElement(std::move(state))
    , m_system(std::move(system))
  {
  }
  std::shared_ptr<SolarSystem> getSystem() const { return m_system; }
  bool handleEvent(SDL_Event* e) override;
  void display(Render& renderer) const override;
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  void setTotalX(int x) { m_total_x = x; }
  void setTotalY(int y) { m_total_y = y; }
  void setZoomLevel(double zoom_level) { m_zoom_level = zoom_level; }
};


}

#endif // !UI_ELEMENTS_H
