#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "bullet.h"


class CollisionStrategy;

class Bullets {
  Bullets& operator=(const Bullets&)=delete;
public:
  Bullets(const std::string&);
  Bullets(const Bullets&);
  ~Bullets();

  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }
  bool shooting() { return !bulletList.empty(); }
  bool collided(const Drawable*) ;


private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Image*> bulletImages;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;

};

#endif
