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
Clone the repo to some workspace folder. Make sure you have the rust compiler and cargo installed. You will also need: 
- The rust compiler to have wasm32 as a compilation option. run `rustup target add wasm32-unknown-unknown` to be sure it is up to date. 
- trunk, which builds wasm32 for us and can host the server for testing `cargo install trunk`

Then, run:
- `cargo build` to build
- `cargo run` to build and run
- `cargo test` to test the project.
<!-- - `trunk ...` to run a server? -->