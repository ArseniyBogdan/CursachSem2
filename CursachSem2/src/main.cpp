#include "game/uno_game.h"
#include "player/my_player_1.h"
#include "player/my_player_2.h"
#include "player/my_player_3.h"
#include "utils/logger.h"
#include "utils/stats.h"
#include "player/StupidPlayer.h"
#include <vld.h>

void statisticTest()
{
    UnoGame game;
    StupidPlayer player1("Vasya");
    GodPlayer player2("Kolya");
    game.addPlayer(&player1);
    game.addPlayer(&player2);
    StatsObserver result = runGames(game, 1000);
    StatsObserver::MV winMV = result.getWinsMV();
    std::cout << "Kolya won in " << winMV.mean[1] * 100 << "% of games" << std::endl;
}

int main()
{
    // Ваш main должен выглядеть примерно так
    std::cout << _MSC_VER << std::endl;
    UnoGame game;
    Logger logger;
    StupidPlayer player1("Vasya");
    GodPlayer player2("Kolya");
    game.addPlayer(&player1);
    game.addPlayer(&player2);
    game.addObserver(&logger);
    game.runGame();
    statisticTest();
}
