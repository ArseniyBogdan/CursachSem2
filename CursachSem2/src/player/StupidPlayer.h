#pragma once
#include "../game/uno_game.h"
#include <time.h>
using std::string;
using std::vector;

class StupidPlayer: public UnoPlayer{
private:
    string Player_name;
    vector<const Card*> cards;

    /// @brief ����� ���������, ����� �� �� ������� ����� ����� "������ 4"
    /// @param �����, ������� ����� �� �������� ������
    /// @return true, ���� ��, false, ���� ���
    bool CouldPlayWD4(const Card* card);
public:
    /// @brief �������� ������
    /// @param name - ��� ������.
    StupidPlayer(string const name);

    /// @brief ����� ���������� ���� ���.
    /// @return ��� ������.
    virtual string name() const;

    /// @brief ����� �������� �� ���� �����.
    /// @param cards ������ ����.
    virtual void receiveCards(const vector<const Card*>& cards);

    /// @brief ����� ���������� �����, ������� �� ������� (������� � �����).
    /// @return �����, ������� ����� ������� � �����.
    virtual const Card* playCard();

    /// @brief ���� � ������ ��� ���������� ����, �� ����� �������������� 
    /// ����� � �������, ����� �� �� �� ����� �������� ��� ���.
    /// @param additionalCard �������������� �����.
    /// @return true, ���� ����� ����� �� ������ ��� �����, ����� false.
    virtual bool drawAdditionalCard(const Card* additionalCard);

    /// @brief ���� ����� ������� "������ ����" ��� "������ ������", �� 
    /// ���� �������� � ���� ����� ����.
    /// @return ����� ����.
    virtual CardColor changeColor();

    /// @brief ������� 2. ���������� ������ � ������� `gameNumber`.
    /// @param gameNumber ����� ������.
    virtual void handleSetStarted(int gameNumber);
};
