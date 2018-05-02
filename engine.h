
#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "sound.h"
#include "menuEngine.h"

class CollisionStrategy;
class SmartSprite;
class Player;

class Engine {
public:
  Engine ();
  ~Engine ();

  bool play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;

  Clock& clock;


  SDL_Renderer * const renderer;
  World sea;
  World hills;
  MenuEngine menuEngine;
  Vector2f menuMsgLoc;
  Viewport& viewport;

  std::vector<SmartSprite*> sprites;

  int lifes;
  int points;
  // Drawable* rabbit;
  // Drawable* spinningStar;
  Player* player;

  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  int currentSprite;

  bool makeVideo;
  bool showHud;
  bool exposion;
  bool isWin;
  int exposionTrick;

  Hud* hud;
  SDLSound sound;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
  void checkShootCollisions();
};
