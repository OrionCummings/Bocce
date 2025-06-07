# Bocce
Multiplayer Bocce Game

# Goals

* Up to 16 players lmao

# Project Layout

### src
Contains all source (.c) files in the project

### inc
Contains all header (.h) files in the project

### build
Contains all files associated with building the project. This folder is excluded from git tracking.

### lib
Contains all library files associated with the project. 

### res
Contains all resource files (images, models, sounds, etc.)

### db
Contains files directly associated with databases. This does not include and C source or header files that may interact with the database.

# Modules

### Networking

##### Client

##### Server

### Graphics

##### Environment

##### UI

### Physics

* Only need collisions between circles and lines tbh

### Database

* I think it'd be neat to have a record of all games played??? (I've never used databases in a real application)
* Maybe some kind of account system, but not secure AT ALL lmao. Like names associated with stats with no authentication lmao

