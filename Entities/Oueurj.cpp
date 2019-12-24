#include <iostream>
#include <vector>
#include "Oueurj.h"
#include "Pos.h"
#include "../Board.h"
#include "Entity.h"
#include "../Combat.h"

using namespace std;

const int Oueurj::BASE_DMG = 1;
const int Oueurj::HP_MAX = 5;
const int Oueurj::MP_MAX = 10;
const int Oueurj::POWER_MAX = 3;
const int Oueurj::HEAL_COST = 3;
const int Oueurj::POWERATK_COST = 2;
const Pos Oueurj::DEPLACEMENTS_POS[] = {Pos(1, -1), Pos(1, 0), Pos(1, 1), Pos(0, -1), Pos(0,0), Pos(0,1), Pos(-1, -1), Pos(-1, 0), Pos(-1, 1)};

Oueurj::Oueurj() : Entity('j', -1, -1, HP_MAX, BASE_DMG), teleportsLeft(0), mp(MP_MAX), power(POWER_MAX) {}

Oueurj::Oueurj(Pos p) : Entity('j', p, HP_MAX, BASE_DMG), teleportsLeft(0), mp(MP_MAX), power(POWER_MAX) {}

Oueurj::Oueurj(int row, int col) : Entity('j', row, col, HP_MAX, BASE_DMG), teleportsLeft(0), mp(MP_MAX), power(POWER_MAX) {}

void Oueurj::act(Entity &J, vector<vector<char>> &charMap, vector<Entity*> &streumons) {
    bool tourEnded = false;
    while (!tourEnded) { // Tant que le tour n'est pas valide on demande au joueur ce qu'il veut faire
        cout << "Que d�sirez-vous faire ?" << endl
        << "Se d�placer : d | Lancer un sort : s | Ouvrir l'inventaire : i | Quitter : q" << endl;
        string choice;
        cin >> choice;
        tourEnded = manageChoice(choice, charMap, streumons);
    }

}

bool Oueurj::quitGame() const {
    string confirmChoice;
    cout << "�tes-vous s�r de vouloir quitter le jeu ? o/n" << endl;
    cin >> confirmChoice; // Demande de confirmation
    while (confirmChoice[0] != 'o' && confirmChoice[0] != 'n') {
        cout << "�tes-vous s�r de vouloir quitter le jeu ? (Oui = o, Non = n)" << endl;
        cin >> confirmChoice; // Si le joueur n'a entr� ni 'o' ni 'n' en premier caract�re
    }
    if (confirmChoice[0] == 'o') { // Le joueur veut quitter
        Board::gameOn = false;
        return true;
    }
    else if (confirmChoice[0] == 'n') // Le joueur ne veut pas quitter
        return false;

}

bool Oueurj::manageChoice(string choice, vector<vector<char>> &charMap, vector<Entity*> &streumons) {
    switch (choice[0]) {
    case 'd' : { // Le joueur veut se d�placer
        cout << "D�placement :" << endl << "O� d�sirez-vous aller ? (sur l'une des 8 cases autour du joueur)" << endl
        << "1 bas gauche, 2 bas, 3 bas droite, 4 gauche, 5 sur place, 6 droite, 7 haut gauche, 8 haut, 9 haut droite (pav� num�rique)" << endl;
        string choice2;
        cin >> choice2;
        int deplacement = choice2[0] - '0'; // on convertit l'entier entr� ( de type string) en entier
        movePlayer(deplacement, charMap, streumons);
        return true;
    }
    case 's' : { // Sorts choisis
        cout << "Aucun sort." << endl;
        return false;
    }
    case 'i' : { // Inventaire choisi
        cout << "Pas d'inventaire." << endl;
        return false;
    }
    case 'q' : { // Quitter le jeu choisi
        return quitGame();
    }
    }
}


void Oueurj::movePlayer(int deplacement, vector<vector<char>> &charMap, vector<Entity*> &streumons) {
    if (0 < deplacement && deplacement < 10) {
        Pos targetPos = pos + DEPLACEMENTS_POS[deplacement-1]; // Target position is the position of the player plus the vector associated to the movement
        cout << pos << " | " << targetPos << " | " << deplacement << endl;

        if (charMap[targetPos.x][targetPos.y] != '#' && charMap[targetPos.x][targetPos.y] != 'X') { // If it's not a wall
            int monsterIndex = monsterIndexAt(targetPos, streumons); // We check the monster type at the target position (if there is one)
            if ( monsterIndex == -1 ) // Si il n'y a pas de monstre on se d�place
                pos = targetPos;
            else { // Sinon on se bat comme un vrai Oueurj (ou on ajoute le mob au Streum�dex)
                cout << "Monstre " << streumons[monsterIndex]->getType() << " rencontr� !" << endl;
                Combat combat = Combat(*this, *streumons[monsterIndex]);
                combat.startCombat();
                if (this->isAlive()) {
                    pos = targetPos;
                    streumons.erase(streumons.begin() + monsterIndex);
                }
                else
                    cout << "Vous avez �t� vaincu..." << endl;
            }
        }

        else
            cout << "COULDN'T MOVE" << endl;

    }
}

int Oueurj::monsterIndexAt(Pos target, vector<Entity*> &streumons) {
    int i = 0;
    for (Entity* monster : streumons) {
        if (monster->pos == target)
            return i; // monster found, return its type
        i++;
    }
    return -1; // no monster found, return empty char
}

    //////////////////////////////////
    // BELOW ARE THE COMBAT METHODS //
    //////////////////////////////////

bool Oueurj::playCombatTurn(Entity& M) { // Method managing the turn of the player in the combat
    char attackChoice;
    bool turnOver = false;
    while (!turnOver) {
        cout << "Quelle attaque d�sirez-vous lancer ? Attaque normale : n, Attaque puissante : p, Soin : s" << endl;
        cout << "(Il vous reste " << mp << " point(s) de mana)" << endl;
        cin >> attackChoice;

        switch ( attackChoice ) {
        case 'n': {
            attack(M); // Normal attack designed in the Entity class
            return M.isAlive(); // This attack cannot fail so no need to check if it returns true
        }
        case 'p': {
            if ( powerAttack(M) ) // More powerful attack
                return M.isAlive();
        }
        case 's': {
            if ( heal() ) // Heal the player
                return true;
        }
        }
    }
}

bool Oueurj::powerAttack(Entity& M) {
    if (mp > POWERATK_COST) {
        M.inflictDamage(power);
        mp -= POWERATK_COST;
        cout << "Vous infligez une attaque puissante !" << endl
        << "L'ennemi n'a plus que " << M.getHp() << " point(s) de vie" << endl;
        return true;
    }
    else {
        cout << "Pas assez de mana ! Vous avez " << mp << " point(s) de mana (" << POWERATK_COST << " requis)" << endl;
        return false; // Not enough mp
    }
}

bool Oueurj::heal() {
    if (mp > HEAL_COST) {
        hp += power;
        mp -= HEAL_COST;
        if (hp > HP_MAX)
            hp = HP_MAX;
        cout << "Vous vous soignez, vous avez d�sormais " << hp << " points de vie" << endl;
        return true;
    }
    else {
        cout << "Pas assez de mana ! Vous avez " << mp << " point(s) de mana (" << HEAL_COST << " mp requis)" << endl;
        return false; // Not enough mp
    }
}
