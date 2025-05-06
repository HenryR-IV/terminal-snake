#include "snakeGame.h"

SnakeGame::SnakeGame()
{
	gameSize.x = gameSize.y = 30;
	snake.push_front({gameSize.x/2, gameSize.y/2}); // initial head position
	snakeDirection = STOPPED;
	srand(time(NULL));
	newFruitPos();
	gameOver = false;
	getHighscore();
}

SnakeGame::SnakeGame(Position size)
{
	gameSize = size;
	snake.push_front({gameSize.x/2, gameSize.y/2}); // initial head position
	snakeDirection = STOPPED;
	srand(time(NULL));
	newFruitPos();
	gameOver = false;
	getHighscore();
}

SnakeGame::~SnakeGame()
{
	storeHighscore();
}

std::string SnakeGame::to_string()
{
	std::stringstream ss;
	ss << "Game Size: " << gameSize.x << " x " << gameSize.y << '\n';
	ss << "Snake Positions: ";
	for(int i = 0; i < snake.size(); i++)
		ss << "(" << snake[i].x << ", " << snake[i].y << "), ";
	ss << '\n';
	ss << "Snake Size: " << snake.size() << '\n';
	ss << "Snake Direction: ";
	switch(snakeDirection)
	{
		case STOPPED: 
			ss << "STOPPED" << '\n';
			break;
		case UP: 
			ss << "UP" << '\n';
			break;
		case RIGHT: 
			ss << "RIGHT" << '\n';
			break;
		case DOWN: 
			ss << "DOWN" << '\n';
			break;
		case LEFT: 
			ss << "LEFT" << '\n';
			break;
	};
	ss << "Fruit Position: (" << fruit.x << ", " << fruit.y << ")" << '\n';

	return ss.str();
}

bool SnakeGame::fruitPosValid()
{
	for(int i = 0; i < snake.size(); i++)
	{
		if(snake[i].x == fruit.x && snake[i].y == fruit.y)
			return false;
	}
	return true;
}

void SnakeGame::newFruitPos()
{
	do
	{
		fruit.x = rand()%gameSize.x;
		fruit.y = rand()%gameSize.y;
	}
	while(!fruitPosValid());
}

bool SnakeGame::checkGameOver()
{
	if(gameOver)
		return true;

	switch(snakeDirection)
	{
		case STOPPED:
			break;
		case UP:
			if(snake.front().y == 0)
			{
				setIfHighscore();
				return true;
			}
			break;
		case RIGHT:
			if(snake.front().x == gameSize.x-1)
			{
				setIfHighscore();
				return true;
			}
			break;
		case DOWN:
			if(snake.front().y == gameSize.y-1)
			{
				setIfHighscore();
				return true;
			}
			break;
		case LEFT:
			if(snake.front().x == 0)
			{
				setIfHighscore();
				return true;
			}
			break;
	};

	return false;
}

void SnakeGame::moveSnake()
{
	switch(snakeDirection)
	{
		case STOPPED:
			break;
		case UP:
			snake.push_front({snake.front().x, snake.front().y-1});
			break;
		case RIGHT:
			snake.push_front({snake.front().x+1, snake.front().y});
			break;
		case DOWN:
			snake.push_front({snake.front().x, snake.front().y+1});
			break;
		case LEFT:
			snake.push_front({snake.front().x-1, snake.front().y});
			break;
	};
}

void SnakeGame::checkSelfCollision()
{
	for(int i = 1; i < snake.size(); i++)
	{
		if(snake.front().x == snake[i].x && snake.front().y == snake[i].y)
		{
			gameOver = true;
			setIfHighscore();
			return;
		}
	}
}

void SnakeGame::update()
{
	// check if direction will cause game over
	gameOver = checkGameOver();
	if(gameOver || snakeDirection == STOPPED)
	{
		snakeDirection = STOPPED;
		return;
	}
	moveSnake();
	// if new head is on not on fruit, remove tail
	if(fruitPosValid())
		snake.pop_back();
	else
		newFruitPos();
	checkSelfCollision();
}

void SnakeGame::reset()
{
	snake.clear();
	snake.push_front({gameSize.x/2, gameSize.y/2}); // initial head position
	snakeDirection = STOPPED;
	newFruitPos();
	gameOver = false;
}

void SnakeGame::setIfHighscore()
{
	if(snake.size() > highscore)
	{
		highscore = snake.size();
	}
}

void SnakeGame::storeHighscore()
{
	std::ofstream file(HIGHSCORE_PATH);
	if(!file)
	{
		file.close();
		return;
	}
	file << highscore;
	file.close();
}

void SnakeGame::getHighscore()
{
	highscore = 0;
	std::ifstream infile(HIGHSCORE_PATH);
	if(!infile)
	{
		infile.close();
		return;
	}
	infile >> highscore;
	infile.close();
}
