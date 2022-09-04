#pragma once

#include <tyra>
#include <cstdlib>
#include <time.h>

namespace Racer {

enum states { menu, game, credits};
enum files { rock, paper, scissors};

class RacerGame : public Tyra::Game {
 public:
  RacerGame(Tyra::Engine* engine);
  ~RacerGame();

  void init();
  void loop();
  
 private:
  void loadTexture(Tyra::Sprite* sprite, const char* image);
  void loadSprite(Tyra::Sprite* sprite, Tyra::Vec2 newPosition);
  void finishGame(const int player, const int ia);
  void reset();

  Tyra::Engine* engine;
  states actualState;
  Tyra::Sprite spriteBackground;
  Tyra::Sprite spriteBackgroundMenu;
  Tyra::Sprite spriteCredits;
  Tyra::Sprite spriteText;
  Tyra::Sprite spriteTextMenu;
  Tyra::Sprite spriteSlot[4];
  Tyra::Sprite spriteCursor[3];
  Tyra::Sprite spriteSelect[3];
  Tyra::Pad* pad1;
  Tyra::Timer timeTyra;

  bool showGame = false;
  bool showCredits = false;
  int showCursor = 1 ;
  int imageIA = 0;
  int slotRender = 2;
  unsigned int score = 0;
  float timer = 0;
  float maxTime = 10000;
  u8 volume = 60;
  u8 volumeMute = 0;
  bool mute = false;
};

}  // namespace Racer
