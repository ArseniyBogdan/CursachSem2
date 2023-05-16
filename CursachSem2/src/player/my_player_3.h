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

    /// @brief Игрок возвращает свое имя.
    /// @return имя игрока.
    std::string name() const;

    /// @brief Игрок получает на руки карты.
    /// @param cards массив карт.
    void receiveCards(const std::vector<const Card*>& cards);

    /// @brief Игрок возвращает карту, которую он сыграет (положит в сброс).
    /// @return карта, которую игрок положит в сброс.
    const Card* playCard();

    /// @brief Если у игрока нет подходящих карт, он тянет дополнительную 
    /// карту и говорит, хочет ли он ее сразу положить или нет.
    /// @param additionalCard дополнительная карта.
    /// @return true, если игрок сразу же кладет эту карту, иначе false.
    bool drawAdditionalCard(const Card* additionalCard);

    /// @brief Если игрок положил "Закажи цвет" или "Возьми четыре", то 
    /// игра запросит у него новый цвет.
    /// @return новый цвет.
    CardColor changeColor();

    /// @brief Событие 2. Начинается партия с номером `gameNumber`.
    /// @param gameNumber номер партии.
    virtual void handleSetStarted(int gameNumber);

    /// @brief Событие 7. Игрок получил дополнительную карту.
    /// @param playerIndex номер игрока в списке.
    virtual void handlePlayerDrewAnotherCard(int playerIndex);

    /// @brief Событие 8. Игрок получил карты и пропустил ход.
    /// @param playerIndex номер игрока в списке.
    /// @param numberOfCards количество карт.
    virtual void handlePlayerDrewAndSkip(int playerIndex, int numberOfCards);

    virtual void handlePlayerDisqualified(
        int playerIndex,
        int handScore,
        const Card* card);
};