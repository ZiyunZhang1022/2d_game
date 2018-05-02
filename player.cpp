#include "player.h"
#include "smartSprite.h"

Player::Player( const std::string& name) :
  TwoWaySprite(name),
  collision(false),
  isGol(false),
  observers(),
  initialVelocity(getVelocity()),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(bulletName),
  minBulletSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

Player::Player(const Player& s) :
  TwoWaySprite(s),
  collision(s.collision),
  isGol(s.isGol),
  observers(s.observers),
  initialVelocity(s.getVelocity()),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minBulletSpeed(s.minBulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet)
  { }

Player& Player::operator=(const Player& s) {
  TwoWaySprite::operator=(s);
  collision = s.collision;
  observers = s.observers;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    transfer = false;
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    transfer = true;
  }
}
void Player::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::shoot() {
  if ( timeSinceLastBullet < bulletInterval ) {
    return;
  }
  Vector2f vel = getVelocity();
  float x;
  float y = getY() + getScaledHeight()/2;
  if(vel[0] > 0) {
    x = getX() + getScaledWidth();
    vel[0] += minBulletSpeed;
  } else if(vel[0] < 0){
    x = getX();
    vel[0] -= minBulletSpeed;
  }

  bullets.shoot(Vector2f(x,y),vel);
  timeSinceLastBullet = 0;

}

void Player::draw() const {
  TwoWaySprite::draw();
  bullets.draw();
}

void Player::update(Uint32 ticks) {
  if ( !collision ) TwoWaySprite::update(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() && isGol) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
  stop();
}
