#ifndef PLAYER__H
#define PLAYER__H

#include <list>
#include <cmath>
#include "multisprite.h"
#include "twoWaySprite.h"
#include "gamedata.h"
#include "bullets.h"

// In this example the player is derived from MultiSprite.
// However, there are many options.
class SmartSprite;

class Player : public TwoWaySprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );

  void collided() { collision = true; }
  void missed() { collision = false; }
  void setGolState() {
    isGol = !isGol;
  }
  bool getGolState() const {
    return isGol;
  }
  Player& operator=(const Player&);
  void shoot();

  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  Bullets& getBulltes() {return bullets;}

  void right();
  void left();
  void up();
  void down();
  void stop();
private:
  bool collision;
  bool isGol;
  std::list<SmartSprite*> observers;
  Vector2f initialVelocity;

  std::string bulletName;
  Bullets bullets;
  float minBulletSpeed;
  float bulletInterval;
  float timeSinceLastBullet;
};
#endif
