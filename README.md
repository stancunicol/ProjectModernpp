# Battle city

## General Description
Battle City is a 2D multiplayer game where players fight in an arena filled with obstacles and walls, with the goal of destroying enemies and being the last one standing on the game board. 

This project implements a client-server application where multiple players can connect and play simultaneously on a randomly generated arena. The application allows users to log in/register, create multiplayer matches, includes various customization options for controls and the possibility to choose the difficulty.
### System Requirements
To run the game, you will need the following:

OS: Windows/Linux/MacOS

Software:
Visual Studio with vcpkg for installing external libraries
SQLite for storing user data
CROW - a C++ library for creating an HTTP server
Library for creating the GUI (QT)

Dependencies:
C++17 or newer
CROW for server implementation

## Features
1. Login and Registration
Users can either create an account or log in to an existing one by providing a unique username.
Authentication is done via a simple login/register page. Email or password are not required to create an account.
2. Multiplayer Matches
The game supports matches with up to 4 players who fight in an arena with different types of walls and obstacles.
Players can be grouped in teams or play individually, and each match follows a set of combat and elimination rules.
There is a scoring system, and each player earns points for eliminating enemies.
3. Game Levels
Difficulty Levels: Players can choose between different difficulty levels (Easy, Medium, Hard), which affect the complexity of the game and the behavior of enemies.
4. Arena Configuration
The game arena is randomly generated and can contain destructible walls and indestructible walls.
The arena may also contain hidden bombs that are triggered when a wall is destroyed, potentially affecting players and surrounding walls.
5. Scoring and Points System
Points are awarded for each enemy eliminated, with each kill giving the player 100 points.
Winning a match grants an additional 500 points.
The score accumulates as players participate in matches and perform well.
6. Multigaming and Matchmaking
The game supports multiple games running simultaneously, and players are grouped into matches of 4 based on their scores.
If a player has been waiting for more than 30 seconds without being matched, the game will start automatically with players of different scores.
7. Graphical User Interface (GUI)
A custom graphical user interface (GUI) is implemented to improve the user experience, using QT.
The GUI includes a 2D game map, player scores, options to select the difficulty level, and the ability to choose the type of game.
8. Exception Handling and Testing
The application includes exception handling to prevent errors or crashes during gameplay.
Unit tests have been implemented to ensure the functionality of the application, with a coverage of at least 40%.

### Installation
Clone the repository:
```
git clone https://github.com/username/battle-city.git
cd battle-city
```
Install dependencies:

Use vcpkg to install the external dependencies (CROW, SQLite ORM).
If using Visual Studio, open the .sln solution file and make sure all packages are correctly installed.

Build and run:

Run the server application and then launch the clients to start a match.
Access the server:

The server will listen on a specific port (e.g., localhost:8080).

