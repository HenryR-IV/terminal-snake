# terminal-snake
Snake - the game, written in C++ for Linux only using glibc and the C++ stl
![snake demo](demo/snake.mp4)

### Installation
1. First Clone or download this repository

1. Compile
```
make install
```
Installs by default to ~/.local/bin and ~/.snake/highscore.txt.
This can be changed in the makefile, or be done manually by running:
```
make
```
This will only generate the executable in the ./bin directory. Highscore file will be generate in whatever directory you run it from.
```
make uninstall
```
This will remove the executable from .local/bin and delete .snake directory.
