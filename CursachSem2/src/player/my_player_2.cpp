#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include "my_player_2.h"

SimplePlayer_2::SimplePlayer_2(const char* name){
	if (name == NULL) {
		setNewName("Undefined");
	}
	setNewName(name);
}

void SimplePlayer_2::setNewName(const char* name) {
	playerName = name;
}

std::string SimplePlayer_2::name() const {
	return playerName;
}

void SimplePlayer_2::receiveCards(const std::vector<const Card*>& cards) {
	for(const Card* card: cards){
		playerCards.push_back(card);
	}
}

void SimplePlayer_2::handleSetStarted(int gameNumber) {
	playerCards.clear();
	playerWeakness.clear();
	for (int i = 0; i < game()->numberOfPlayers(); i++) {
		playerWeakness.push_back(-1);
	}
}

const Card* SimplePlayer_2::getIndexOfHigherCard(CardColor color) {
	const Card* playCard = nullptr;

	for (const Card* card : playerCards) {
		if (card->color == color && !card->is_wild()) {
			if (playCard == nullptr || card->value > playCard->value) {
				playCard = card;
			}
		}
	}
	return playCard;
}

const Card* SimplePlayer_2::getIndexOfZeroCard(CardColor color) {
	for (const Card* card : playerCards) {
		if (card->color == color && card->value == 0) {
			return card;
		}
	}
	return nullptr;
}

const Card* SimplePlayer_2::getIndexOfSameValueCard(int value) {
	const Card* playCard = nullptr;
	CardColor moreColor = Red;
	int weakness = getWeakness();
	if (weakness != -1) {
		moreColor = CardColor(weakness);
	}
	else {
		moreColor = WhatColorAreMore();
	}
	/*CardColor moreColor = changeColor();*/
	for (const Card* card : playerCards) {
		if (card->value == value && (moreColor == card->color || playCard == nullptr)) {
			playCard = card;
		}
	}
	return playCard;
}

const Card* SimplePlayer_2::getIndexOfWildCard(int value){
	for (const Card* card : playerCards) {
		if (card->value == value && card->is_wild()) {
			return card;
		}
	}
	return nullptr;
}

CardColor SimplePlayer_2::WhatColorAreMore() {
	// получаем итератор на первый элемент
	int arrColors[4]{};

	for (const Card* card: playerCards) {
		if (card->is_wild()) {
			continue;
		}
		arrColors[card->color] += 1;
	}

	int color = 0;
	for (int i = 1; i < 4; i++) {
		if (arrColors[i] > arrColors[color]) {
			color = i;
		}
	}
	return CardColor(color);
}

void SimplePlayer_2::printCards() {
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

const Card* SimplePlayer_2::playCard() {

	//printCards();
	const Card* topCard = game()->topCard();
	const Card* playCard = nullptr;
	// если есть возможность, то лучше сменить цвет, следовательно мы ищем карту с похожим номиналом
	// если такой карты нет, то ищем среди карт с похожим цветом 0
	// или же потом ищем карты с наибольшим номиналом
	
	// возможные ситуации для 1: 0-9, 10-12, 13, 14

	if (topCard->value < 10) {
		playCard = getIndexOfSameValueCard(topCard->value);
	}
	if (playCard == nullptr) {
		// если нет возможности перевести цвет, то ищём 0
		playCard = getIndexOfZeroCard(game()->currentColor());
	}
	// возможные ситуации для 3: дикая карта, не дикая карта
	if (playCard == nullptr) {
		// если нет 0, то избавляемся от старшей карты
		playCard = getIndexOfHigherCard(game()->currentColor());
	}

	if (playCard == nullptr) {
		playCard = getIndexOfSameValueCard(topCard->value);
	}

	if (playCard == nullptr) {
		playCard = getIndexOfWildCard(14);
	}

	if (playCard == nullptr) {
		playCard = getIndexOfWildCard(13);
	}

	// надо сделать розыгрыш диких карт, а также карт действия
	// если предыдущий игрок набирал карты, то развернуть игру вспять
	// если следующий игрок набирал карты определённого цвета, то сменить цвет на этот


	// что нужно ещё в идеале делать, так это смотреть за тем, какого цвета карты берут люди
	// можно также считать карты, которые вышли из игры

	std::vector<const Card*>::iterator itr = std::find(playerCards.begin(), playerCards.end(), playCard);
	playerCards.erase(itr);
	return playCard;
}

bool SimplePlayer_2::drawAdditionalCard(const Card* additionalCard) {
	//std::cout << additionalCard->value << " " << additionalCard->color << std::endl;

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

CardColor SimplePlayer_2::changeColor() {
	// меняем на цвет, которого у нас больше всего
	// считаем карты по цветам
	//int weakness = getWeakness();
	//if (weakness != -1 && CouldChangeColor(CardColor(weakness))) {
	//	return CardColor(weakness);
	//}
	return WhatColorAreMore();
}

int SimplePlayer_2::getWeakness() {
	int offset = 1;
	if (game()->currentDirection() == Inverse) {
		offset = -1;
		if (playerIndex() + offset < 0) {
			return playerWeakness[playerWeakness.size()-1];
		}
	}
	else if (playerIndex() + offset >= playerWeakness.size()) {
		return playerWeakness[0];
	}
	return playerWeakness[playerIndex() + offset];
}

bool SimplePlayer_2::CouldChangeColor(CardColor color) {
	const Card* topCard = game()->topCard();

	for (const Card* card : playerCards) {
		if (card->value == topCard->value && color == card->color && !topCard->is_wild()) {
			return true;
		}
	}
	return false;
}

void SimplePlayer_2::handlePlayerDrewAnotherCard(int playerIndex) {
	playerWeakness[playerIndex] = game()->currentColor();
}

void SimplePlayer_2::handlePlayerDrewAndSkip(int playerIndex, int numberOfCards) {
	playerWeakness[playerIndex] = -1;
}

void SimplePlayer_2::handlePlayerDisqualified(int playerIndex, int handScore, const Card* card) {
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