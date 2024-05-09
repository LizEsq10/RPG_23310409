//
// Created by Victor Navarro on 19/02/24.
//
#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cstring>

using namespace std;

bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant : participants) {
        if (participant->getIsPlayer()) {
            partyMembers.push_back((Player *) participant);
        } else {
            enemies.push_back((Enemy *) participant);
        }
    }
}

Combat::Combat(vector<Player*> _partyMembers, vector<Enemy*> _enemies) {
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character*>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()){
        partyMembers.push_back((Player*) participant);
    } else {
        enemies.push_back((Enemy*) participant);
    }
}

void Combat::combatPrep() {
    // Sort participants by speed
    sort(participants.begin(), participants.end(), compareSpeed);
}

char* Combat::toString() {
    // Crear un buffer para almacenar la representación en cadena de los participantes
    char buffer[1024]; // Selecciona un tamaño adecuado para el buffer

    // Inicializar el buffer
    buffer[0] = '\0';

    // Iterar sobre los participantes y concatenar sus representaciones en cadena al buffer
    for (auto it = participants.begin(); it != participants.end(); ++it) {
        // Concatenar la representación en cadena del participante actual al buffer
       // strcat(buffer, (*it)->toString());
        strcat(buffer, "\n");
    }

    // Agregar un separador al final del buffer
    strcat(buffer, "====================\n");

    // Devolver el buffer como un arreglo de caracteres
    return buffer;
}

Character* Combat::getTarget(Character* attacker) {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        if((*it)->getIsPlayer() != attacker->getIsPlayer()){
            return *it;
        }
    }
    //TODO: Handle this exception
    return nullptr;
}

void Combat::chooseEnemy() {
    cout << "Elige con qué enemigo deseas pelear:" << endl;
    for (int i = 0; i < enemies.size(); ++i) {
        cout << i+1 << ". " << enemies[i]->getName() << endl;
    }
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= enemies.size()) {
        // Establecer el enemigo seleccionado para el jugador
        partyMembers[0]->setSelectedEnemy(enemies[choice-1]);
        // Actualizar la lista de participantes solo con el jugador y el enemigo seleccionado
        participants.clear();  // Limpiar la lista de participantes
        participants.push_back(partyMembers[0]);  // Agregar al jugador
        enemyselect = enemies[choice-1];  // Almacenar el enemigo seleccionado
        participants.push_back(enemyselect);  // Agregar al enemigo seleccionado
    } else {
        cout << "Opción inválida. Se iniciará el combate con todos los participantes." << endl;
        // No se ha seleccionado ningún enemigo, por lo que se peleará con todos los enemigos
        enemyselect = nullptr;
    }
}

void Combat::doCombat() {
    cout << "Inicio del combate" << endl;
    chooseEnemy();

    // Mostrar los parámetros de vida, defensa y ataque de todos los participantes al inicio del combate
    cout << "=== Participant Status: ===" << endl;
    for (auto participant : participants) {
        cout << participant->toString() << endl;
    }
    cout << "" << endl;

    combatPrep();
    int round = 1;
    //Este while representa las rondas del combate
    while(enemies.size() > 0 && partyMembers.size() > 0) {
        cout<<"Round " << round << endl;
        vector<Character*>::iterator it = participants.begin();
        registerActions(it);
        executeActions(it);

        // Mostrar los parámetros de vida, defensa y ataque de todos los participantes al final de cada ronda
        cout << "=== Participant Status: ===" << endl;
        for (auto participant : participants) {
            cout << participant->toString() << endl;
        }
        cout << "" << endl;

        round++;

        // Si se ha derrotado al enemigo seleccionado, salir del bucle de combate
        if (enemyselect && enemyselect->getHealth() <= 0) {
            Player* player = nullptr;
            // Encontrar al jugador en la lista de participantes
            for (auto participant : participants) {
                if (participant->getIsPlayer()) {
                    player = dynamic_cast<Player*>(participant);
                    break;
                }
            }

            if (player) {
                // Ganar experiencia por derrotar al enemigo
                player->gainExperience(enemyselect->getExperience());
            }

            // Eliminar al enemigo derrotado de la lista de enemigos
            //enemies.erase(remove(enemies.begin(), enemies.end(), selectedEnemy), enemies.end());
            // Preguntar al jugador si desea continuar peleando o salir solo si aun hay enemigos
            if(enemies.empty()) {
                break;
            }
            char choice;
            cout << "¿Deseas continuar peleando? (y/n): ";
            cin >> choice;

            if (choice != 'y' && choice != 'Y') {
                // El jugador decide salir del combate
                break;
            } else {
                // Mostrar la lista actualizada de enemigos restantes
                cout << "=== Enemigos restantes: ===" << endl;
                for (auto enemy : enemies) {
                    cout << enemy->getName() << " (Health: " << enemy->getHealth() << ")" << endl;
                }
                cout << "" << endl;
                // Permitir al jugador seleccionar un nuevo enemigo
                chooseEnemy();
            }
        }
    }

    if (enemyselect && enemyselect->getHealth() <= 0) {
        cout << "You win!" << endl;
    } else {
        cout << "You lose!" << endl;
    }
}

void Combat::executeActions(vector<Character*>::iterator participant) {
    while(!actionQueue.empty()) {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        //Check if there are any dead characters
        checkParticipantStatus(*participant);
        checkParticipantStatus(currentAction.target);
    }
}

void Combat::checkParticipantStatus(Character *participant) {
    if(participant != nullptr && participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        } else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::registerActions(vector<Character*>::iterator participantIterator) {
    //Este while representa el turno de cada participante
    //La eleccion que cada personaje elije en su turno
    while(participantIterator != participants.end()) {
        if((*participantIterator)->getIsPlayer()) {
            Action playerAction = ((Player*) *participantIterator)->takeAction(enemies);
            // Si el jugador eligió atacar, establecer al enemigo seleccionado como objetivo
            if (playerAction.action != nullptr && enemyselect != nullptr) {
                playerAction.target = enemyselect;
            }
            actionQueue.push(playerAction);
        } else {
            // El enemigo elige su acción
            Action enemyAction = ((Enemy*)*participantIterator)->takeAction(partyMembers);
            actionQueue.push(enemyAction);
            participantIterator++;
            continue;
        }
        participantIterator++;
    }
}
