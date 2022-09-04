#include "racer_game.hpp"

using namespace std;
namespace Racer {

using namespace Tyra;

const char * imageFile[] = { "piedra1.png", "papel1.png", "tijera1.png",
                             "piedra2.png", "papel2.png", "tijera2.png" };

RacerGame::RacerGame(Engine* t_engine) { 
    engine = t_engine; 
    pad1 = &engine->pad;
}
RacerGame::~RacerGame() {
  for(int i = 0; i < 3; i++){
    engine->renderer.getTextureRepository().freeBySprite(spriteSelect[i]);
    engine->renderer.getTextureRepository().freeBySprite(spriteCursor[i]);
    engine->renderer.getTextureRepository().freeBySprite(spriteSlot[i]);
  }  
  engine->renderer.getTextureRepository().freeBySprite(spriteSlot[3]);
  engine->renderer.getTextureRepository().freeBySprite(spriteBackground);
  engine->renderer.getTextureRepository().freeBySprite(spriteBackgroundMenu);
  engine->renderer.getTextureRepository().freeBySprite(spriteCredits);
  engine->renderer.getTextureRepository().freeBySprite(spriteText);
  engine->renderer.getTextureRepository().freeBySprite(spriteTextMenu);
}

void RacerGame::init() { 
    engine->renderer.setClearScreenColor(Color(32.0F, 32.0F, 32.0F));

    srand(time(0)); // Seed for random number

    const auto& screenSettings = engine->renderer.core.getSettings();

    engine->audio.song.load(FileUtils::fromCwd("Bubble-Bobble-Arcade-Music.wav"));
    engine->audio.song.inLoop = true;
    engine->audio.song.setVolume(volume);
    engine->audio.song.play();

    actualState = menu;

    float posX = -170;
    float screenWidth = screenSettings.getWidth() / 2.0F; 
    float screenHeight = screenSettings.getHeight() / 2.0F;

    spriteTextMenu.mode = SpriteMode::MODE_STRETCH;
    spriteTextMenu.size = Vec2(256,32);
    spriteTextMenu.scale = 2;
    spriteTextMenu.position = Vec2(0, screenHeight );

    loadTexture(&spriteTextMenu,"pressStart.png");

    for(int i = 0; i < 3; i++){
        /** Sprite contains rectangle information. */
        loadSprite(&spriteSelect[i], 
        Vec2(screenWidth - spriteSelect[i].size.x / 2.0F + posX,
         screenHeight - spriteSelect[i].size.y / 2.0F));

        /** Texture contains png image. */
        loadTexture(&spriteSelect[i], imageFile[i]);

        posX += 128;
    }

    posX = -170;
    screenHeight = screenSettings.getHeight() / 2.0F + 128;

    for(int i = 0; i < 3; i++){
        /** Sprite contains rectangle information. */
        loadSprite(&spriteCursor[i], 
        Vec2(screenWidth - spriteCursor[i].size.x / 2.0F + posX,
         screenHeight - spriteCursor[i].size.y / 2.0F));

        /** Texture contains png image. */
        loadTexture(&spriteCursor[i], imageFile[i+3]);

        posX += 128;
    }

    posX = -124;
    screenHeight = screenSettings.getHeight() / 2.0F - 128;

    u32 idSlot2 = spriteSlot[2].id;
    u32 idSlot3 = spriteSlot[3].id;
    for(int i = 0; i < 2; i++){
        /** Sprite contains rectangle information. */
        loadSprite(&spriteSlot[i], 
        Vec2(screenWidth - spriteSlot[i].size.x / 2.0F + posX,
         screenHeight - spriteSlot[i].size.y / 2.0F));

        /** Texture contains png image. */
        loadTexture(&spriteSlot[i], "slot.png");

        posX += 150;
    }

    spriteSlot[2] = spriteSlot[0]; 
    spriteSlot[3] = spriteSlot[1]; 

    spriteSlot[2].id = idSlot2;
    spriteSlot[3].id = idSlot3;

    /** Sprite contains rectangle information. */

    spriteBackground.mode = SpriteMode::MODE_STRETCH;
    spriteBackground.size = Vec2(716, 716);     
    spriteBackground.position = Vec2(20,0);

    /** Texture contains png image. */
    loadTexture(&spriteBackground, "background.png");
    
    spriteText.mode = SpriteMode::MODE_STRETCH;
    spriteText.scale = 2;

    spriteBackgroundMenu.mode = SpriteMode::MODE_STRETCH;
    spriteBackgroundMenu.size = Vec2(256, 256);     
    spriteBackgroundMenu.scale = 2;
    spriteBackgroundMenu.position = Vec2(0,0);

    loadTexture(&spriteBackgroundMenu,"menu.png");

    spriteCredits.mode = SpriteMode::MODE_STRETCH;
    spriteCredits.size = Vec2(256, 256); 
    spriteCredits.scale = 2;    
    spriteCredits.position = Vec2(0,0);

    loadTexture(&spriteCredits,"credits.png");
    
}

void RacerGame::loop() { 
    
    //printf("Timer: %f, deltaTime: %d\n", timer, time.getTimeDelta()); 
    timer += timeTyra.getTimeDelta(); 
    timeTyra.prime();

    // Audio Config for the player

    if(timer > 2000){
        if (pad1->getClicked().L1 || pad1->getClicked().R1){

            if(engine->audio.song.isPlaying() == false){
                engine->audio.song.play();
                TYRA_LOG("MUSIC PLAY");
            }else{
                engine->audio.song.stop();
                TYRA_LOG("MUSIC STOP");
            }
            timer = 0;
        }

        if (pad1->getClicked().L2 || pad1->getClicked().R2){
            mute = !mute;

            if(mute == true){
                volumeMute = volume;
                engine->audio.song.setVolume(0);
                TYRA_LOG("MUSIC MUTE");
            }else{
                volume = volumeMute;
                engine->audio.song.setVolume(volumeMute);
                TYRA_LOG("MUSIC NOT MUTE");
            }
            timer = 0;
        }

        if (engine->audio.song.isPlaying() == true && (pad1->getLeftJoyPad().v <= 100 || pad1->getPressed().DpadUp)) {
            if(mute == true){
                volume = volumeMute;
                engine->audio.song.setVolume(volumeMute);
                mute = false;
            }

            if(volume < 255){
                volume++;
                engine->audio.song.setVolume(volume);
            }            
            timer = 0;
            TYRA_LOG("Volume UP");
            printf("volume: %d",volume);
        } else if (engine->audio.song.isPlaying() == true && (pad1->getLeftJoyPad().v >= 200 || pad1->getPressed().DpadDown)) {
            if(mute == true){
                volume = volumeMute;
                engine->audio.song.setVolume(volumeMute);
                mute = false;
            }

            if(volume > 0){
                volume--;
                engine->audio.song.setVolume(volume);
            }        

            timer = 0;      
            TYRA_LOG("Volume DOWN");
            printf("volume: %d",volume);
        }
    }
    

    auto& renderer = engine->renderer;

    if(actualState == menu){
        if(pad1->getClicked().Start){
            actualState = game;
            timer = 0;
            TYRA_LOG("Start");
        }else if(pad1->getClicked().Select){
            actualState = credits;
            TYRA_LOG("Select");
        }

        /** Begin frame will clear our screen. */
        renderer.beginFrame();

        renderer.renderer2D.render(spriteBackgroundMenu);

        if(timer < maxTime){
            renderer.renderer2D.render(spriteTextMenu);          
        }else if(timer > maxTime + 2000){
            timer = 0;
        }
        

        /** End frame will perform vsync. */
        renderer.endFrame();
    }else if(actualState == game){
        if(timer > 3000){
        
            if(slotRender == 4 && (pad1->getClicked().Square || pad1->getClicked().Start) ){
                reset();
                TYRA_LOG("Square");
            }

            if(pad1->getClicked().Triangle || pad1->getClicked().Select){
                actualState = menu;
                TYRA_LOG("Square");
            }

            if (pad1->getLeftJoyPad().h <= 100 || pad1->getPressed().DpadLeft) {
                showCursor--;
                timer = 0;
                TYRA_LOG("LEFT");
            } else if (pad1->getLeftJoyPad().h >= 200 || pad1->getPressed().DpadRight) {
                showCursor++;
                timer = 0;
                TYRA_LOG("RIGHT");
            }

            if (slotRender == 2 && pad1->getClicked().Cross){
                
                loadTexture(&spriteSlot[2], imageFile[showCursor+3]);
                slotRender++;
                TYRA_LOG("SELECTED");
                                    
                imageIA = rand() % 3;

                loadTexture(&spriteSlot[3], imageFile[imageIA+3]);
                slotRender++;

                finishGame(showCursor,imageIA);
            }

        }
   
        if(showCursor < 0){
            showCursor = 2;
        }else if(showCursor >2){
            showCursor = 0;
        }


        /** Begin frame will clear our screen. */
        renderer.beginFrame();

        renderer.renderer2D.render(spriteBackground);

        if(slotRender > 2)
        renderer.renderer2D.render(spriteText);

        for(int i = 0; i < slotRender; i++){
            renderer.renderer2D.render(spriteSlot[i]);
        }
        
        /** Render sprite. */
        for(int i = 0; i < 3; i++){
            renderer.renderer2D.render(spriteSelect[i]);
        } 

        renderer.renderer2D.render(spriteCursor[showCursor]);
        
        /** End frame will perform vsync. */
        renderer.endFrame();
    }else if(actualState == credits){
        
        if(pad1->getClicked().Cross || pad1->getClicked().Circle){
            actualState = menu;
            TYRA_LOG("Start");
        }

        /** Begin frame will clear our screen. */
        renderer.beginFrame();

        renderer.renderer2D.render(spriteCredits);

        /** End frame will perform vsync. */
        renderer.endFrame();
    }
    
    

    
    
}

void RacerGame::loadSprite(Sprite* sprite, Vec2 newPosition) {
  sprite->mode = SpriteMode::MODE_STRETCH;

  sprite->size = Vec2(128.0F, 128.0F);

  sprite->position = newPosition;  

  TYRA_LOG("Sprite created!");
}

void RacerGame::loadTexture(Sprite* sprite, const char* image) {
  /**
   * Renderer has high layer functions,
   * which allows to render:
   * - Sprite (2D)
   * - Mesh (3D)
   *
   * It uses ONLY low layer functions which are in renderer.core
   */
  auto& renderer = engine->renderer;

  /**
   * TextureRepository is a repository of textures.
   * It is a singleton class, with all game textures.
   * We are linking these textures with sprite's (2D) and mesh (3D) materials.
   */
  auto& textureRepository = renderer.getTextureRepository();

  /**
   * Texture is stored in "res" directory.
   * Content of "res" directory is automatically copied into
   * "bin" directory, which contains our final game.
   *
   * File utils automatically add's device prefix to the path,
   * based on current working directory.
   *
   * In PS2 world:
   * - USB has a "mass:" prefix
   * - Our PC in PS2Link has a "host:" prefix
   * - Our PC in PCSX2 has a "host:" prefix
   */
  auto filepath = FileUtils::fromCwd(image);

  /**
   * Tyra supports following PNG formats:
   * 32bpp (RGBA)
   * 24bpp (RGB)
   * 8bpp, palletized (RGBA)
   * 4bpp, palletized (RGBA)
   *
   * 8bpp and 4bpp are the fastest.
   * All of these formats can be easily exported via GIMP.
   */
  auto* texture = textureRepository.add(filepath);

  /** Let's assign this texture to sprite. */
  texture->addLink(sprite->id);

  TYRA_LOG("Texture loaded!");
}

void RacerGame::finishGame(const int player, const int ia){
    spriteText.size = Vec2(128, 16);

    const auto& screenSettings = engine->renderer.core.getSettings();
    float screenWidth = screenSettings.getWidth() / 2.0F; 

    spriteText.position = Vec2(screenWidth - spriteText.size.x / 2.0F - 16,30);

    if(player == ia){
        spriteText.size = Vec2(128, 32);
        spriteText.position = Vec2(screenWidth - spriteText.size.x / 2.0F + 10,30);
        loadTexture(&spriteText, "draw.png");
    }else if(player == rock && ia == paper){
        loadTexture(&spriteText, "lose.png");
        spriteText.position = Vec2(screenWidth - spriteText.size.x / 2.0F - 20,30);
    }else if(player == rock && ia == scissors){
        score++;
        loadTexture(&spriteText, "win.png");  
    }else if(player == paper && ia == rock){
        score++;
        loadTexture(&spriteText, "win.png");
    }else if(player == paper && ia == scissors){
        spriteText.position = Vec2(screenWidth - spriteText.size.x / 2.0F - 20,30);
        loadTexture(&spriteText, "lose.png");
    }else if(player == scissors && ia == rock){
        spriteText.position = Vec2(screenWidth - spriteText.size.x / 2.0F - 20,30);
        loadTexture(&spriteText, "lose.png");
    }else if(player == scissors && ia == paper){
        score++;
        loadTexture(&spriteText, "win.png");
    }
    
}

void RacerGame::reset(){
    imageIA = 0;
    timer = 0;
    slotRender = 2;
    Texture* texture = engine->renderer.getTextureRepository().getBySpriteId(spriteSlot[2].id);                
    texture->removeLinkById(spriteSlot[2].id); // Removing this texture of the sprite. 
    texture = engine->renderer.getTextureRepository().getBySpriteId(spriteSlot[3].id);
    texture->removeLinkById(spriteSlot[3].id); // Removing this texture of the sprite. 
    texture = engine->renderer.getTextureRepository().getBySpriteId(spriteText.id);
    texture->removeLinkById(spriteText.id); // Removing this texture of the sprite. 
}

}  // namespace Racer
