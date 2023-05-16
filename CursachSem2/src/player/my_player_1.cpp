#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include "my_player_1.h"

// ����������� ������
GodPlayer::GodPlayer(const char* name) {
	playerName = name;
}

/// @brief ����� ���������� ���� ���.
/// @return ��� ������.
std::string GodPlayer::name() const {
	return playerName;
}

/// @brief ����� �������� �� ���� �����.
/// @param cards ������ ����.
void GodPlayer::receiveCards(const std::vector<const Card*>& cards) { // ����� �������� �����
	for (const Card* card: cards) {
		playerCards.insert(card);
	}
}

/// @brief ������� 2. ���������� ������ � ������� `gameNumber`.
/// ������������� �� ������ ���������� ������ � ���������� ������
/// @param gameNumber ����� ������.
void GodPlayer::handleSetStarted(int gameNumber) { 
	playerCards.clear();
	playerWeaknesses.clear();
	for (int i = 0; i < game()->numberOfPlayers(); i++) {
		playerWeaknesses.push_back(-1);
	}
}

/// @brief ����� ���� � ���� ������� ����� ����� color
/// @param color - ���� ������� �����
/// @return ��������� �� ��������� �����, ��� ������� ���������
const Card* GodPlayer::getHigherCard(CardColor color) { // ����� ������� �����
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

/// @brief ����� ���� ������� � ���� ����� ����� color
/// @param color - ���� ������� �����
/// @return ��������� �� ��������� �����, ��� ������� ���������
const Card* GodPlayer::getZeroCard(CardColor color) { // ����� ����� �� ��������� ����
	for (const Card* card : playerCards) {
		if (card->color == color && card->value == 0) {
			return card;
		}
	}
	return nullptr;
}

/// @brief ����� ���� ����� � ���� �������� value � �����, 
/// ������� ����� "��������" ���������� ������
/// @param value - �������� ������� �����
/// @return ��������� �� ��������� �����, ��� ������� ���������
const Card* GodPlayer::getSameValueCardWithWeakness(int value) { // ����� ����� � ���� � ���������� ��������� � ������, ������� ��������� ��� ���������� ������
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

/// @brief ����� ���� ������ ���������� ����� �������� value
/// @param value - �������� ������� �����
/// @return ��������� �� ��������� �����, ��� ������� ���������
const Card* GodPlayer::getSameValueCard(int value) { // ����� ����� � ���� � ���������� ���������
	for (const Card* card : playerCards) {
		if (card->value == value) {
			return card;
		}
	}
	return nullptr;
}

/// @brief ����� ���������� �����, ������� �� ������� (������� � �����).
/// @return �����, ������� ����� ������� � �����.
const Card* GodPlayer::playCard() {
	const Card* topCard = game()->topCard();
	const Card* playCard = nullptr;
	
	playCard = getSameValueCardWithWeakness(topCard->value);
	playCard = playCard == nullptr ? getZeroCard(game()->currentColor()) : playCard;
	playCard = playCard == nullptr ? getHigherCard(game()->currentColor()) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(topCard->value) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(13) : playCard;
	playCard = playCard == nullptr ? getSameValueCard(14) : playCard;

	// �������� �������� �� �����, ������� ����� ��������
	std::set<const Card*>::iterator itr = std::find(playerCards.begin(), playerCards.end(), playCard);

	// ������� ����� �� ���������
	playerCards.erase(itr);
	return playCard;
}

/// @brief ���� � ������ ��� ���������� ����, �� ����� �������������� 
/// ����� � �������, ����� �� �� �� ����� �������� ��� ���.
/// @param additionalCard �������������� �����.
/// @return true, ���� ����� ����� �� ������ ��� �����, ����� false.
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

/// @brief ���� ����� ������� "������ ����" ��� "������ ������", �� 
/// ���� �������� � ���� ����� ����. �� ���������� ��� ����, ��������
/// � ��� ������ ����� �� ����.
/// @return ����� ����.
CardColor GodPlayer::changeColor() {  // ����� �����
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

/// @brief ����� ��������� "���������" ���� ��� ����������� ������
/// @param name - ��� ������.
/// @return ���� ������ ����� ���, �� ���������� -1,
/// ����� ���������� �������� ��������, ��������������� �����
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

/// @brief ������� 7. ����� ������� �������������� �����.
/// ����������, ����� ���� ������ ������� - ��� � ���� 
/// "������" ����� ����� ������
/// @param playerIndex ����� ������ � ������.
void GodPlayer::handlePlayerDrewAnotherCard(int playerIndex) {
	playerWeaknesses[playerIndex] = game()->currentColor();
}

/// @brief ������� 8. ����� ������� ����� � ��������� ���.
/// ������������� ���� ������� � �������� ������ � "��������"
/// @param playerIndex ����� ������ � ������.
void GodPlayer::handlePlayerDrewAndSkip(int playerIndex, int numberOfCards) {
	playerWeaknesses[playerIndex] = -1;
}

/// @brief ������� 11. ����� �����������������. 
/// ��������� ������ "���������" ������ �� ����,
/// ��� ������ �� ������� ������������������
/// @param playerIndex ����� ������ � ������.
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