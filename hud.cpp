#include "hud.h"
#include "gamedata.h"
#include "renderContext.h"
#include "ioMod.h"
#include <sstream>

Hud::Hud() : renderer(RenderContext::getInstance()->getRenderer()),
               font(TTF_OpenFont(Gamedata::getInstance().getXmlStr("font/file").c_str(),
                  Gamedata::getInstance().getXmlInt("font/size"))),r(){

}


Hud::Hud(const Hud& hud):
renderer(hud.renderer),
  font(hud.font),
r(hud.r) { }

Hud::~Hud() {
  TTF_CloseFont(font);
}

void Hud::draw(Vector2f position, int width, int height) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 100 );
  r.x = position[0];
  r.y = position[1];
  r.w = width;
  r.h = height;

    int writeX = position[0] + 100;
    int writeY = position[1] + 20;
    writeText("w : go up", writeX, writeY);
    writeY += 20;
    writeText("s : go down", writeX, writeY);
    writeY += 20;
    writeText("d : go right", writeX, writeY);
    writeY += 20;
    writeText("a : go left", writeX, writeY);
    writeY += 20;
    writeText("If collision : lost a life", writeX, writeY);
    writeY += 20;
    writeText("q : exit the game!", writeX, writeY);
    writeY += 20;
    writeText("g : enter gol state!", writeX, writeY);


  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawRect( renderer, &r );

}

void Hud::draw(Vector2f position, int width, int height, std::string result, int points) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 100 );
  r.x = position[0];
  r.y = position[1];
  r.w = width;
  r.h = height;
  int writeX = position[0] + 100;
  int writeY = position[1] + 50;
  writeText(result, writeX, writeY);
  std::stringstream ss1;
  ss1<<points;
  writeY += 20;
  writeText("you got " + ss1.str() + " points !", writeX, writeY);
  writeY += 20;
  writeText("press 'R' to restart!", writeX, writeY );
  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawRect( renderer, &r );
}

void Hud::draw(Vector2f position, int width, int height, int life, int points, int bulletList, int freeList, bool isGol) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 100 );
  r.x = position[0];
  r.y = position[1];
  r.w = width;
  r.h = height;

    int writeX = position[0] + 10;
    int writeY = position[1] + 10;
    std::stringstream ss1;
    std::stringstream ss2;
    std::stringstream ss3;
    std::stringstream ss4;
    ss1 << life;
    writeText("lifes : " + ss1.str(), writeX,writeY);
    writeY += 20;
    ss4<<points;
    writeText("points : " + ss4.str(), writeX, writeY);
    writeY += 20;
    ss2 << bulletList;
    writeText("bulletList : " + ss2.str(), writeX, writeY);
    writeY += 20;
    ss3 << freeList;
    writeText("freeList : " + ss3.str(), writeX, writeY);
    writeY += 20;
    if(isGol) {
      writeText("In god state", writeX, writeY);
    }else {
      writeText("Not in god state", writeX, writeY);
    }
    writeY += 20;
    writeText("pree 'G' to change god state", writeX, writeY);


  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawRect( renderer, &r );

}

void Hud::writeText(const std::string& msg, int x, int y) const{
  SDL_Color textColor({0xff, 0, 0, 0});
  textColor.r = Gamedata::getInstance().getXmlInt("textColor/red");
  textColor.g = Gamedata::getInstance().getXmlInt("textColor/green");
  textColor.b = Gamedata::getInstance().getXmlInt("textColor/blue");
  textColor.a = Gamedata::getInstance().getXmlInt("textColor/alpha");
  SDL_Surface* surface =
    TTF_RenderText_Solid(font, msg.c_str(), textColor);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {x, y, textWidth, textHeight};
  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
