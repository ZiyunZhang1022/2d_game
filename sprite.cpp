#include <cmath>
#include <random>
#include <functional>
#include "sprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

Vector2f Sprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Vector2f Sprite::makeStartLoc(int x, int y) const {
  float newvx = Gamedata::getInstance().getRandFloat(x,x+50);;
  float newvy = Gamedata::getInstance().getRandFloat(y-50,y+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}


Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel),
  image( img ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }
Sprite::~Sprite() {if (explosion) delete explosion; }
Sprite::Sprite(const std::string& name) :
  Drawable(name,
           makeStartLoc(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(nullptr)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  image(s.image),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  explosion(s.explosion)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Sprite::explode() {

if ( !explosion ) {
  std::cout<< "Sprite explode" << std::endl;
  Sprite
  sprite(getName(), getPosition(), getVelocity(), image);
  explosion = new ExplodingSprite(sprite);
}
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  if ( explosion ) explosion->draw();
  else image->draw(getX(), getY(), getScale());
}

void Sprite::update(Uint32 ticks) {
  if ( explosion ) {
  explosion->update(ticks);
  if ( explosion->chunkCount() == 0 ) {
    delete explosion;
    explosion = NULL;
  }
  return;
 }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }
}
