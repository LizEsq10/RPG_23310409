//
// Created by Victor Navarro on 15/02/24.
//

#include <string.h>
#include "Character.h"
Character::Character(const char _name[], int _health, int _attack, int _defense, int _speed, bool _isPlayer) {
    strcpy (name, _name);
    health = _health;
    attack = _attack;
    defense = _defense;
    speed = _speed;
    isPlayer = _isPlayer;
}

string Character::getName() {
    return name;
}

int Character::getHealth() {
    return health;
}

int Character::getInihealth(){
    return Inihealth;
}

int Character::getAttack() {
    return attack;
}

double Character::getDefense() {
    return isDefending ? defense * 1.20 : defense;
}

int Character::getSpeed() {
    return speed;
}

string Character::toString() {
    string aux = "Name";
    aux.append(name).append("\nHealth: " + to_string(health) + "\nAttack: " + to_string(attack) + "\nDefense: " + to_string(defense) + "\nSpeed: " + to_string(speed));
    return aux;
}

bool Character::getIsPlayer() {
    return isPlayer;
}

bool Character::flee(Character*target) {
    if(this->speed > target->speed)
        return true;

    int chance = rand() % 100;
    return chance > 40;
}

void Character::Defense(){
    defense = std::ceil(defense*1.2);
    isDefending = true;
}