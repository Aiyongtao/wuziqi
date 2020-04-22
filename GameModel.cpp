#include <utility>
#include <cstdlib>
#include <ctime>
#include "GameModel.h"


void GameModel::startGame(GameType type) {
	gameType = type;
	gameMapVec.clear();

	for (int i = 0; i < kBoardSizeNum; i++) {
		std::vector<int> lineBoard;
		for (int j = 0; j < kBoardSizeNum; j++)
		{
			lineBoard.push_back(0);
		}
		gameMapVec.push_back(lineBoard);
	}

	if (gameType == BOT)
	{
		scoreMapVec.clear();
		for (int i = 0; i < kBoardSizeNum; i++)
		{
			std::vector<int> lineScores;
			for (int j = 0; j < kBoardSizeNum; j++)
			{
				lineScores.push_back(0);
			}
			scoreMapVec.push_back(lineScores);
		}
	}

	playerFlag = true;
}

void GameModel::updateGameMap(int row, int col) {
	if (playerFlag)
	{
		gameMapVec[row][col] = 1;
	}
	else
	{
		gameMapVec[row][col] = -1;
	}
	playerFlag = !playerFlag;
}

void GameModel::actionByPerson(int row, int col) {
	updateGameMap(row, col);
}

void GameModel::actionByAI(int& clickRow, int& clickCol) {
	calculateScore();

	int maxScore = 0;

	std::vector<std::pair<int, int>> maxPoint;
	for (int row = 0; row < kBoardSizeNum; row++)
	{
		for (int col = 0; col < kBoardSizeNum; col++)
		{
			if (gameMapVec[row][col] == 0)
			{
				if (scoreMapVec[row][col] > maxScore)
				{
					maxPoint.clear();
					maxScore = scoreMapVec[row][col];
					maxPoint.push_back(std::make_pair(row, col));
				}
				else if (scoreMapVec[row][col] == maxScore)
				{
					maxPoint.push_back(std::make_pair(row, col));
				}
			}
		}
	}

	srand((unsigned)time(0));
	int index = rand() % maxPoint.size();
	
	std::pair<int, int> pointPair = maxPoint.at(index);
	clickRow = pointPair.first;
	clickCol = pointPair.second;
	updateGameMap(clickRow, clickCol);
}

void GameModel::calculateScore() {
	int personNum = 0;
	int botNum = 0;
	int emptyNum = 0;

	scoreMapVec.clear();
	for (int  i = 0; i < kBoardSizeNum; i++)
	{
		std::vector<int> lineScore;
		for (int j = 0; j < kBoardSizeNum; j++)
		{
			lineScore.push_back(0);
		}
		scoreMapVec.push_back(lineScore);
	}
	
	for (int row = 0; row < kBoardSizeNum; row++)
	{
		for (int col = 0; col < kBoardSizeNum; col++)
		{
			if (row > 0 && col > 0 && gameMapVec[row][col] == 0)
			{
				for (int y = -1; y <= 1; y++)
				{
					for (int x = -1; x <= 1; x++)
					{
						personNum = 0;
						botNum = 0;
						emptyNum = 0;

						if (!(y == 0 && x == 0))
						{
							for (int i = 1; i <= 4; i++) {
								if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
									col + i * x > 0 && col + i * x < kBoardSizeNum &&
									gameMapVec[row + i * y][col + i * x] == 1) 
								{
									personNum++;
								}
								else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
									col + i * x >0 && col + i * x < kBoardSizeNum &&
									gameMapVec[row + i * y][col + i * x] == 0) 
								{
									emptyNum++;
									break;
								}
								else
								{
									break;
								}
							}

							for (int i = 1; i <= 4; i++)
							{
								if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
									col - i * x > 0 && col - i * x < kBoardSizeNum &&
									gameMapVec[row - i * y][col - i * x] == 1) 
								{
									personNum++;
								}
								else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
									col - i * x > 0 && col - i * x < kBoardSizeNum &&
									gameMapVec[row - i * y][col - i * x] == 1) 
								{
									emptyNum++;
									break;
								}
								else
								{
									break;
								}
							}

							if (personNum == 1) 
							{
								scoreMapVec[row][col] += 10;
							}
							else if (personNum == 2) {
								if (emptyNum == 1)
								{
									scoreMapVec[row][col] += 30;
								}
								else if (emptyNum == 2) {
									scoreMapVec[row][col] += 40;
								}
							}
							else if (personNum == 3) {
								if (emptyNum == 1)
								{
									scoreMapVec[row][col] += 60;
								}
								else if (emptyNum == 2) {
									scoreMapVec[row][col] += 110;
								}
							}
							else if (personNum == 4) {
								scoreMapVec[row][col] += 20000;
							}

							emptyNum = 0;

							for (int i = 1; i <= 4; i++)
							{
								if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
									col + i * x > 0 && col + i * x < kBoardSizeNum &&
									gameMapVec[row + i * y][col + i * x] == -1) 
								{
									botNum++;
								}
								else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
									col + i * x > 0 && col + i * x < kBoardSizeNum &&
									gameMapVec[row + i * y][col + i * x] == 0) 
								{
									emptyNum++;
									break;
								}
								else
								{
									break;
								}
							}

							for (int i = 1; i <= 4; i++)
							{
								if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
									col - i * x > 0 && col - i * x < kBoardSizeNum &&
									gameMapVec[row - i * y][col - i * x] == -1) 
								{
									botNum++;
								}
								else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
									col - i * x > 0 && col - i * x < kBoardSizeNum &&
									gameMapVec[row - i * y][col - i * x] == 0) 
								{
									emptyNum++;
									break;
								}
								else
								{
									break;
								}
							}
						}
						if (botNum == 0)
						{
							scoreMapVec[row][col] += 15;
						}
						else if (botNum == 1) {
							scoreMapVec[row][col] += 20;
						}
						else if (botNum == 2) {
							if (emptyNum == 1) {
								scoreMapVec[row][col] += 40;
							}
							else if (emptyNum == 2) {
								scoreMapVec[row][col] += 80;
							}
						}
						else if (botNum == 3) {
							if (emptyNum == 1)
							{
								scoreMapVec[row][col] += 90;
							}
							else if (emptyNum == 2) {
								scoreMapVec[row][col] += 150;
							}
						}
						else if (botNum >= 4) {
							scoreMapVec[row][col] += 9999999;
						}
					}
				}
			}
		}
	}
}

bool GameModel::isWin(int row, int col) {
	for (int i = 0; i < 5; i++) {
		if (col - i > 0 && col - i + 4 < kBoardSizeNum &&
			gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
			gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
			gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
			gameMapVec[row][col - i] == gameMapVec[row][col - i + 4]) 
		{
			return true;
		}
	}
	
	for (int i = 0; i < 5; i++)
	{
		if (row - i > 0 && row - i + 4 < kBoardSizeNum &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 4][col]) 
		{
			return true;
		}
	}
	
	for (int i = 0; i < 5; i++)
	{
		if (row + i < kBoardSizeNum && row + i - 4 > 0 &&
			col - i > 0 && col - i + 4 < kBoardSizeNum &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4]) 
		{
			return true;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (row - i > 0 &&
			row - i + 4 < kBoardSizeNum &&
			col - i > 0 &&
			col - i + 4 < kBoardSizeNum &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}

bool GameModel::isDeadGame() {
	// 所有空格全部填满
	for (int i = 1; i < kBoardSizeNum; i++)
		for (int j = 1; j < kBoardSizeNum; j++)
		{
			if (!(gameMapVec[i][j] == 1 || gameMapVec[i][j] == -1))
				return false;
		}
	return true;
}
