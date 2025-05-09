#pragma once

#include <termios.h>
#include <unistd.h>
#include "snakeGame.h"

#define WIDTH 40
#define HEIGHT 30

class Renderer
{
	private:
		termios oldTerm, term;
		SnakeGame game;
		void drawGame();
		void writeString(std::string s, Position pos);
		void drawBorder();
		void getInput();
		bool exit, restart, pause;
		void init_term();
		void restore_term();
		void initGame();
		struct 
		{
			Position tail;
			Position head;
			Position fruit;
			int score;
			bool gameOver;
		} prevGamestate;
		void setPrevGamestate();
		void updateScoreDisplay();
		void updateHighscoreDisplay();

	public:
		Renderer();
		~Renderer();
		void run();
};
