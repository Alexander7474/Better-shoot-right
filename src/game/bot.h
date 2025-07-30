#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

class Agressivite;

using namespace std;


// TODO -- 
// Bot ne doit pas hériter de gameCharacter mais posséder un gameCharacter (dans un unique_ptr) avec getter
// Avec la possession d'un gameCharacter bot pourra avoir une simple méthode update et pas Bupdate
// gameWindow est une variable global arrête de la passer en paramètre 
// Modifie la fonction Draw pour qu'elle corresponde au standard de BBOP
// Rend tes variable plus explicite, arrête les abrègés et les nom pas fini genre timer c'est timerDiscussion
// Utilise des std::vector préalloué pour Chokpoint(Chokpointqui devrais plutôt s'appeller crossPoints)
// Utilise des enum class au lieu de enum
// Fait une doxygen pour tes fonctions et renome les au format camelCase
// IL FAUT QUE TOUS LES BOT PUISSE ÊTRE UTILISÉ À PARTIR DE LA CLASS BOT ET DES MÉTHODES UPDATE() ET DRAW()

class Bot
{
public:
    Bot();
    /**
     * @brief verifie si le joueur se situe dans le champ visuel
     *
     * @param user Pointeur vers le joueur
     * 
     * @return true si le jouer est dans le champvisuel
     */
    bool ChampVisuel(GameCharacter *user);
    /**
     * @brief verifie si le bot est proche d un des point de patrouille
     *
     * @return true si le bot est a une certaine distance de la zone de patrouille 
     */
    bool PatrolZone();
    /**
     * @brief retourne a un point donne
     *
     * @param point Point a rejoindre
     * 
     * @return true si le bot est arrive au point donne
     */
    bool MoveToPoint(Vector2f point);
    /**
     * @brief verifie si un des menbres du joueur est detecte 
     *
     * @param menber Pointeur vers un des menbres du joeur
     * @param point point de detection
     * 
     * @return true si le point est dans la collisonbox du joueur
     */
    bool DetectPoint(CollisionBox* menber,Vector2f point);
    /**
     * @brief permet de donner une liste de point de patrouille
     *
     * @param s1 point de passage
     * @param s2 point de passage
     * @param s3 point de passage
     * 
     * @return true si le point est dans la collisonbox du joueur
     */
    void setChockPoint(Vector2f s1,Vector2f s2,Vector2f s3);
    /**
     * @return dernier point du jouer etant encore detecte
     */
    enum class State{
        patrol =1,
        seek =2,
        engage =3,
        interaction =4
    };
    Vector2f getSeekPosition();
    void update(Map *map,GameCharacter *perso1);//mise a jour du bot//
    void PatrolMod();//patrouille//
    void DetectPlayer(GameCharacter *user);//look bot// 
    void Draw(Scene* scene);//dessine la bar de vie et le dialogue//
    GameCharacter * getCharacthere();
    State getState();
protected:
    unique_ptr<GameCharacter> pnj;
    Vector2f *Chokpoint; //liste des point de passage en mode patrouille//
    Vector2f PreviousPosition; //ancienne position pour le deplacement//
    float fov ;//fov/
    Vector2f SeekPosition;//dernier point du joueur avant de sortir du champvisuel//
    float Detect,Diviseur;//temp de detection//
    bool ftd;//first time detect//
    Font *font;
    float timer;//timer pour la discussion//
    int cpt,iterateur;//sense en mode patrouille//
    float unlock;//temp avant de delock le joueur//
    RectangleShape *hpbar;//barre de vie//
    TexteBox *dialoque;//texte de dialogue//
    State etat;//etat du bot//
    bool discussing;//en train de parler//
    Agressivite *menace;
    


};

