#ifndef TEXT_H
#define TEXT_H

#include "ui/exceptions.h"
#include "ui/texture.h"
#include <SDL_ttf.h>
#include <memory>
#include <string>

namespace kardeshev {
namespace ui {


class Font
{
public:
  const static std::string DEFAULT_FONT_SMALL;
  const static std::string DEFAULT_FONT_MEDIUM;
  const static std::string DEFAULT_FONT_LARGE;

private:
  TTF_Font* m_font;
public:
  Font(TTF_Font* font)
    : m_font(font)
  {
  }

  TTF_Font* getFont() const
  {
    return m_font;
  }

  void closeFont()
  {
    TTF_CloseFont(m_font);
    m_font = nullptr;
  }
};

} // namespace ui
} // namespace kardeshev


#endif // !TEXT_H
