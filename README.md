# Ars Magica Helper Application

## Overview
This project is meant to create a GUI interface that makes tracking
character information for the tabletop [Ars Magica](https://www.atlas-games.com/arsmagica/) much easier.
The goals are:
- To handle all the calculation of abilities, arts, and various formulae within the game.
- Have a season tracker within which the player can view events, both for character progression and for story beats.
- Easy to use. The user shouldn't be confused about how to do things.
- Extensibility. Should the user desire, they should be able to add their own formulae, edit existing formulae, and configure their view

## How to Contribute
Clone the repo to some workspace folder. Make sure you have the rust compiler and cargo installed. You will also need to have trunk installed (You can use `cargo install trunk` if you do not have it already). Once that is done, simply run `trunk serve` in the `ars-webclient-app` directory. You can view the webclient at the localhost address (127.0.0.1:8000)[127.0.0.1:8000] in any browser.