#include "BattleShip.h"

using namespace std;

short BattleShip::recursionCount = 0;

BattleShip::BattleShip()
{
	map<int, char> tmp;
	int symb = 65;

	for (int x = 0; x < FIELD_SIZE; x++)
	{
		for (int y = 1; y < FIELD_SIZE + 1; y++)
		{
			tmp[y] = ' ';
		}

		field[char(symb)] = tmp;
		enemyField[char(symb)] = tmp;
		tmp.clear();
		symb++;
	}

	ships[1] = 4;///Single-deck = 4 ships
	ships[2] = 3;
	ships[3] = 2;
	ships[4] = 1;

	shipSize = 4;
}

void BattleShip::printField()
{
	cout << "\tMY FIELD" << endl << endl;
	cout << "  ";
	for (int x = 65; x < 65 + field.size(); x++)
		cout << " " << char(x);

	cout << endl;

	for (int y = 1; y < FIELD_SIZE + 1; y++)
	{
		if (y + 1<FIELD_SIZE + 1)
			cout << y << " |";
		else
			cout << y << "|";

		for (auto x = field.begin(); x != field.end(); x++)
		{
			if (field.at((*x).first).at(y) == '0')
				cout << ' ' << " ";
			else
				cout << field.at((*x).first).at(y) << " ";
		}

		cout << endl;
	}

	cout << endl << "\tENEMY FIELD" << endl << endl;
	cout << "  ";
	for (int x = 65; x < 65 + enemyField.size(); x++)
		cout << " " << char(x);

	cout << endl;

	for (int y = 1; y < FIELD_SIZE + 1; y++)
	{
		if (y + 1<FIELD_SIZE + 1)
			cout << y << " |";
		else
			cout << y << "|";

		for (auto x = enemyField.begin(); x != enemyField.end(); x++)
		{
			cout << enemyField.at((*x).first).at(y) << " ";
		}

		cout << endl;
	}
}

void BattleShip::setShips()
{
	bool vertical;
	short pos = rand() % 3 + 1;
	pos == 1 ? vertical = true : vertical = false;

	if (ships.at(shipSize) > 0)
	{
		auto xPos = char(pos);

		if (vertical)
		{
			pos = rand() % 10 + 65;/// X position
			xPos = char(pos);
			pos = rand() % (11 - shipSize) + 1;///Y position
		}
		else
		{
			pos = rand() % (11 - shipSize) + 65;/// X position
			xPos = char(pos);
			pos = rand() % 10 + 1;///Y position
		}

		if (!checkPositionForShip(xPos, pos, shipSize, vertical))
		{
			recursionCount++;
			if (recursionCount == 30)///in very bad case
			{
				clearField();
				recursionCount = 0;
				ships[1] = 4;
				ships[2] = 3;
				ships[3] = 2;
				ships[4] = 1;
				shipSize = 4;
			}
			setShips();
		}
		else
			putShip(xPos, pos, shipSize, vertical);
	}
	else
		if (shipSize - 1 >= 1)
		{
			shipSize--;
			setShips();
		}
}

void BattleShip::checkEnemyCell(BattleShip & obj, const char & _x, const int & _y)
{
	auto result = obj.checkCell(_x, _y);

	if (result == 0)
		enemyField.at(_x).at(_y) = '0';
	else if (result == 1)
		enemyField.at(_x).at(_y) = 'x';
	else if (result == 2)
	{
		//TODO set 0 to all cells around ship 
	}
}

short BattleShip::checkCell(const char & _x, const int & _y)
{
	if (field.at(_x).at(_y) == '@')
	{
		field.at(_x).at(_y) = 'x';
		if (isDestroyed(_x, _y))
			return 2;
		else
			return 1;///wounded
	}

	return 0;
}

void BattleShip::autoCheckEnemyCell(BattleShip & obj)
{
	auto posX = char(rand() % 10 + 65);
	auto posY = rand() % 10 + 1;
	auto result = obj.checkCell(posX, posY);

	if (enemyField.at(posX).at(posY) == '0' || enemyField.at(posX).at(posY) == 'x')
		autoCheckEnemyCell(obj);
	else
	{
		if (result == 0)
			enemyField.at(posX).at(posY) = '0';
		else if (result == 1)
		{
			//TODO set 0 in cells which can't be a part of ship
			enemyField.at(posX).at(posY) = 'x';
		}
		else if (result == 2)
		{
			//TODO set 0 to all cells around
			cout << "Destroyed" << endl;
		}
	}

}

