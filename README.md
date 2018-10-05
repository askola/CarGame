# C++ Programming - The Top Down Racing

---
### To run this game [SFML(2.0+)][] and [Box2D(2.1.2+)][] need to be installed on the computer.
###### It is assumed that SFML and Box2D is installed on the standard library location.
---
Library installations with apt-get/aptitude (requires root): 

```sh
sudo apt-get install libsfml-dev libbox2d-dev
```

This is our implementation of a Micromachines-inspired game for C++ Programming course.

---

The folder structure:

```sh
project
│   README.md
│   .gitignore
│
├───doc     //For the final documentation
│   │
│
├───plan    //Files for project planning
│   │   projectplan.pdf
│   │
│   └───ideas
│       │   useful_links.doc
│
├───CarGame     //Source code
│   │   Makefile
│   │   game.cpp
│   │   game.hpp
│   │   game_state.hpp
│   │   main.cpp
│
└───test    //Test files
    │   test_source.cpp
```
---
#### Team and responsibilities:
###### Miri as Project manager
* Game Structure
* State machine and states
* Menu's
* Game timer
* makefile

###### Tommi(me)
* Car Physics
* SFML-views

###### Henrik
* Tracks and track graphics
* Track obstacles



[SFML(2.0+)]: http://www.sfml-dev.org/
[Box2D(2.1.2+)]: http://box2d.org/

