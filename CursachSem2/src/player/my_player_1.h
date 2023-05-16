#pragma once
#include "../game/uno_game.h"
class GodPlayer : public UnoPlayer {
    std::string playerName; // имя игрока
    std::set<const Card*> playerCards; // "рука" игрока
    std::vector<int> playerWeaknesses; // "слабости" других игроков

    /// @brief Игрок ищет в руке старшую карту цвета color
    /// @param color - цвет искомой карты
    /// @return указатель на найденную карту, или нулевой указатель
    const Card* getHigherCard(CardColor color);

    /// @brief Игрок ищет нулевую в руке карту цвета color
    /// @param color - цвет искомой карты
    /// @return указатель на найденную карту, или нулевой указатель
    const Card* getZeroCard(CardColor color);

    /// @brief Игрок ищет карту в руке значения value и цвета, 
    /// который будет "неудобен" следующему игроку
    /// @param value - значение искомой карты
    /// @return указатель на найденную карту, или нулевой указатель
    const Card* getSameValueCardWithWeakness(int value);

    /// @brief Игрок ищет первую попавшуюся карту значения value
    /// @param value - значение искомой карты
    /// @return указатель на найденную карту, или нулевой указатель
    const Card* getSameValueCard(int value);

    /// @brief Игрок проверяет "неудобный" цвет для следующуего игрока
    /// @param name - имя игрока.
    /// @return если такого цвета нет, то возвращает -1,
    ///  иначе возвращает числовое значение, соответствующее цвету
    int getWeakness();
public:
    /// @brief Создание игрока
    /// @param name - имя игрока.
    GodPlayer(const char* name);

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

    /// @brief Событие 11. Игрок дисквалифицирован.
    /// @param playerIndex номер игрока в списке.
    /// @param handScore суммарная стоимость его карт.
    /// @param card карта, которую он положил
    virtual void handlePlayerDisqualified(
        int playerIndex,
        int handScore,
        const Card* card);
};