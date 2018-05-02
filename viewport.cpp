#include <sstream>
#include "viewport.h"
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() :
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")),
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL)
{}

void Viewport::setObjectToTrack(const Drawable *obj) {
  objectToTrack = obj;
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
    int ticks = Clock::getInstance().getTicks();
    int second = ticks/1000;
    int fps = 0;
    if(second > 0){
        fps = Clock::getInstance().getFrames() / second;
    }
    std::stringstream ss;
    ss << fps;
  IoMod::getInstance().
    writeText("fps: " + ss.str(), 30, 30);
    IoMod::getInstance().writeText("trcking: " + objectToTrack->getName(), 30, 50);

  string username = gdata.getXmlStr("username");
  SDL_Color textColor({0xff, 0, 0, 0});
  textColor.r = gdata.getXmlInt("textColor/red");
  textColor.g = gdata.getXmlInt("textColor/green");
   textColor.b = gdata.getXmlInt("textColor/blue");
   textColor.a = gdata.getXmlInt("textColor/alpha");
  IoMod::getInstance().
  writeText("username: " + username, 30, viewHeight - 30, textColor);
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
