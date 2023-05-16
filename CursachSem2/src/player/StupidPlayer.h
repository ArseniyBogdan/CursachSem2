#pragma once
#include "../game/uno_game.h"
#include <time.h>
using std::string;
using std::vector;

class StupidPlayer: public UnoPlayer{
private:
    string Player_name;
    vector<const Card*> cards;

    /// @brief Игрок проверяет, может ли он сыграть дикую карту "возьми 4"
    /// @param карта, которая лежит на верхушке сброса
    /// @return true, если да, false, усли нет
    bool CouldPlayWD4(const Card* card);
public:
    /// @brief Создание игрока
    /// @param name - имя игрока.
    StupidPlayer(string const name);

    /// @brief Игрок возвращает свое имя.
    /// @return имя игрока.
    virtual string name() const;

    /// @brief Игрок получает на руки карты.
    /// @param cards массив карт.
    virtual void receiveCards(const vector<const Card*>& cards);

    /// @brief Игрок возвращает карту, которую он сыграет (положит в сброс).
    /// @return карта, которую игрок положит в сброс.
    virtual const Card* playCard();

    /// @brief Если у игрока нет подходящих карт, он тянет дополнительную 
    /// карту и говорит, хочет ли он ее сразу положить или нет.
    /// @param additionalCard дополнительная карта.
    /// @return true, если игрок сразу же кладет эту карту, иначе false.
    virtual bool drawAdditionalCard(const Card* additionalCard);

    /// @brief Если игрок положил "Закажи цвет" или "Возьми четыре", то 
    /// игра запросит у него новый цвет.
    /// @return новый цвет.
    virtual CardColor changeColor();

    /// @brief Событие 2. Начинается партия с номером `gameNumber`.
    /// @param gameNumber номер партии.
    virtual void handleSetStarted(int gameNumber);
};
