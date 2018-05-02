#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "multisprite.h"

#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"


auto Less = [](const Drawable* ith, const Drawable* ithplusfirsteth) {
  return ith->getScale() < ithplusfirsteth->getScale();
};

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() {

    std::vector<SmartSprite*>::iterator it = sprites.begin();
    while (it != sprites.end()) {
        delete *it;
        ++it;
    }
    std::vector<CollisionStrategy*>::iterator is = strategies.begin();
    while(is != strategies.end()) {
      delete *is;
      ++is;
    }
    delete player;
    delete hud;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sea("sea", Gamedata::getInstance().getXmlInt("sea/factor") ),
  hills("hills", Gamedata::getInstance().getXmlInt("hills/factor")),
  viewport( Viewport::getInstance() ),
  sprites(),
  lifes(3),
  points(0),
  player(new Player("bird")),
  strategies(),
  currentStrategy(0),
  collision(false),
  currentSprite(1),
  makeVideo( false ),
  showHud(false),
  exposion(false),
  isWin(false),
  exposionTrick(0),
  hud(new Hud()),
  sound()
{
    Vector2f pos = player->getPosition();
    sound[0];
    int w = player->getScaledWidth();
    int h = player->getScaledHeight();
    unsigned int stoneSize = Gamedata::getInstance().getXmlInt("numberOfStons");
    for(unsigned int i = 0; i < stoneSize; i++) {
      SmartSprite* s = new SmartSprite("stone", pos, w, h);
      float scale = Gamedata::getInstance().getRandFloat(0.5,2);
      s->setScale(scale);
      sprites.push_back(s);
        // sprites.push_back(new SmartSprite("stone", pos, w, h));
      player->attach( sprites[i] );
    }
    std::vector<SmartSprite*>::iterator ptr = sprites.begin();
    ++ptr;
    sort(ptr, sprites.end(), Less);


//    sprites.push_back(new TwoWaySprite("runrabbit"));
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  for(unsigned int i = 0; i < sprites.size();i++) {
    if(sprites.at(i)->getScale() < 1) {
      sprites.at(i)->draw();
    }
  }
  sea.draw();
  for(unsigned int i = 0; i < sprites.size();i++) {
    if(sprites.at(i)->getScale() < 1.5) {
      sprites.at(i)->draw();
    }
  }
  hills.draw();
  Vector2f position;
  if(showHud) {
   position = Vector2f(Gamedata::getInstance().getXmlInt("midhud/startLoc/x"),
                                    Gamedata::getInstance().getXmlInt("midhud/startLoc/y"));
  int width = Gamedata::getInstance().getXmlInt("midhud/width");
  int height = Gamedata::getInstance().getXmlInt("midhud/height");
  hud -> draw(position, width, height);
  }else {

    if(lifes <= 0) {
      position = Vector2f(Gamedata::getInstance().getXmlInt("midhud/startLoc/x"),
                                       Gamedata::getInstance().getXmlInt("midhud/startLoc/y"));
     int width = Gamedata::getInstance().getXmlInt("midhud/width");
     int height = Gamedata::getInstance().getXmlInt("midhud/height");
     hud -> draw(position, width, height, "fail", points);
   }else if(isWin) {
     position = Vector2f(Gamedata::getInstance().getXmlInt("midhud/startLoc/x"),
                                      Gamedata::getInstance().getXmlInt("midhud/startLoc/y"));
    int width = Gamedata::getInstance().getXmlInt("midhud/width");
    int height = Gamedata::getInstance().getXmlInt("midhud/height");
    hud -> draw(position, width, height, "Win", points);
    clock.pause();
   }else {

    position = Vector2f(Gamedata::getInstance().getXmlInt("uprighthud/startLoc/x"),
                                      Gamedata::getInstance().getXmlInt("uprighthud/startLoc/y"));
    int width = Gamedata::getInstance().getXmlInt("uprighthud/width");
    int height = Gamedata::getInstance().getXmlInt("uprighthud/height");
    hud -> draw(position, width, height, lifes, points, player->bulletCount(), player->freeCount(), player->getGolState());
    for(unsigned int i = 0; i < sprites.size();i++) {
      if(sprites.at(i)->getScale() >=  1.5) {
        sprites.at(i)->draw();
      }

    }
  // spinningStar->draw();
  // rabbit -> draw();
    strategies[currentStrategy]->draw();
    player -> draw();
  }
}
  viewport.draw();


  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) && (*it)->getScale() >= 1.5 ) {

      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
      player->explode();
      lifes--;
      exposionTrick = clock.getElapsedTicks();
      exposion = true;
      break;
    }
    else ++it;
  }
}

void Engine::checkShootCollisions() {
  Bullets& bullets = player -> getBulltes();
  auto it = sprites.begin();
  while (it != sprites.end()) {
  // //   /* code */
    if((*it)-> getScale() > 1.5 &&bullets.collided(*it)) {
      sound[0];
        SmartSprite* doa = *it;
        doa -> explode();
        points += 100;

    }else {
      ++ it;
    }


  }
}

void Engine::update(Uint32 ticks) {
//  star->update(ticks);
  if(!(exposion)) {
    checkForCollisions();
    if(player-> bulletCount() != 0 && !exposion) {
      checkShootCollisions();}
    player -> update(ticks);
    for ( Drawable* sprite : sprites ) {
    sprite->update( ticks );
  }
} else if(exposion) {
  exposion = false;
}
  if(player->getX() >= (player->getWorldWidth() - player->getScaledWidth())) {
    isWin = !isWin;
  }
  // spinningStar->update(ticks);
  // rabbit -> update(ticks);

  sea.update();
  hills.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(sprites.at(10));
  }
  else {
    Viewport::getInstance().setObjectToTrack(sprites.at(0));
  }
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
        currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if( keystate[SDL_SCANCODE_F1] ) {
          showHud = !showHud;
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
          sound[0];
        }
        if( keystate[SDL_SCANCODE_G]) {
          player -> setGolState();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
  //     if(keystate[SDL_SCANCODE_A]){
	// static_cast<Player*>(player) -> left();
  //     }
      if(keystate[SDL_SCANCODE_D]) {
	static_cast<Player*>(player) -> right();
      }
      if(keystate[SDL_SCANCODE_W]) {
	static_cast<Player*>(player) -> up();
      }
      if(keystate[SDL_SCANCODE_S]) {
	static_cast<Player*>(player) -> down();
      }
      if ( keystate[SDL_SCANCODE_E] ) {
        sprites.at(0)->explode();
      }
      draw();
      if(!showHud){
      update(ticks);
    }
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
    return false;
}
