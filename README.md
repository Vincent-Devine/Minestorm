# Project Minestorm

## Presentation
Minestorm project for ISART.\
This project aims at recreating the game Minestorm (launched in 1982 on Vectrex) in C language with Raylib library for the visuals.\
The second goal of this project is to become more familiar with code structuring and math applied to video games.
</br></br>

## Skills
This project was evaluated on :
- Knowing how to determine the collision of 2D primitives.
- Knowing and implementing a collision algorithm.
- Our code had to be optimized and without any memory leaks

</br>

## Infomation about the project
project start : 10/15/2021\
project end : 11/04/2021\
version : GOLD\
date last version : 11/04/2021\
development team :
- DEVINE Vincent
- GOUPIL Kristian
</br>
</br>

## Terminal commands
### Create an executable
```sh
$ make
```
### Execute the executable
```sh
$ ./minestorm
```
### Clean the project
```sh
$ make clean
```
</br>

## Project structure
```sh
├── asset
│   ├── Assets Mine Storm.jpg
│   ├── logo.png
│   ├── minestorm_background.png
│   ├── minestorm_foreground.jpg
│   ├── minestorm_foreground.png
│   └── minestorm_sprite_atlas_mine_storm.png
│
├── Makefile
├── README.md
│
├── src
│   ├── display.c
│   ├── display.h
│   │
│   ├── game.c
│   ├── game.h
│   │
│   ├── gameInit.c
│   ├── gameInit.h
│   │
│   ├── main.c
│   │
│   ├── manager.c
│   ├── manager.h
│   │
│   ├── move.c
│   ├── move.h
│   │
│   ├── my_math.c
│   ├── my_math.h
│   │
│   └── object.h
│
└── third_party
    ├── include
    │   └── raylib.h
    ├── libs-x86_64-linux-gnu
    │   └── libraylib.a
    └── libs-x86_64-pc-cygwin
        └── libraylib.a
```

## Contact
mail : k.goupil@student.isartdigital.com\
mail : v.devine@student.isartdigital.com