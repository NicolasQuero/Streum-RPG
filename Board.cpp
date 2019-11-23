#include <iostream>
#include "Oueurj.h"
#include "Board.h"
#include "Streumons/Streumon.h"
#include "Streumons/Monster_s.h"
#include "Streumons/Monster_o.h"
#include <vector>
#include <algorithm>

using namespace std;

bool Board::gameOn = true;

Board::Board(vector<string> mapStrings) { // each string from the list must have the same length
    setCharMaps(mapStrings); // stores the value of each character from mapText in charMap
    J = Oueurj(2, 3);
    setPlayer(J);
}

void Board::setCharMaps(vector<string> mapStrings) {
    int i = 0; // Coordonn�es i,j d'un caract�re parcouru
    int j = 0;
    for (string line : mapStrings) { // On parcourt mapStrings avec line
        vector<char> charLine;
        for (char c : line) { // Pour chaque caract�re de line
            if (find(monsterPossibleLetters.begin(), monsterPossibleLetters.end(), c) != monsterPossibleLetters.end()) { // Si la lettre r�pr�sente un monstre i.e. est contenue dans monsterLetters
                addMonster(c, i, j); // On ajoute un nouveau monstre associ� � c avec addMonster() dans monstersOnMap
                charLine.push_back(' '); // On ajoute la case vide de la map derri�re le monstre dans charLine
            }
            else {
                //monsterLine.push_back(' '); // Sinon on ajoute un caract�re vide dans monsterLine
                charLine.push_back(c); // Et le caract�re dans la charLine
            }
            j++;
        }
        charMap.push_back(charLine); // On met la ligne charLine dans charMap
        i++; j = 0;
    }
}

void Board::addMonster(char c, int i, int j) {
    switch ( c ) {
        case 's': {
            Monster_s* Ms = new Monster_s(i, j);
            monstersOnMap.push_back(Ms); //->push_back(new Monster_s(i, j));
            break;
        }
        case 'o': {
            Monster_o* Mo = new Monster_o(i, j);
            monstersOnMap.push_back(Mo);
            break;
        }
    }
}



void Board::setPlayer(Oueurj &Joueur) {
    playerPos = Pos(Joueur.pos.x, Joueur.pos.y);
}

char Board::getCharAt(int &x, int &y) const { return charMap[x][y]; }

void Board::printMap() const {
    cout << playerPos.x << ", " << playerPos.y << endl;
    for (Streumon* monster : monstersOnMap) {
        cout << monster->pos.x << ", " << monster->pos.y << " " << monster->getType() << endl;
    }
    int x = 0; int y = 0;
    for (vector<char> line : charMap) {
        for (char c : line) {
            bool monsterFound = false;
            for (Streumon* monster : monstersOnMap) {
                if (monster->pos.x == x && monster->pos.y == y) {
                    cout << monster->getType();
                    monsterFound = true;
                    break;
                }
            }
            if (!monsterFound && x == playerPos.x && y == playerPos.y) // Si le joueur est � la position rendue on l'affiche
                cout << 'J';
            else if (!monsterFound) // Sinon on affiche la map
                cout << c;
            y++;
        }
        x++;
        y = 0;
        cout << endl;
    }
}

bool Board::playTurn() {

    J.act(charMap, monstersOnMap);
}



Board::~Board() {
    for (Streumon* monster : monstersOnMap) {
        delete monster;
    }
    vector<Streumon*>().swap(monstersOnMap); // On lib�re la m�moire de monstersOnMap
}