bool BattleShip::checkEndGame()
{
	int countOfDestroyed = 0;

	for (auto x = enemyField.begin(); x != enemyField.end(); x++)
	{
		for (auto y = x->second.begin(); y != x->second.end(); y++)
		{
			if (y->second == 'x')
				countOfDestroyed++;
		}
	}
	if (countOfDestroyed == ALL_SHIP_CELL)
	{
		cout << endl << "YOU WON" << endl;
		return true;
	}

	countOfDestroyed = 0;
	for (auto x = field.begin(); x != field.end(); x++)
	{
		for (auto y = x->second.begin(); y != x->second.end(); y++)
		{
			if (y->second == 'x')
				countOfDestroyed++;
		}
	}
	if (countOfDestroyed == ALL_SHIP_CELL)
	{
		cout << endl << "YOU LOST" << endl;
		return true;
	}

	return false;
}

bool BattleShip::isDestroyed(const char & _x, const int & _y)
{
	for (auto y = _y; y < field.at(_x).size(); y++)
	{
		if (field.at(_x).at(y) == '@')
			return false;
		else if (field.at(_x).at(y) == ' ')
			break;
	}
	for (auto y = _y; y > 0; y--)
	{
		if (field.at(_x).at(y) == '@')
			return false;
		else if (field.at(_x).at(y) == ' ')
			break;
	}
	for (auto x = _x; int(x) < 75; x++)
	{
		if (field.at(x).at(_y) == '@')
			return false;
		else if (field.at(x).at(_y) == ' ')
			break;
	}
	for (auto x = _x; int(x) >= 65; x--)
	{
		if (field.at(x).at(_y) == '@')
			return false;
		else if (field.at(x).at(_y) == ' ')
			break;
	}

	return true;
}

bool BattleShip::checkPositionForShip(char _x, int _y, short shipSize, bool vertical)
{
	if (vertical)
	{
		for (int y = _y; y < _y + shipSize; y++)
		{
			if (field.at(char(_x)).at(y) != ' ')
				return false;
		}
	}
	else
	{
		for (char x = _x; x < _x + shipSize; x++)
		{
			if (field.at(char(x)).at(_y) != ' ')
				return false;
		}
	}
	return true;
}

void BattleShip::putShip(const char & _x, int _y, short shipSize, bool vertical)
{
	auto shipSizeTMP = shipSize;
	int left = 65, right = 74, y = _y;
	if (vertical)
	{
		for (int y = _y; y < _y + shipSize; y++)
			field.at(_x).at(y) = '@';

		if (y > 1)
			y--;
		if (shipSize + 1 < field.at(_x).size())
			shipSize++;
		if (int(_x) - 1 > left)
			left = int(_x) - 1;
		if (int(_x) + 1 < right)
			right = int(_x) + 1;

		for (int x = left; x <= right; x++)
			for (int Y = y; Y< _y + shipSize; Y++)
			{
				if (field.at(char(x)).size()>Y && field.at(char(x)).at(Y) != '@')
					field.at(char(x)).at(Y) = '0';
			}
	}
	else
	{
		int yLast = 10;

		for (char x = _x; x < _x + shipSize; x++)
			field.at(x).at(_y) = '@';
	
		if (y > 1)
			y--;
		if (_y + 1 < field.at(_x).size())
			yLast = _y + 1;
		if (int(_x) - 1 > left)
			left = int(_x) - 1;
		if (int(_x) + shipSize + 1 < right)
			right = int(_x) + shipSize + 1;

		for (int x = left; x <= right; x++)
			for (int Y = y; Y <= yLast; Y++)
			{
				if (field.at(char(x)).at(Y) != '@')
					field.at(char(x)).at(Y) = '0';
			}
	}

	ships.at(shipSizeTMP) = ships.at(shipSizeTMP) - 1;
	recursionCount = 0;
	setShips();
}

void BattleShip::clearField()
{
	for (auto x = field.begin(); x != field.end(); x++)
		for (auto y = x->second.begin(); y != x->second.end(); y++)
		{
			field.at(x->first).at(y->first) = ' ';
		}
}