[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# PROJECT NAME HERE

### GROUP NAME HERE

Angela Mashuryan 

### Project Description:

This is a simple, networked, grid-based game. Players can navigate a grid, collect items, fight monsters, and try to escape or collect all treasure.
  
### Instructions:

## Features

*   **Networked Gameplay:** Supports up to two players connected to a server.
*   **Grid-Based Movement:** Players use `w`, `a`, `s`, `d` keys to move north, west, south, and east.
*   **Room Exploration:** The grid consists of rooms containing traps (`T`), treasures (`$`), monsters (`M`), potions (`P`), and an escape point (`E`).
*   **Inventory System:** Players can collect items (like potions and rocks) and view their inventory using the `i` command.
*   **Item Collection:** Players can pick up items in their current room using the `g` command.
*   **Potion Usage:** Players can use potions to avoid death from traps or monster attacks using the `u` command.
*   **Monster Combat:** Players encounter monsters and engage in simple combat.
*   **Win Conditions:** Players can win by escaping at the 'E' location or by collecting all treasures. Death by monster or trap results in a loss.
*   **Game Saving/Loading:** The game state is saved to `game_save.dat` so progress can be resumed.
*   **Monster Movement:** Monsters move randomly after each player turn.

## Files

*   **`server.c`:** The server program that manages the game state, handles client connections, and updates the game world.
*   **`client.c`:** The client program that allows players to connect to the server and interact with the game.
*   **`game.c`:** Contains functions for grid initialization, display, saving, and loading.
*   **`grid.h`:** Header file defining the game data structures and function prototypes.
*   **`Makefile`:** A build script for compiling the game.

## Compilation

To compile the game, run:

make server

make client

./server

./client (x2 for two players)

You can run two clients on the same machine or on different machines if they are on the same network. If running on different machines, you might need to change the IP address in client.c from "127.0.0.1" (localhost) to the server's IP address.

## Presentation

https://drive.google.com/file/d/1qeVsl7J9E5Qx6ZA0AQ83PTiGqDfwLgQr/view



