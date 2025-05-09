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

void Renderer::writeString(std::string s, Position pos)
{
	// add 1 to position since terminal is 1 indexed
	std::string f = "\x1b[" + std::to_string(pos.y+1) + ";" + std::to_string(pos.x+1) + "f" + s;
	write(1, f.c_str(), f.size());
}

void Renderer::drawBorder()
{
	std::string s(game.gameSize.x, '-'); 
	s = 'o' + s + 'o';
	writeString(s, {0,0}); // top
	writeString(s, {0,game.gameSize.y+1}); // bottom

	// sides
	for(int i = 1; i <= game.gameSize.y; i++)
	{
		writeString("|", {0, i});
		writeString("|", {game.gameSize.x+1, i});
	}

}

void Renderer::drawSnake()
{
	// +1 offset for border

	// set color
	writeString("\u001b[34m", {0, 0});
	// draw body
	for(int i = 1; i < game.snake.size(); i++)
	{
		writeString("o", {game.snake[i].x+1, game.snake[i].y+1});
	}

	// draw head and unset color
	if(!game.gameOver)
		writeString("0\u001b[37m", {game.snake[0].x+1, game.snake[0].y+1});
	if(game.gameOver)
		writeString("X\u001b[37m", {game.snake[0].x+1, game.snake[0].y+1});
}

void Renderer::drawGame()
{
	// clear screen
	write(1, "\x1b[2J", 4);

	drawBorder();

	drawSnake();

	// draw fruit, set & unset color
	writeString("\u001b[36m*\u001b[37m", {game.fruit.x+1, game.fruit.y+1});
	
	// score display
	writeString("Score: " + std::to_string(game.snake.size()), {0, game.gameSize.y+2});
	
	// highscore display
	if(game.highscore == -1)
		writeString("Highscore: NONE", {game.gameSize.x+3, 0});
	else
		writeString("Highscore: " + std::to_string(game.highscore), {game.gameSize.x+3, 0});

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
