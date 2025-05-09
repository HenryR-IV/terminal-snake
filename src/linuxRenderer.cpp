#include "linuxRenderer.h"


void Renderer::setPrevGamestate()
{
	prevGamestate.tail = game.snake.back();
	prevGamestate.head = game.snake[0];
	prevGamestate.fruit = game.fruit;
	prevGamestate.score = game.snake.size();
	prevGamestate.gameOver = game.gameOver;
}

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

	// score
	writeString("Score: " + std::to_string(game.snake.size()), {0, game.gameSize.y+2});
	
	// highscore
	writeString("Highscore: " + std::to_string(game.highscore), {game.gameSize.x+3, 0});

	//fruit
	writeString("\u001b[36m*\u001b[37m", {game.fruit.x+1, game.fruit.y+1});
}

void Renderer::updateScoreDisplay()
{
	writeString("\u001b[0K", {7 , game.gameSize.y+2});
	writeString(std::to_string(game.snake.size()), {7 , game.gameSize.y+2});
}

void Renderer::updateHighscoreDisplay()
{
	writeString("\u001b[0K", {game.gameSize.x+14, 0});
	writeString(std::to_string(game.highscore), {game.gameSize.x+14, 0});
}

void Renderer::drawGame()
{
	if(game.gameOver && game.gameOver != prevGamestate.gameOver)
	{
		// replace head with dead
 		writeString("X", {game.snake[0].x+1, game.snake[0].y+1});

		// game over display
		writeString("Game Over!", {game.gameSize.x/2-5, game.gameSize.y/2});
		writeString("Score: " + std::to_string(game.snake.size()), {game.gameSize.x/2-5, game.gameSize.y/2+1});
		writeString("Press q to quit", {game.gameSize.x/2-8, game.gameSize.y/2+2});
		writeString("Press r to restart", {game.gameSize.x/2-8, game.gameSize.y/2+3});

		updateHighscoreDisplay();
	}
	else if(!game.gameOver && game.snake.size() == prevGamestate.score)
	{
		// replace previous head
		writeString("o", {prevGamestate.head.x+1, prevGamestate.head.y+1});
		// remove tail
 		writeString(" ", {prevGamestate.tail.x+1, prevGamestate.tail.y+1});
		// draw new head
 		writeString("0", {game.snake[0].x+1, game.snake[0].y+1});
	}
	else if(!game.gameOver && game.snake.size() != prevGamestate.score)
	{
		// redraw fruit
		writeString("\u001b[36m*\u001b[37m", {game.fruit.x+1, game.fruit.y+1});
		writeString(" ", {prevGamestate.fruit.x+1, prevGamestate.fruit.y+1});

		// replace previous head
		writeString("o", {prevGamestate.head.x+1, prevGamestate.head.y+1});
		// draw new head
 		writeString("0", {game.snake[0].x+1, game.snake[0].y+1});

		updateScoreDisplay();
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

void Renderer::initGame()
{
	game.reset();
	// clear screen
	write(1, "\x1b[2J", 4);

	// includes initial score and highscore
	drawBorder();
	restart = false;
	setPrevGamestate();
}

void Renderer::run()
{
	initGame();
	do
	{
		setPrevGamestate();
		getInput();
		if(!pause)
			game.update();
		if(restart)
			initGame();
		drawGame();
		usleep(100000);
	}
	while(!exit);
}
