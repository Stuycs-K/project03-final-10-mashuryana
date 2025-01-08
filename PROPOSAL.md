# Final Project Proposal

## Group Members:

Angela Mashuryan        
# Intentions:

Create a grid-based game, with each unit/room containing either a trap, treasure, or monster. 
    
# Intended usage:

Players will be displayed a grid and will be able to navigate the grid using keys (w/a/s/d to move in each direction). They will have access to an inventory, and each unit/room will have a limited supply of treasure. Players can join the same "server" of the grid-based game and play with others. There might be an option to save the game progress and resume from where they left off. Players can win by escaping (getting to an end unit/room, defeating a monster, or getting to a certain amount of treasure (luck). Only one player can win.
  
# Technical Details:

Monsters will be handled by child processes. Their behavior will consist of .......

The grid display will be stored in shared memory; locations of traps, treasures, and monsters will be randomized at the beginning only then saved throughout the duration of the game. 

Signals will detect when a treasure is accessed, a player connects/disconnects a "server," a trap is set off.

Sockets (for connection between different machines) will allow players to join the same "server." Max of 2 players in one "server"?

Each unit/room's supply of treasure will be handled by semaphores to allow each player access at a time and with real-time inventory increases/decreases.

    
# Intended pacing:

Get the grid display working, including randomization by 1/11.
Handle sockets and multiplayer functionality by 1/14. 
Handle signals and semaphores by 1/16.
Handle monsters by 1/19.
Video by 1/20.

