#include "StupidPlayer.h"

/// @brief Создание игрока
/// @param name - имя игрока.
StupidPlayer::StupidPlayer(string const name)
{
	Player_name = name;
}

/// @brief Игрок возвращает свое имя.
/// @return имя игрока.
string StupidPlayer::name() const
{
	return Player_name;
}

/// @brief Игрок получает на руки карты.
/// @param cards массив карт.
void StupidPlayer::receiveCards(const vector<const Card*>& cards)
{
	for (const Card* cur_card : cards) this->cards.push_back(cur_card);
}

/// @brief Событие 2. Начинается партия с номером `gameNumber`.
/// @param gameNumber номер партии.
void StupidPlayer::handleSetStarted(int gameNumber) {
	cards.clear();
}

/// @brief Игрок возвращает карту, которую он сыграет (положит в сброс).
/// @return карта, которую игрок положит в сброс.
const Card* StupidPlayer::playCard()
{
	const Card* current_card = this->game()->topCard();
	const Card* card = nullptr;

	if (!current_card->is_wild())
	{
		for (const Card* cur_card : this->cards)
		{
			if ((current_card->color == cur_card->color ||
				current_card->value == cur_card->value) && !cur_card->is_wild())
			{
				card = cur_card;
				break;
			}
			else if (cur_card->value == Wild)
			{
				card = cur_card;
				break;
			}
			else if (CouldPlayWD4(cur_card) && cur_card->value == WildDraw4) {
				card = cur_card;
				break;
			}
		}
	}
	else if (current_card->is_wild())
	{
		for (const Card* cur_card : this->cards)
		{
			if (this->game()->currentColor() == cur_card->color && !cur_card->is_wild())
			{
				card = cur_card;
				break;
			}
			else if (cur_card->value == Wild)
			{
				card = cur_card;
				break;
			}
			else if (CouldPlayWD4(cur_card) && cur_card->value == WildDraw4) {
				card = cur_card;
				break;
			}
		}
	}

	std::vector<const Card*>::iterator itr = std::find(cards.begin(), cards.end(), card);
	cards.erase(itr);
	return card;
}

/// @brief Игрок проверяет, может ли он сыграть дикую карту "возьми 4"
/// @param карта, которая лежит на верхушке сброса
/// @return true, если да, false, усли нет
bool StupidPlayer::CouldPlayWD4(const Card* card) {
	if (card->value == WildDraw4)
	{
		for (const Card* cr : this->cards)
		{
			if (cr->color == this->game()->currentColor() && !cr->is_wild()) { return false; }
		}
	}
	return true;
}

/// @brief Если у игрока нет подходящих карт, он тянет дополнительную 
/// карту и говорит, хочет ли он ее сразу положить или нет.
/// @param additionalCard дополнительная карта.
/// @return true, если игрок сразу же кладет эту карту, иначе false.
bool StupidPlayer::drawAdditionalCard(const Card* additionalCard)
{
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
		cards.push_back(additionalCard);
		return false;
	}
}

/// @brief Событие 2. Начинается партия с номером `gameNumber`.
/// @param gameNumber номер партии.
/// @return случайный цвет
CardColor StupidPlayer::changeColor()
{
	srand(time(NULL));
	return CardColor(rand() % 4);
}