#include "linuxRenderer.h"

void Renderer::init_term()
{
	// save old terminal settings
	tcgetattr(0,&oldTerm);
	// get old terminal settings to update
	tcgetattr(0,&term);

	// turn off echo and read by byte instead of line
	term.c_lflag &= ~(ECHO | ICANON);

	// set minimum characters to read to 0
	term.c_cc[VMIN] = 0;

	// set read timeout, 0 for asynchronous
	term.c_cc[VTIME] = 0;

	// apply updated settings
	tcsetattr(0, TCSAFLUSH, &term);

	// clear screen and hide cursor
	write(1, "\x1b[2J\x1b[?25l", 10);
}

void Renderer::restore_term()
{
	// reset terminal attributes
	tcsetattr(0, TCSAFLUSH, &oldTerm);
	// unhide cursor, clear screen
	write(1, "\x1b[?25h\x1b[2J\x1b[H", 13);
}

Renderer::Renderer()
{
	game = SnakeGame({WIDTH, HEIGHT});

	init_term();

	exit = restart = pause = false;
}

Renderer::~Renderer()
{
	restore_term();
}

void Renderer::putChar(const char c, Position pos)
{
	// add to both positions since terminal position indexing starts at 1, and the border offset
	std::string f = "\x1b[" + std::to_string(pos.y+2) + ";" + std::to_string(pos.x+2) + "f" + c;
	write(1, f.c_str(), f.size());
}

void Renderer::writeString(std::string s, Position pos)
{
	std::string f = "\x1b[" + std::to_string(pos.y+2) + ";" + std::to_string(pos.x+2) + "f" + s;
	write(1, f.c_str(), f.size());
}

void Renderer::drawBorder()
{
	std::string s(game.gameSize.x, '-'); 
	std::string f = "\x1b[Ho" + s + "o";
	write(1, f.c_str(), f.size());
	f = "\x1b[" + std::to_string(game.gameSize.y+2) + ";0fo" + s + "o";
	write(1, f.c_str(), f.size());
	for(int i = 2; i <= game.gameSize.y+1; i++)
	{
		f = "\x1b[" + std::to_string(i) + ";1f|\x1b[" + std::to_string(i) + ";" + std::to_string(game.gameSize.x+2) + "f|";  
		write(1, f.c_str(), f.size());
	}

}

void Renderer::drawGame()
{
	// clear screen
	write(1, "\x1b[2J", 4);

	// draw border
	drawBorder();

	// draw snake
	writeString("\u001b[34m", game.snake[0]);
	for(int i = 1; i < game.snake.size(); i++)
	{
		putChar('o', game.snake[i]);
	}
	if(!game.gameOver)
		writeString("0\u001b[37m", game.snake[0]);
	if(game.gameOver)
		writeString("X\u001b[37m", game.snake[0]);

	// draw fruit
	writeString("\u001b[36m*\u001b[37m", game.fruit);
	
	// draw position
	writeString("Score: " + std::to_string(game.snake.size()), {0, game.gameSize.y+1});
	
	// highscore display
	if(game.highscore == -1)
		writeString("Highscore: NONE", {game.gameSize.x+2, 0});
	else
		writeString("Highscore: " + std::to_string(game.highscore), {game.gameSize.x+2, 0});

	// game over
	if(game.gameOver)
	{
		writeString("Game Over!", {game.gameSize.x/2-5, game.gameSize.y/2});
		writeString("Score: " + std::to_string(game.snake.size()), {game.gameSize.x/2-5, game.gameSize.y/2+1});
		writeString("Press q to quit", {game.gameSize.x/2-8, game.gameSize.y/2+2});
		writeString("Press r to restart", {game.gameSize.x/2-8, game.gameSize.y/2+3});
	}
}

void Renderer::getInput()
{
	char c;
	read(0, &c, 1);
	switch(c)
	{
		case 'w':
			if(game.snakeDirection != DOWN)
				game.snakeDirection = UP;
			break;
		case 'a':
			if(game.snakeDirection != RIGHT)
				game.snakeDirection = LEFT;
			break;
		case 's':
			if(game.snakeDirection != UP)
				game.snakeDirection = DOWN;
			break;
		case 'd':
			if(game.snakeDirection != LEFT)
				game.snakeDirection = RIGHT;
			break;
		case 'q':
			exit = true;
			break;
		case 'r':
			restart = true;
			break;
		case 'p':
			pause = !pause;
			break;
	};
}

void Renderer::run()
{
	do
	{
		drawGame();
		getInput();
		if(!pause)
			game.update();
		if(restart)
		{
			game.reset();
			restart = false;
		}
		usleep(100000);
	}
	while(!exit);
}
