#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include "my_player_1.h"

// конструктор игрока
GodPlayer::GodPlayer(const char* name) {
	playerName = name;
}

/// @brief Игрок возвращает свое имя.
/// @return имя игрока.
std::string GodPlayer::name() const {
	return playerName;
}

/// @brief Игрок получает на руки карты.
/// @param cards массив карт.
void GodPlayer::receiveCards(const std::vector<const Card*>& cards) { // Игрок получает карты
	for (const Card* card: cards) {
		playerCards.insert(card);
	}
}

/// @brief Событие 2. Начинается партия с номером `gameNumber`.
/// Следовательно мы должны подчистить данные с предыдущей партии
/// @param gameNumber номер партии.
void GodPlayer::handleSetStarted(int gameNumber) { 
	playerCards.clear();
	playerWeaknesses.clear();
	for (int i = 0; i < game()->numberOfPlayers(); i++) {
		playerWeaknesses.push_back(-1);
	}
}

/// @brief Игрок ищет в руке старшую карту цвета color
/// @param color - цвет искомой карты
/// @return указатель на найденную карту, или нулевой указатель
const Card* GodPlayer::getHigherCard(CardColor color) { // Поиск старшей карты
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

/// @brief Игрок ищет нулевую в руке карту цвета color
/// @param color - цвет искомой карты
/// @return указатель на найденную карту, или нулевой указатель
const Card* GodPlayer::getZeroCard(CardColor color) { // Поиск карты со значением ноль
	for (const Card* card : playerCards) {
		if (card->color == color && card->value == 0) {
			return card;
		}
	}
	return nullptr;
}

/// @brief Игрок ищет карту в руке значения value и цвета, 
/// который будет "неудобен" следующему игроку
/// @param value - значение искомой карты
/// @return указатель на найденную карту, или нулевой указатель
const Card* GodPlayer::getSameValueCardWithWeakness(int value) { // Поиск карты в руке с конкретным значением и цветом, который невыгоден для следующего игрока
	if (value > 9) return nullptr;

	CardColor moreColor = changeColor();
	int weakness = getWeakness();
	if (weakness != -1) {
		moreColor = CardColor(weakness);
	}

	for (const Card* card : playerCards) {
		if (card->value == value && moreColor == card->color) {
			return card;
		}
	}

	return nullptr;
}

/// @brief Игрок ищет первую попавшуюся карту значения value
/// @param value - значение искомой карты
/// @return указатель на найденную карту, или нулевой указатель
const Card* GodPlayer::getSameValueCard(int value) { // Поиск карты в руке с конкретным значением
	for (const Card* card : playerCards) {
		if (card->value == value) {
			return card;
		}
	}
	return nullptr;
}

/// @brief Игрок возвращает карту, которую он сыграет (положит в сброс).
/// @return карта, которую игрок положит в сброс.
const Card* GodPlayer::playCard() {
	const Card* topCard = game()->topCard();
	const Card* playCard = nullptr;
	
	playCard = getSameValueCardWithWeakness(topCard->value);
	playCard = playCard == nullptr ? getZeroCard(game()->currentColor()) : playCard;
	playCard = playCard == nullptr ? getHigherCard(game()->currentColor()) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(topCard->value) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(13) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(14) : playCard;

	// получаем итератор на карту, которую хотим сбросить
	std::set<const Card*>::iterator itr = std::find(playerCards.begin(), playerCards.end(), playCard);

	// удаляем карту из множества
	playerCards.erase(itr);
	return playCard;
}

/// @brief Если у игрока нет подходящих карт, он тянет дополнительную 
/// карту и говорит, хочет ли он ее сразу положить или нет.
/// @param additionalCard дополнительная карта.
/// @return true, если игрок сразу же кладет эту карту, иначе false.
bool GodPlayer::drawAdditionalCard(const Card* additionalCard) {
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
		playerCards.insert(additionalCard);
		return false;
	}
}

/// @brief Если игрок положил "Закажи цвет" или "Возьми четыре", то 
/// игра запросит у него новый цвет. Мы возвращаем тот цвет, которого
/// у нас больше всего на руке.
/// @return новый цвет.
CardColor GodPlayer::changeColor() {  // Смена цвета
	int arrColors[4]{};
	for (const Card* card : playerCards) {
		if (!card->is_wild()) {
			arrColors[card->color] += 1;
		}
	}

	int color = 0;
	for (int i = 1; i < 4; i++) {
		if (arrColors[i] > arrColors[color]) {
			color = i;
		}
	}
	return CardColor(color);
}

/// @brief Игрок проверяет "неудобный" цвет для следующуего игрока
/// @param name - имя игрока.
/// @return если такого цвета нет, то возвращает -1,
/// иначе возвращает числовое значение, соответствующее цвету
int GodPlayer::getWeakness() {
	int offset = 1;
	if (game()->currentDirection() == Inverse) {
		offset = -1;
		if (playerIndex() + offset < 0) {
			return playerWeaknesses[playerWeaknesses.size() - 1];
		}
	}
	else if (playerIndex() + offset >= playerWeaknesses.size()) {
		return playerWeaknesses[0];
	}
	return playerWeaknesses[playerIndex() + offset];
}

/// @brief Событие 7. Игрок получил дополнительную карту.
/// Запоминаем, какой цвет сейчас текущий - это и есть 
/// "слабое" место этого игрока
/// @param playerIndex номер игрока в списке.
void GodPlayer::handlePlayerDrewAnotherCard(int playerIndex) {
	playerWeaknesses[playerIndex] = game()->currentColor();
}

/// @brief Событие 8. Игрок получил карты и пропустил ход.
/// Следовательно надо вернуть к исходным данные о "слабости"
/// @param playerIndex номер игрока в списке.
void GodPlayer::handlePlayerDrewAndSkip(int playerIndex, int numberOfCards) {
	playerWeaknesses[playerIndex] = -1;
}

/// @brief Событие 11. Игрок дисквалифицирован. 
/// Обновляем массив "слабостей" исходя из того,
/// что одного из игроков дисквалифицировали
/// @param playerIndex номер игрока в списке.
void GodPlayer::handlePlayerDisqualified(int playerIndex, int handScore, const Card* card) {
	std::vector<int> new_playerWeakness;
	for (int i = 0; i < playerWeaknesses.size() - 1; i++) {
		new_playerWeakness.push_back(-1);
	}

	int offset = 0;
	for (int i = 0; i < playerWeaknesses.size(); i++) {
		if (i != playerIndex) {
			new_playerWeakness[i - offset] = playerWeaknesses[i];
		}
		else {
			offset = 1;
		}
	}
	playerWeaknesses = new_playerWeakness;
}