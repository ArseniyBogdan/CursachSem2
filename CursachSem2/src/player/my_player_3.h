#pragma once
#include "../game/uno_game.h"
class SimplePlayer_3 : public UnoPlayer {
    std::string playerName;
    std::vector<const Card*> playerCards;
    std::vector<int> playerWeakness;

    void setNewName(const char* name);
    int getIndexOfHigherCard(CardColor color);
    int getIndexOfZeroCard(CardColor color);
    int getIndexOfSameValueCard(int value);
    int getIndexOfActionCard(CardColor color);
    int getIndexOfActionCard(CardColor color, int value);
    int getIndexOfWildCard(int value);
    int getIndexOfSameActionCard(int value);
    int WhatColorAreMore();
    int getWeakness();
    void printCards();

public:
    SimplePlayer_3(const char* name);

    /// @brief ����� ���������� ���� ���.
    /// @return ��� ������.
    std::string name() const;

    /// @brief ����� �������� �� ���� �����.
    /// @param cards ������ ����.
    void receiveCards(const std::vector<const Card*>& cards);

    /// @brief ����� ���������� �����, ������� �� ������� (������� � �����).
    /// @return �����, ������� ����� ������� � �����.
    const Card* playCard();

    /// @brief ���� � ������ ��� ���������� ����, �� ����� �������������� 
    /// ����� � �������, ����� �� �� �� ����� �������� ��� ���.
    /// @param additionalCard �������������� �����.
    /// @return true, ���� ����� ����� �� ������ ��� �����, ����� false.
    bool drawAdditionalCard(const Card* additionalCard);

    /// @brief ���� ����� ������� "������ ����" ��� "������ ������", �� 
    /// ���� �������� � ���� ����� ����.
    /// @return ����� ����.
    CardColor changeColor();

    /// @brief ������� 2. ���������� ������ � ������� `gameNumber`.
    /// @param gameNumber ����� ������.
    virtual void handleSetStarted(int gameNumber);

    /// @brief ������� 7. ����� ������� �������������� �����.
    /// @param playerIndex ����� ������ � ������.
    virtual void handlePlayerDrewAnotherCard(int playerIndex);

    /// @brief ������� 8. ����� ������� ����� � ��������� ���.
    /// @param playerIndex ����� ������ � ������.
    /// @param numberOfCards ���������� ����.
    virtual void handlePlayerDrewAndSkip(int playerIndex, int numberOfCards);

    virtual void handlePlayerDisqualified(
        int playerIndex,
        int handScore,
        const Card* card);
};