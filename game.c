#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/joy.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/game.h>
#include <ace/utils/palette.h>
#include <ace/utils/font.h>
#include <stdio.h>
#include <ace/managers/rand.h>
#include <ace/managers/state.h>
#include <ace/utils/custom.h>
#include <ace/utils/ptplayer.h>
#include <ace/utils/file.h>

//------------------------------------------------------- gdzie� przed funkcjami
// zmienne trzymaj�ce adresy do viewa, viewporta, simple buffer managera
static tView *s_pView;
static tVPort *s_pVp;
static tSimpleBufferManager *s_pVpManager;
static tBitMap *s_pCards;

static tPtplayerMod *s_pMod;

static tFont *s_pFont;
static tTextBitMap *s_pBmText;

extern tStateManager *g_pStateMachineGame;

#define MAP_TILE_HEIGHT 7

char szMsg[50];  // do wyswietlania wegla na HUD
char szMsg2[50]; // do wyswietlania kondkow na HUD
char levelFilePath[20];

UBYTE musicPlay = 1;


void stateGameCreate(void)
{
  // Here goes your startup code
  //-------------------------------------------------------------- gdzie� w create
  s_pView = viewCreate(0,
                       TAG_VIEW_COPLIST_MODE, COPPER_MODE_BLOCK, // spos�b u�ywania coppera - ten jest najprostszy bo nic z nim nie musisz wi�cej robi�
                       TAG_VIEW_GLOBAL_CLUT, 1,                  // globalna paleta dla wszystkich viewport�w
                       TAG_END                                   // wi�cej argument�w nie ma
  );

  s_pVp = vPortCreate(0,
                      TAG_VPORT_VIEW, s_pView, // parent view
                      TAG_VPORT_BPP, 5,        // bits per pixel: 4bpp = 16col, 5pp = 32col, etc.
                      TAG_END);

  ptplayerCreate(1);
  s_pMod = ptplayerModCreate("data/placeholdermod.mod");
  ptplayerLoadMod(s_pMod, 0, 0);

  // Paleta z falkona
  paletteLoad("data/grota.plt", s_pVp->pPalette, 32);

  g_pCustom->color[0] = 0x0FFF; // zmie� kolor zero aktualnie u�ywanej palety na 15,15,15

  s_pCards = bitmapCreateFromFile("data/grota.bm", 0);          // z pliku tileset.bm, nie lokuj bitmapy w pami�ci FAST
 

  

  s_pFont = fontCreate("data/topaz.fnt");
  s_pBmText = fontCreateTextBitMap(300, s_pFont->uwHeight); // bitmapa robocza długa na 200px, wysoka na jedną linię tekstu

  // proste wy�wietlanie bitmapy na viewporcie
  s_pVpManager = simpleBufferCreate(0,
                                    TAG_SIMPLEBUFFER_VPORT, s_pVp,                              // parent viewport
                                    TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED, // wst�pne czyszczenie bitmapy, przyspieszenie rysowania grafiki
                                    TAG_SIMPLEBUFFER_IS_DBLBUF, 0,                              // nie potrzebujemy double buffering
                                    TAG_SIMPLEBUFFER_BOUND_WIDTH, 320 + 16,
                                    TAG_END);

  cameraSetCoord(s_pVpManager->pCamera, 0, 0);

 
  
  joyOpen(); // b�dziemy u�ywa� d�oja w grze
  keyCreate();
  // na koniec create:

  randInit(1337);

  
  blitRect(s_pVpManager->pBack, 0, 0, 320, 128, 0);   // czarne tlo na ekran

  blitCopy(s_pCards, 100,100, s_pVpManager->pBack, 0, 0, 100, 100, MINTERM_COPY, 0xFF); // narysujmy se karte

  

  viewProcessManagers(s_pView);
  viewLoad(s_pView);

  ptplayerEnableMusic(1);
  systemUnuse(); // system w trakcie loop nie jest nam potrzebny
}

void stateGameLoop(void)
{
 
  joyProcess();
  keyProcess();

  
  if (joyUse(JOY1_RIGHT) || keyUse(KEY_D) || keyUse(KEY_RIGHT))
  {
    
  }
  else if (joyUse(JOY1_LEFT) || keyUse(KEY_A) || keyUse(KEY_LEFT))
  {
   
  }
  else if (joyUse(JOY1_UP) || keyUse(KEY_W) || keyUse(KEY_UP))
  {
    
  }
  else if (joyUse(JOY1_DOWN) || keyUse(KEY_S) || keyUse(KEY_DOWN))
  {
   
  }
  else if (keyUse(KEY_ESCAPE))
  {
    ptplayerStop();
    
 
    // stateChange(g_pStateMachineGame, &g_sStateMenu);
    return;
  }
  
  else if (keyUse(KEY_M))
  {
    if (musicPlay == 1)
    {
      musicPlay = 0;
      mt_mastervol(0);
    }
    else if (musicPlay == 0)
    {
      musicPlay = 1;
      mt_mastervol(64);
    }
  }

  

  

  viewProcessManagers(s_pView); // obliczenia niezb�dne do poprawnego dzia�ania viewport�w
  copProcessBlocks();           // obliczenia niezb�dne do poprawnego dzia�ania coppera
  
}

void stateGameDestroy(void)
{
  // Here goes your cleanup code
  systemUse(); // w��cz grzecznie system
  ptplayerModDestroy(s_pMod);
  ptplayerDestroy();
  

  fontDestroy(s_pFont);
  fontDestroyTextBitMap(s_pBmText);

  viewDestroy(s_pView); // zwolnij z pami�ci view, wszystkie do��czone do niego viewporty i wszystkie do��czone do nich mened�ery
  joyClose();
  keyDestroy();
}

// pod funkcjami bo kompilator czyta od g�ry do do�u i musi najpierw napotka� funkcje by wiedzie� �e istniej�
tState g_sStateGame = {
    .cbCreate = stateGameCreate,
    .cbLoop = stateGameLoop,
    .cbDestroy = stateGameDestroy,
    .cbSuspend = 0,
    //.cbResume = gameOnResume,
    .pPrev = 0};
