#pragma once
#include "../game/uno_game.h"
class GodPlayer : public UnoPlayer {
    std::string playerName; // ��� ������
    std::set<const Card*> playerCards; // "����" ������
    std::vector<int> playerWeaknesses; // "��������" ������ �������

    /// @brief ����� ���� � ���� ������� ����� ����� color
    /// @param color - ���� ������� �����
    /// @return ��������� �� ��������� �����, ��� ������� ���������
    const Card* getHigherCard(CardColor color);

    /// @brief ����� ���� ������� � ���� ����� ����� color
    /// @param color - ���� ������� �����
    /// @return ��������� �� ��������� �����, ��� ������� ���������
    const Card* getZeroCard(CardColor color);

    /// @brief ����� ���� ����� � ���� �������� value � �����, 
    /// ������� ����� "��������" ���������� ������
    /// @param value - �������� ������� �����
    /// @return ��������� �� ��������� �����, ��� ������� ���������
    const Card* getSameValueCardWithWeakness(int value);

    /// @brief ����� ���� ������ ���������� ����� �������� value
    /// @param value - �������� ������� �����
    /// @return ��������� �� ��������� �����, ��� ������� ���������
    const Card* getSameValueCard(int value);

    /// @brief ����� ��������� "���������" ���� ��� ����������� ������
    /// @param name - ��� ������.
    /// @return ���� ������ ����� ���, �� ���������� -1,
    ///  ����� ���������� �������� ��������, ��������������� �����
    int getWeakness();
public:
    /// @brief �������� ������
    /// @param name - ��� ������.
    GodPlayer(const char* name);

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

    /// @brief ������� 11. ����� �����������������.
    /// @param playerIndex ����� ������ � ������.
    /// @param handScore ��������� ��������� ��� ����.
    /// @param card �����, ������� �� �������
    virtual void handlePlayerDisqualified(
        int playerIndex,
        int handScore,
        const Card* card);
};