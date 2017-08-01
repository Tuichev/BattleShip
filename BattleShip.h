#pragma once
#include <iostream>
#include <windows.h>
#include <map>
#include <time.h>

#define FIELD_SIZE 10
#define ALL_SHIP_CELL 20

class BattleShip
{
public:
	BattleShip();
	void printField();	
	void setShips();
	void checkEnemyCell(BattleShip &obj, const char &_x, const int &_y);
	void autoCheckEnemyCell(BattleShip &obj);
	short checkCell(const char &_x, const int &_y);
	bool checkEndGame();
private:
	void putShip(const char &_x, int _y, short shipSize, bool vertical);
	void clearField();
	bool isDestroyed(const char &_x, const int &_y);
	bool checkPositionForShip(char _x, int _y, short shipSize, bool vertical);
	short shipSize;
	static short recursionCount;
	std::map<char, std::map<int, char>> field;
	std::map<char, std::map<int, char>> enemyField;
	std::map<short, short> ships;
};