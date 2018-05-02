#ifndef HUD__H
#define HUD__H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "vector2f.h"

class Hud {
public:
  Hud();
  ~Hud();
  Hud (const Hud&);

  void draw(Vector2f position, int width, int height);
  void draw(Vector2f position, int width, int height, int life,int points, int bulletList, int freeList, bool isGol);
  void draw(Vector2f position, int width, int height, std::string result, int points);
  void writeText(const std::string& msg, int x, int y) const;
private:

  SDL_Renderer * const renderer;
  TTF_Font* font;
  SDL_Rect r;
  Hud& operator=(const Hud&) = delete;

};



#endif
