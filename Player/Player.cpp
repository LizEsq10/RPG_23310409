//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(const char _name[], int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _attack, _defense, _speed, true) {
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

    cout << name << " took " << trueDamage << " damage!" << endl;
    cout << "remainig health" + to_string(this->getHealth())<<endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }
}

void Player::levelUp() {
    level++;
    int ExperienciaRequerida = 10;

    experience -= ExperienciaRequerida;

    health += 5;
    attack += 5;
    defense += 5;
    speed += 5;
    cout << name << " ha subido al nivel " << level << "!" << endl;
    cout << "Nuevas estadísticas: Salud: " << health << ", Ataque: " << attack << ", Defensa: " << defense << ", Velocidad: " << speed << endl;
    cout << "Nivel de experiencia actual: " << experience << endl;

    if (experience >= ExperienciaRequerida) {
        cout << "Tienes experiencia para subir de nivel" << endl;
        levelUp();
    }
}

void Player::gainExperience(int exp) {
    experience += exp;
    int ExperienciaRequerida = 10;
    if (experience >= ExperienciaRequerida) {
        levelUp();
    }
}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    cout << "Select a target: " << endl;
    for (int i = 0; i < possibleTargets.size(); i++) {
        cout << i << ". " << possibleTargets[i]->getName() << endl;
    }

    //TODO: Add input validation
    cin >> selectedTarget;
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
    int originalDefense = defense;
    switch(action) {
        case 1:
            // Utilizar al enemigo seleccionado al principio del combate como objetivo
            if (!enemies.empty()) {
                target = enemyselect;
            } else {
                cout << "No hay enemigos disponibles." << endl;
                currentAction.action = nullptr;
                return currentAction;
            }
            currentAction.target = target;
            currentAction.action = [this, target](){
                doAttack(target);
            };
            currentAction.speed = getSpeed();
            break;
        case 2:
            Defense();

            if (!enemies.empty()) {
                target = enemies[0];
            }
            if (target) {
                currentAction.target = target;
                currentAction.action = [this, target, originalDefense](){
                    // Restablecer la defensa al valor original después de defenderse
                    defense = originalDefense;
                };
                currentAction.speed = target->getSpeed();
            } else {
                cout << "No hay enemigos disponibles." << endl;
                currentAction.action = nullptr;
            }
            break;
        default:
            cout << "Invalid action" << endl;
            currentAction.action = nullptr;
            break;
    }


    return currentAction;
}