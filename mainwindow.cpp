#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <cmath>
#include "Mainwindow.h"

const int kBoardMargin = 30;
const int kRadius = 15;
const int kMarkSize = 6;
const int kBlockSize = 40;
const int kPosDelta = 20;

const int kAIDelay = 700;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setFixedSize(kBoardMargin * 2 + kBlockSize * kBoardSizeNum, kBoardMargin * 2 + kBlockSize * kBoardSizeNum);
	setStyleSheet("background-color:white;");

	setMouseTracking(true);
	QMenu* gameMenu = menuBar()->addMenu(tr("Game"));
	QAction* actionPVP = new QAction("Person VS Person", this);
	connect(actionPVP, SIGNAL(triggered()), this, SLOT(initPVPGame()));
	gameMenu->addAction(actionPVP);

	QAction* actionPVE = new QAction("Person VS Computer", this);
	connect(actionPVE, SIGNAL(triggered()), this, SLOT(initPVEGame()));
	gameMenu->addAction(actionPVE);

	initGame();
}


MainWindow::~MainWindow() {
	if (game)
	{
		delete game;
		game = nullptr;
	}
}

void MainWindow::initGame() {
	game = new GameModel;
	initPVPGame();
}

void MainWindow::initPVPGame() {
	game_type = PERSON;
	game->gameStatus = PLAYING;
	game->startGame(game_type);
	update();
}

void MainWindow::initPVEGame() {
	game_type = BOT;
	game->gameStatus = PLAYING;
	game->startGame(game_type);
	update();
}


void MainWindow::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	pen.setWidth(2);
	painter.setPen(pen);
	for (int i = 0; i <= kBoardSizeNum; i++)
	{
		painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
		painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
	}

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
		clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
		game->gameMapVec[clickPosRow][clickPosCol] == 0) 
	{
		if (game->playerFlag) 
		{
			brush.setColor(Qt::white);
		}
		else
		{
			brush.setColor(Qt::black);
		}
		painter.setBrush(brush);
		painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
	}

	for (int i = 0; i < kBoardSizeNum; i++) 
	{
		for (int j = 0; j < kBoardSizeNum; j++) 
		{
			if (game->gameMapVec[i][j] == 1)
			{
				brush.setColor(Qt::white);
				painter.setBrush(brush);
				painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
			else if (game->gameMapVec[i][j] == -1) {
				brush.setColor(Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
		}
	}
	if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
		clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
		(game->gameMapVec[clickPosRow][clickPosCol] == 1 ||
			game->gameMapVec[clickPosRow][clickPosCol] == -1))
	{
		if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
		{
			qDebug() << "win";
			game->gameStatus = WIN;
//			QSound::play(WIN_SOUND);
			QString str;
			if (game->gameMapVec[clickPosRow][clickPosCol] == 1) 
			{
				str = "white player";
			}
			else if (game->gameMapVec[clickPosRow][clickPosCol] == -1) 
			{
				str = "black player";
			}
			QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + "win!");
		
			if (btnValue == QMessageBox::Ok)
			{
				game->startGame(game_type);
				game->gameStatus = PLAYING;
			}
		}
	}
	if (game->isDeadGame()) {
		QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
		if (btnValue == QMessageBox::Ok)
		{
			game->startGame(game_type);
			game->gameStatus = PLAYING;
		}
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
	int x = event->x();
	int y = event->y();

	if (x >= kBoardMargin + kBlockSize / 2 && 
		x < size().width() - kBoardMargin && 
		y >= kBoardMargin + kBlockSize / 2 &&
		y < size().height() - kBoardMargin) 
	{
		int col = x / kBlockSize;
		int row = y / kBlockSize;
		int leftTopPosX = kBoardMargin + kBlockSize * col;
		int leftTopPosY = kBoardMargin + kBlockSize * row;

		clickPosRow = -1;
		clickPosCol = -1;
		int len = 0;

		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickPosRow = row;
			clickPosCol = col;
		}
		if (len < kPosDelta)
		{
			clickPosRow = row;
			clickPosCol = col;
		}
		len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickPosRow = row;
			clickPosCol = col + 1;
		}
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickPosRow = row + 1;
			clickPosCol = col;
		}
		len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickPosRow = row + 1;
			clickPosCol = col + 1;
		}
	}

	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (!(game_type == BOT && !game->playerFlag))
	{
		chessOneByPerson();
		if (game->gameType == BOT && !game->playerFlag)
		{
			QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
		}
	}
}

void MainWindow::chessOneByPerson() {
	if (clickPosRow != -1 && clickPosCol != -1 && game->gameMapVec[clickPosRow][clickPosCol] == 0)
	{
		game->actionByPerson(clickPosRow, clickPosCol);

		update();
	}
}

void MainWindow::chessOneByAI() {
	game->actionByAI(clickPosRow, clickPosCol);
	update();
}