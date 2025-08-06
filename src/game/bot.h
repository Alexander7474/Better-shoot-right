#pragma once

#include "../../Bbop-2D/include/BBOP/Graphics/bbopGlobal.h"
#include "../../Bbop-2D/include/BBOP/Graphics/cameraClass.h"
#include "../engine/gameCharacter.h"
#include "../engine/crossair.h"

class Agressivite;

using namespace std;


// TODO -- 
//Arranger les fonctions de déplacement 
//Le bot doit pouvoir mourir
//Le Bot doit pouvoir recharger


class Bot : public BbopDrawable
{
public:
    Bot();
    /**
     * @brief verifie si le joueur se situe dans le champ visuel
     *
     * @param user Pointeur vers le joueur
     * 
     * @return true si le jouer est dans le champVisuel
     */
    bool champVisuel(GameCharacter *user);
    /**
     * @brief verifie si le bot est proche d un des point de patrouille
     *
     * @return true si le bot est a une certaine distance de la zone de patrouille 
     */
    bool patrolZone();
    /**
     * @brief retourne a un point donne
     *
     * @param point Point a rejoindre
     * 
     * @return true si le bot est arrive au point donne
     */
    bool moveToPoint(Vector2f point);
    /**
     * @brief verifie si un des menbres du joueur est detecte 
     *
     * @param menber Pointeur vers un des menbres du joeur
     * @param point point de detection
     * 
     * @return true si le point est dans la collisonbox du joueur
     */
    bool detectPoint(CollisionBox* menber,Vector2f point);
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
    void update(Map *map,GameCharacter *perso1,Game * game);//mise a jour du bot//
    void patrolMod();//patrouille//
    void detectPlayer(GameCharacter *user);//look bot// 
    void Draw(GLint* renderUniforms) const override;//dessine la bar de vie et le dialogue//
    GameCharacter * getCharacter();
    State getState();
protected:
    unique_ptr<GameCharacter> pnj;
    Vector2f *Chokpoint; //liste des point de passage en mode patrouille//
    Vector2f PreviousPosition; //ancienne position pour le deplacement//
    float fov ;//fov/
    Vector2f SeekPosition;//dernier point du joueur avant de sortir du champVisuel//
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

