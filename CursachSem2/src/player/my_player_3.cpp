#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include "my_player_3.h"

SimplePlayer_3::SimplePlayer_3(const char* name){ //
	if (name == NULL) {
		setNewName("Undefined");
	}
	setNewName(name);
}

void SimplePlayer_3::setNewName(const char* name) { //
	playerName = name;
}

std::string SimplePlayer_3::name() const { // 
	return playerName;
}


void SimplePlayer_3::receiveCards(const std::vector<const Card*>& cards) { //
	int countCards = cards.size();
	for(int i = 0; i < countCards; i++){
		playerCards.push_back(cards[i]);
	}
}

void SimplePlayer_3::handleSetStarted(int gameNumber) {
	playerCards.clear();
	playerWeakness.clear();
	for (int i = 0; i < game()->numberOfPlayers(); i++) {
		playerWeakness.push_back(-1);
	}
}

int SimplePlayer_3::getIndexOfHigherCard(CardColor color) {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	const Card* playCard = nullptr;
	int index = -1;
	
	// перебираем весь список и ищем старшую карту этого цвета
	while (iter != playerCards.end()) {
		if ((*iter)->color == color && !(*iter)->is_wild()) {
			if (playCard == nullptr || (*iter)->value > playCard->value) {
				playCard = *iter;
				index = (iter - playerCards.begin());
			}
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::getIndexOfZeroCard(CardColor color) {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	// перебираем весь список и ищем старшую карту этого цвета
	while (iter != playerCards.end()) {
		if ((*iter)->color == color && (*iter)->value == 0) {
			return (iter - playerCards.begin());
		}
		iter++;
	}
	return -1;
}

int SimplePlayer_3::getIndexOfSameValueCard(int value) {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	if (value > 9) return -1;
	int index = -1;
	// перебираем весь список и ищем старшую карту этого цвета
	CardColor moreColor = CardColor(WhatColorAreMore());
	int weakness = getWeakness();
	if (weakness != -1) {
		moreColor = CardColor(weakness);
	}
	while (iter != playerCards.end()) {
		// делаем проверку на карту действия или нет
		if ((*iter)->value == value && moreColor == (*iter)->color) {
			index = (iter - playerCards.begin());
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::getIndexOfSameActionCard(int value) {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	int index = -1;
	while (iter != playerCards.end()) {
		// делаем проверку на карту действия или нет
		if ((*iter)->value == value) {
			index = (iter - playerCards.begin());
			break;
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::getIndexOfActionCard(CardColor color) {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	int index = -1;
	while (iter != playerCards.end()) {
		if ((*iter)->is_action() && color == (*iter)->color) {
			// пока возвращаем рандомное значение
			index = iter - playerCards.begin();
			break;
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::getIndexOfActionCard(CardColor color, int value) {
	// получаем итератор на первый элемент
	if (value < 10 && value > 12) return -1;
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	int index = -1;
	while (iter != playerCards.end()) {
		if ((*iter)->value == value && color == (*iter)->color) {
			index = iter - playerCards.begin();
			break;
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::getIndexOfWildCard(int value){
	if (value < 13 && value > 14) return -1;
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	int index = -1;
	while (iter != playerCards.end()) {
		if ((*iter)->value == value) {
			index = iter - playerCards.begin();
			break;
		}
		iter++;
	}
	return index;
}

int SimplePlayer_3::WhatColorAreMore() {
	// получаем итератор на первый элемент
	std::vector<const Card*>::const_iterator iter = playerCards.cbegin();
	int arrColors[4]{};
	while (iter != playerCards.end()) {
		if ((*iter)->is_wild()) {
			iter++;
			continue;
		}
		arrColors[(*iter)->color] += 1;
		iter++;
	}

	int color = 0;
	for (int i = 1; i < 4; i++) {
		if (arrColors[i] > arrColors[color]) {
			color = i;
		}
	}
	if (arrColors[color] == 0) return -1;
	return color;
}

void SimplePlayer_3::printCards() {
	for (auto it = playerCards.begin(); it != playerCards.end(); it++) {
		if (it != playerCards.begin()) {
			std::cout << ", ";
		}
		std::cout << (*it)->value;
		switch ((*it)->color) {
			case 0: std::cout << " RED"; break;
			case 1: std::cout << " GREEN"; break;
			case 2: std::cout << " BLUE"; break;
			case 3: std::cout << " YELLOW"; break;
		}
	}
	std::cout << std::endl;;
}

const Card* SimplePlayer_3::playCard() {

	//printCards();
	const Card* topCard = game()->topCard();
	const Card* playCard = nullptr;
	int indexOfPlayCard = -1;
	// если есть возможность, то лучше сменить цвет, следовательно мы ищем карту с похожим номиналом
	// если такой карты нет, то ищем среди карт с похожим цветом 0
	// или же потом ищем карты с наибольшим номиналом
	
	// возможные ситуации для 1: 0-9, 10-12, 13, 14
	indexOfPlayCard = getIndexOfSameValueCard(topCard->value);
	// возможные ситуации для 2: дикая карта, не дикая карта
	if (!topCard->is_wild() && indexOfPlayCard == -1) {
		// если нет возможности перевести цвет, то ищём 0
		indexOfPlayCard = getIndexOfZeroCard(game()->currentColor());
	}
	// возможные ситуации для 3: дикая карта, не дикая карта
	if (indexOfPlayCard == -1) {
		// если нет 0, то избавляемся от старшей карты
		indexOfPlayCard = getIndexOfHigherCard(game()->currentColor());
	}
	// сыграть карту действия
	if (indexOfPlayCard == -1) {
		indexOfPlayCard = getIndexOfActionCard(game()->currentColor());
	}

	if (indexOfPlayCard == -1) {
		indexOfPlayCard = getIndexOfSameActionCard(topCard->value);
	}

	if (indexOfPlayCard == -1) {
		indexOfPlayCard = getIndexOfWildCard(13);
	}

	if (indexOfPlayCard == -1) {
		indexOfPlayCard = getIndexOfWildCard(14);
	}

	// надо сделать розыгрыш диких карт, а также карт действия
	// если предыдущий игрок набирал карты, то развернуть игру вспять
	// если следующий игрок набирал карты определённого цвета, то сменить цвет на этот


	// что нужно ещё в идеале делать, так это смотреть за тем, какого цвета карты берут люди
	// можно также считать карты, которые вышли из игры

	playCard = playerCards[indexOfPlayCard];
	playerCards.erase(playerCards.begin() + indexOfPlayCard);
	return playCard;
}

bool SimplePlayer_3::drawAdditionalCard(const Card* additionalCard) {
	if (game()->currentColor() == additionalCard->color) {
		return true;
	}
	else if (game()->topCard()->value == additionalCard->value) {
		return true;
	}
	else if (additionalCard->is_wild()) {
		return true;
	}
	else {
		playerCards.push_back(additionalCard);
		return false;
	}
}

CardColor SimplePlayer_3::changeColor() {
	//меняем на цвет, которого у нас больше всего
	// считаем карты по цветам
	// если выбираем карту с большим
	return CardColor(WhatColorAreMore());
}

int SimplePlayer_3::getWeakness() {
	int offset = 1;
	if (game()->currentDirection() == Inverse) {
		offset = -1;
		if (playerIndex() + offset < 0) {
			return playerWeakness[playerWeakness.size() - 1];
		}
	}
	else if (playerIndex() + offset >= playerWeakness.size()) {
		return playerWeakness[0];
	}
	return playerWeakness[playerIndex() + offset];
}

void SimplePlayer_3::handlePlayerDrewAnotherCard(int playerIndex) {
	playerWeakness[playerIndex] = game()->currentColor();
}

void SimplePlayer_3::handlePlayerDrewAndSkip(int playerIndex, int numberOfCards) {
	playerWeakness[playerIndex] = -1;
}

void SimplePlayer_3::handlePlayerDisqualified(int playerIndex, int handScore, const Card* card) {
	std::cout << "Disqualified" << std::endl;
	std::vector<int> new_playerWeakness;
	for (int i = 0; i < playerWeakness.size() - 1; i++) {
		new_playerWeakness.push_back(-1);
	}

	int offset = 0;
	for (int i = 0; i < playerWeakness.size(); i++) {
		if (i != playerIndex) {
			new_playerWeakness[i - offset] = playerWeakness[i];
		}
		else {
			offset = 1;
		}
	}
	playerWeakness = new_playerWeakness;
}