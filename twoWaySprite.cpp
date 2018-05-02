#include "twoWaySprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

Vector2f TwoWaySprite::makeVelocity(int vx, int vy) const {
    float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
    float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
    newvx *= [](){ if(rand()%2) return -1; else return 1; }();
    newvy *= [](){ if(rand()%2) return -1; else return 1; }();

    return Vector2f(newvx, newvy);
}
TwoWaySprite::~TwoWaySprite( ) { if (explosion) delete explosion; }

TwoWaySprite::TwoWaySprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))

           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
  Drawable(s),
  images(s.images),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

  TwoWaySprite& TwoWaySprite::operator=(const TwoWaySprite& s) {
    Drawable::operator=(s);
    images = (s.images);
    currentFrame = (s.currentFrame);
    numberOfFrames = ( s.numberOfFrames );
    frameInterval = ( s.frameInterval );
    timeSinceLastFrame = ( s.timeSinceLastFrame );
    worldWidth = ( s.worldWidth );
    worldHeight = ( s.worldHeight );
    return *this;
  }
  void TwoWaySprite::explode() {
  if ( !explosion ) {
    Sprite
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

  void TwoWaySprite::draw() const {
    if ( explosion ) explosion->draw();
    else images[currentFrame]->draw(getX(), getY(), getScale());
  }

  void TwoWaySprite::advanceFrame(Uint32 ticks) {
  	timeSinceLastFrame += ticks;
    int halfNumberFrames = numberOfFrames / 2;
    if(transfer) {
      if (timeSinceLastFrame > frameInterval) {
        currentFrame = (currentFrame+1) % halfNumberFrames + halfNumberFrames;
    		timeSinceLastFrame = 0;
    	}
    }else {
  	   if (timeSinceLastFrame > frameInterval) {
         currentFrame = (currentFrame+1) % halfNumberFrames;
  		   timeSinceLastFrame = 0;
  	    }
      }
  }

  void TwoWaySprite::update(Uint32 ticks) {

    if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
   }
    advanceFrame(ticks);

    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

    if ( getY() < 0) {
      setVelocityY( fabs( getVelocityY() ) );

    }
    if ( getY() > worldHeight-getScaledHeight()) {
      setVelocityY( -fabs( getVelocityY() ) );

    }

    if ( getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      transfer = false;
    }
    if ( getX() > worldWidth-getScaledWidth()) {
      setVelocityX( -fabs( getVelocityX() ) );
      transfer = true;
    }

  }
