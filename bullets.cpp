#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"


Bullets::~Bullets() {
	delete strategy;
}

Bullets::Bullets(const std::string& n) :
	name(n),
	myVelocity(
		Gamedata::getInstance().getXmlInt(name+"/speedX"),
		Gamedata::getInstance().getXmlInt(name+"/speedY")
	),
	bulletImages( ImageFactory::getInstance().getImages(name)),
	bulletList(),
	freeList(),
  strategy(new MidPointCollisionStrategy)
 {

 }

Bullets::Bullets(const Bullets& m) :
	name(m.name),
	myVelocity(m.myVelocity),
	bulletImages(m.bulletImages),
	bulletList(m.bulletList),
	freeList(m.freeList),
	strategy(m.strategy)
 {
	 std::cout<< "bulltes copy" << std::endl;
 }

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel) {

	if( freeList.empty() ) {
		Bullet b (name, pos, objVel);
		bulletList.push_back(b);

	} else {

		Bullet b = freeList.front();
		freeList.pop_front();
		b.reset();
		b.setVelocity(objVel);
		b.setPosition(pos);
		bulletList.push_back(b);
	}
}

bool Bullets::collided(const Drawable* obj)  {

	std::list<Bullet>::iterator ptr = bulletList.begin();

	while(bulletList.size() != 0 && ptr != bulletList.end()) {
		if( strategy->execute(*ptr, *obj) ) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
			return true;
		} else {
			++ptr;
		}
	}

	return false;
}

void Bullets::draw() const {
	for( const auto& bullet : bulletList) {
		bullet.draw();
	}
}

void Bullets::update(Uint32 tricks) {
	std::list<Bullet>::iterator ptr = bulletList.begin();
	while(ptr != bulletList.end()) {
		ptr->update(tricks);
		if(ptr->goneTooFar()) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
		} else {
			++ptr;
		}
	}
}
