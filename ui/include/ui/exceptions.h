#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <SDL_error.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
namespace kardeshev {
namespace ui {

struct SDLException : public std::exception
{
  std::string s;
  SDLException()
    : s(SDL_GetError())
  {
  }
  SDLException(const std::string& msg)
    : s("Error: " + msg + ", SDL_GetError(): " + SDL_GetError())
  {
  }
  ~SDLException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};

struct TTFException : public std::exception
{
  std::string s;
  TTFException()
    : s(SDL_GetError())
  {
  }
  TTFException(const std::string& msg)
    : s("Error: " + msg + ", TTF_GetError(): " + TTF_GetError())
  {
  }
  ~TTFException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};


struct IMGException : public std::exception
{
  std::string s;
  IMGException()
    : s(SDL_GetError())
  {
  }
  IMGException(const std::string& msg)
    : s("Error: " + msg + ", IMG_GetError(): " + IMG_GetError())
  {
  }
  ~IMGException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};

} // namespace ui
} // namespace kardeshev
#endif // !EXCEPTIONS_H
