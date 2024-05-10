//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(char* _name, int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _attack, _defense, _speed, true) {
    level = 1;
    experience = 0;
    originalDefense = _defense;
}

void Player::doAttack(Character *target) {
    target->takeDamage(attack);
}

void Player::takeDamage(int damage) {
    int trueDamage = damage - getDefense();
    if (trueDamage < 0) {
        trueDamage = 0;
    }

    health-= trueDamage;

    cout << name << " took: " << trueDamage << " damage!" << endl;
    cout << "Remainig health: " + to_string(this->getHealth())<<endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }
}

void Player::levelUp() {
    level++;
    health += 10;
    attack += 10;
    defense += 10;
    speed += 10;

    cout << name << " ha subido al nivel " << level << endl;
    cout << "Estadisticas: " <<  endl;
    cout << "Salud: " << health << ", Ataque: " << attack << ", Defensa: " << defense << ", Velocidad: " << speed << endl;


}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        experience = experience - 100;
        levelUp();
        cout << "Experiencia restante: " << experience << endl;
    }

}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    bool invalid = true;
    do {
        cout << "-------------------------------" << endl;
        cout << "Select a target: " << endl;
        for (int i = 0; i < possibleTargets.size(); i++) {
            cout << i << ". " << possibleTargets[i]->getName() << endl;
        }
        cin >> selectedTarget;
        for (int i = 0; i < possibleTargets.size(); i++) {
            if (selectedTarget == i) {
                invalid = false;
                break;
            }
        }
        if (invalid) {
            cout << "Invalid option" << endl;
        }
    } while (invalid);
    return possibleTargets[selectedTarget];
}

Action Player::takeAction(vector<Enemy*> enemies) {
    int action;
    cout << "Select an action: " << endl
         << "1. Attack" << endl
         << "2. Defend" << endl;
    cin >> action;
    Action currentAction;
    Character* target = nullptr;
    switch(action) {
        case 1:
            target = selectTarget(enemies);
        currentAction.target = target;
        currentAction.action = [this, target](){
            doAttack(target);
            if (target -> getHealth() <= 0)
            {
                int experienciaObtenida = (((Enemy *) target) -> getExperience());
                gainExperience(experienciaObtenida);

            }
        };
        currentAction.speed = getSpeed();
        break;
        case 2:
            currentAction.target = target;
        currentAction.action = [this]()
        {
            Defense();
        };
        currentAction.speed = 999999;
        break;
        default:
            cout << "Invalid action" << endl;
            currentAction.action = nullptr;
            break;
    }


    return currentAction;
}