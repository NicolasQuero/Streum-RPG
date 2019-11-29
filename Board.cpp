#include <iostream>
#include "Oueurj.h"
#include "GameMap.h"
#include "Board.h"
#include "Streumons/Streumon.h"
#include "Streumons/Monster_s.h"
#include "Streumons/Monster_o.h"
#include <vector>
#include <algorithm>

using namespace std;

bool Board::gameOn = true;

Board::Board(GameMap &gamemap) : gamemap(gamemap), score(0) { // each string from the list must have the same length
    setCharMaps(gamemap.getMapStrings()); // stores the value of each character from mapText in charMap
    J = Oueurj(2, 3);
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
    J = Joueur;
}

char Board::getCharAt(int &x, int &y) const { return charMap[x][y]; }

void Board::printMap() const {
    /*cout << J.pos.x << ", " << J.pos.y << endl;
    for (Streumon* monster : monstersOnMap) {
        cout << monster->pos.x << ", " << monster->pos.y << " " << monster->getType() << endl;
    }*/
    int row = 0; int col = 0;
    for (vector<char> line : charMap) {
        //printCoordinatesAroundTheMap(charMap, row, col); // print coordinates around the map
        for (char c : line) {
            bool monsterFound = false;
            for (Streumon* monster : monstersOnMap) {
                if (monster->pos.x == row && monster->pos.y == col) { // a monster was found!
                    cout << monster->getType();// << "  ";
                    monsterFound = true;
                    break; // so we print the monster and no need to print the rest
                }
            }
            if (!monsterFound && row == J.pos.x && col == J.pos.y) // Si le joueur est � la position rendue on l'affiche
                cout << 'J';// << "  ";
            else if (!monsterFound) { // Sinon on affiche la map
                if (c == '#')
                    cout << c;// << c << c;
                else
                    cout << c;// << "  ";
            }
            col++;
        }
        printInformation(row);
        row++;
        col = 0;
        cout << endl;
    }
}

void Board::printCoordinatesAroundTheMap(vector<vector<char>> charMap, int row, int col) const {
    if (row == 0 && col == 0) { // We print the column indexes over the map
        cout << "   "; // mise en page
        for (char c : charMap[0]) {
            if (col < 10) {
                cout << col << "  "; // If the index of the column is 1 char long we print it with a space behind
                col++;
            }
            else {
                cout << col << " "; // If it's 2 chars long we print it without space behind
                col++;
            }
        }
        cout << endl << 0 << "  "; // after printing the top line we break the line and print the first 0 for the rows
    }
    else if (col == 0) {
        if (row < 10)
            cout << row << "  ";
        else
            cout << row << " ";
    }
}

void Board::printInformation(int &row) const {
    switch ( row ) {
    case 0:
        cout << "    Score : " << score;
        break;
    case 2:
        cout << "    HP : " << J.getHp();
        break;
    case 4:
        cout << "    T�l�portations : " << J.getTeleportsLeft();
    }
}

bool Board::playTurn() {
    J.act(charMap, monstersOnMap);
    for (Streumon* monstre : monstersOnMap) {
        monstre->act(J, charMap, monstersOnMap);
    }
    if (!J.isAlive()) {
        for (int i = 0; i<5; i++)
            cout << " ********** VOUS �TES MORT ! ********** " << endl;
        gameOn = false;
        return false;
    }

    return true;
}



Board::~Board() {
    for (Streumon* monster : monstersOnMap) {
        delete monster;
    }
    vector<Streumon*>().swap(monstersOnMap); // On lib�re la m�moire de monstersOnMap
}

