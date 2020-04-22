#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include<vector>

enum GameType
{
	PERSON,
	BOT
};

enum GameStatus
{
	PLAYING,
	WIN,
	DEAD
};

const int kBoardSizeNum = 15;

class GameModel
{
public:

	void startGame(GameType type);
	void calculateScore();
	void actionByPerson(int row, int col);
	void actionByAI(int& clickRow, int& clickCol);
	void updateGameMap(int row, int col);
	bool isWin(int row, int col);
	bool isDeadGame();

	std::vector<std::vector<int>> gameMapVec;
	std::vector<std::vector<int>> scoreMapVec;
	bool playerFlag;
	GameType gameType;
	GameStatus gameStatus;
};


#endif // !GAMEMODEL_H
