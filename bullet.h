#ifndef BULLET__H
#define BULLET__H

#include <iostream>
#include "multisprite.h"

class Bullet : public MultiSprite {
public:
  Bullet(const std::string&, const Vector2f&, const Vector2f&);
  Bullet(const Bullet&);

  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }

  void reset();
  Bullet& operator=(const Bullet&);
private:
  float distance;
  float maxDistance;
  bool tooFar;
};

#endif
