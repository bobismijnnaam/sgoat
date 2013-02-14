#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL/SDL.h>

enum GameStates { // Hier wordt vastgesteld welke gamestates er allemaal mogelijk zijn en krijgen ze een nummertje, dit nummeren gebeurt "achter de schermen"
    STATE_NULL = 0,
    STATE_INTRO,
    STATE_MENU,
    STATE_HELP,
    STATE_LVLSELECT,
    STATE_GAME,
    STATE_EXIT
}   ;

enum GameManStates {
    STATE_TRANSITION = 0,
    STATE_RUNNING
} ;

class cGameState {  // Dit is de blauwdruk voor een gamestate class. Merk op dat er 2 virtuele functies zijn
                    // Dat zijn de functies met =0 erachter. Die "bestaan" als het ware nog niet. Deze class kan dus
                    // in feite niet gebruikt worden. Als je de functies wilt gebruiken moet je een class definiëren
                    // die gebaseerd is op deze: "overerving". Daar moet je de functies dan alsnog definiëren,
                    // waardoor alle functies die gebaseerd zijn op deze ongeveer dezelfde structuur hebben.
                    // Dit zorgt ervoor dat je gebruikt kunt maken van de feature polymorphisme in C++.
                    // Als je daar meer over wilt weten kun je me aanspreken in de les of het opzoeken op internet
public:
    virtual int events() = 0; // Ligt grotendeels vast maar kan als nodig veranderd worden. haalt input binnen (muis enzo, toetsenbord) want dat moet toch elke loop gebeuren
    virtual int logic() = 0; // Hier gebeurd de magie. Hier vind GEEN REGEL rendering plaats.
    virtual int render(SDL_Surface* dst) = 0; // Weergeven van die shit

    virtual ~cGameState(){}; // De destructor. Wordt verder niet gebruikt, maar moet je puur voor de conventies deiniëren
} ;

class cGameMan {    // Dit is het skelet voor de gamestate manager. Wat de gamestate manager eigenlijk doet is
                    // bijhouden in welke state (situatie: hoofdmenu, helpscherm, spelscherm, etc.) het spel is. Dit klinkt heel suf, maar dit is erg belangrijk.
                    // Deze class zorgt ervoor dat ik makkelijk tussen schermen kan wisselen, maar ook dat
                    // er naast de normale gameloop er op de achtergrond nog een proces actief kan blijven
                    // draaien (denk hier bijvoorbeeld aan achievements!
private:
    GameStates nextState; // Hier wordt opgeslagen in welke situatie het spel moet overgaan
    GameStates stateID; // Hier wordt de huidige situatieID in opgeslagen

    cGameState* currState;

public:
    ~cGameMan(); // De destructor; deze wordt uitgevoerd als de class gedelete wordt.
    cGameMan(GameStates startState);   // Dit is de constructor (wordt gerunned als de class wordt aangemaakt). Hier wordt ingesteld
                                // wat de startsituatie is e.d.

    // Dit is de pointer naar de huidige state. Merk op dat dit een pointer is naar de class
                                // cGameState, een class waarvan ik zei dat hij eigenlijk niet kon bestaan. Dit is dus polymorphisme.

    // int setNextState(cGameState* targetState);
    int setNextState(GameStates targetState); // Dit is de functie waarmee je aangeeft dat je van class wilt veranderen
    GameStates getState(); // Met deze functie vraag je de huidige state op
    int changeState(); // Deze functie veranderd de functie ook daadwerkelijk (verwijderd de oude, maakt de nieuwe, etc.)

    int events();
    int logic();
    int render();

    int lvl; // Level keuze
    bool snd; // Geluid

    GameManStates state;
    int stateTime;
    SDL_Surface* sScroll;
    SDL_Surface* tSurf;
} ;

#endif
