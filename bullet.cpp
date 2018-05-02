#include <iostream>
#include <cmath>
#include "bullet.h"
#include "imageFactory.h"

Bullet::Bullet ( const std::string& name, const Vector2f& pos, const Vector2f& vel) :
	MultiSprite(name, pos, vel, ImageFactory::getInstance().getImages(name)),
	distance(0),
	maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
	tooFar(false)
 {}



Bullet::Bullet( const Bullet& mb) : 
	MultiSprite(mb),
	distance(mb.distance),
	maxDistance(mb.maxDistance),
	tooFar(mb.tooFar)
 {}

void Bullet::reset() {
	tooFar = false;
	distance = 0;
}

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  if( getY() < 0 || getY()+getScaledHeight() > worldHeight ) {
  	tooFar = true;
  } if( getX() < 0 || getX()+getScaledWidth() > worldWidth ) {
  	tooFar = true;
  } 

  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if(distance > maxDistance) {
  	tooFar = true;
  }
}

