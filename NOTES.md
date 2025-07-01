
## Icons:
16x16 pixel PFPs
* a-z (default according to display name)
* Goat
* Ghost
* Mushroom
* Star
* Bird

## Acheivements:
* Limit Tests:
    * Chat: exceed the character limit
    * Court: get outside the bounds for more than 1 second




## Code:

* The UI layout must be calculated ONCE (or on window resize!)
* UI layout data can then be passed to another part of init() to initialize the physics objects (court surface, walls, balls, etc...) ONCE!
* UI layout data can then be passed to draw() 