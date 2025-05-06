# terminal-snake
Snake - the game, written in C++ for Linux only using glibc and the C++ stl

### Installation
1. First Clone or download this repository

1. Compile
```
make install
```
Installs by default to ~/.local/bin and ~/.snake/highscore.txt.
This can be changed in the makefile, or be done manually by running:
```
make uninstall
```
which will remove the executable from .local/bin and delete .snake directory 

If you don't wan't to install it you can just run
```
make
```
This will only generate the executable in the local ./bin directory. Highscore file will be generate in whatever directory you run it from.
