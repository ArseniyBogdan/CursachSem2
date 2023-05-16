#include "StupidPlayer.h"

/// @brief �������� ������
/// @param name - ��� ������.
StupidPlayer::StupidPlayer(string const name)
{
	Player_name = name;
}

/// @brief ����� ���������� ���� ���.
/// @return ��� ������.
string StupidPlayer::name() const
{
	return Player_name;
}

/// @brief ����� �������� �� ���� �����.
/// @param cards ������ ����.
void StupidPlayer::receiveCards(const vector<const Card*>& cards)
{
	for (const Card* cur_card : cards) this->cards.push_back(cur_card);
}

/// @brief ������� 2. ���������� ������ � ������� `gameNumber`.
/// @param gameNumber ����� ������.
void StupidPlayer::handleSetStarted(int gameNumber) {
	cards.clear();
}

/// @brief ����� ���������� �����, ������� �� ������� (������� � �����).
/// @return �����, ������� ����� ������� � �����.
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

/// @brief ����� ���������, ����� �� �� ������� ����� ����� "������ 4"
/// @param �����, ������� ����� �� �������� ������
/// @return true, ���� ��, false, ���� ���
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

/// @brief ���� � ������ ��� ���������� ����, �� ����� �������������� 
/// ����� � �������, ����� �� �� �� ����� �������� ��� ���.
/// @param additionalCard �������������� �����.
/// @return true, ���� ����� ����� �� ������ ��� �����, ����� false.
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

/// @brief ������� 2. ���������� ������ � ������� `gameNumber`.
/// @param gameNumber ����� ������.
/// @return ��������� ����
CardColor StupidPlayer::changeColor()
{
	srand(time(NULL));
	return CardColor(rand() % 4);
}