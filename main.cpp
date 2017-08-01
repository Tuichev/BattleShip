#include "BattleShip.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));
	
	BattleShip enemy, my;
	enemy.setShips();
	my.setShips();

	while (!my.checkEndGame())
	{
		my.autoCheckEnemyCell(enemy);
		enemy.autoCheckEnemyCell(my);
		///or my.checkEnemyCell(enemy, 'A', 1); 
		system("cls");
		my.printField();
	}

	std::cin.get();
    return 0;
}

