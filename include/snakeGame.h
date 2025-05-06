#pragma once

#include <deque>
#include <time.h>
#include <fstream>
#include <sstream>

#ifdef INSTALL 
	#define HIGHSCORE_PATH std::string(getenv("HOME")) + "/.snake/highscore.txt"
#else
	#define HIGHSCORE_PATH "highscore.txt"
#endif

enum Direction {STOPPED, UP, RIGHT, DOWN, LEFT};

struct Position
{
	int x, y;
};

class SnakeGame 
{
	private:
		void newFruitPos();
		bool fruitPosValid();
		bool checkGameOver();
		void moveSnake();
		void checkSelfCollision();
		void storeHighscore();
		void getHighscore();

	public:
		Position gameSize;
		std::deque<Position> snake;
		Direction snakeDirection;
		Position fruit;
		bool gameOver;
		int highscore;

		void setIfHighscore();

		SnakeGame();
		SnakeGame(Position size);
		~SnakeGame();

		void update();
		void reset();

		std::string to_string();
};
