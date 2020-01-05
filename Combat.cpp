#include <iostream>
#include "Entities/Oueurj.h"
#include "Entities/Streumons/Streumon.h"
#include "Entities/Pos.h"
#include "Entities/Entity.h"
#include "Combat.h"

Combat::Combat(Entity &J, Entity &M) : J(J), M(M), keepFighting(true) {

}

bool Combat::playCombatTurn() {
    return true;
    //J->playCombatTurn()
}

bool Combat::startCombat() {
    cout << "Le combat entre " << J.getType() << " et " << M.getType() << " commence !" << endl << endl;
    cout << J.getType() << " a " << J.getHp() << " HP." << endl;
    cout << M.getType() << " a " << M.getHp() << " HP." << endl;
    cout << "-----------------------" << endl;
    cout << "-                     -" << endl;
    cout << "-                     -" << endl;
    cout << "-      " << J.getType() << "       " << M.getType() << "      -" << endl;
    cout << "-                     -" << endl;
    cout << "-   " << "HP : " << J.getHp() << "   HP : " << M.getHp() << "   -" << endl;
    cout << "-                     -" << endl;
    cout << "-                     -" << endl;
    cout << "-----------------------" << endl;

    while (keepFighting) {
        keepFighting = J.playCombatTurn(M); // playCombatTurn returns false if E died hence we stop the fight
        if ( keepFighting )
            keepFighting = M.playCombatTurn(J);
    }
    return J.isAlive();
}
