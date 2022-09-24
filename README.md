# kardeshev
[![CI](https://github.com/taDachs/kardeshev/actions/workflows/ci.yml/badge.svg)](https://github.com/taDachs/kardeshev/actions/workflows/ci.yml)

## What is this supposed to be?
Kardshev is supposed to be a minimal 4X game set in space.
I play way too much Stellaris but I always wanted something that is both
smaller and more in depth in certain areas.

## What is missing?
Everything.

## What is missing for a mvp?
- UI framework
  - I use sdl2 so I need some kind of abstraction layer to stay sane
- sprites
  - I am fine with just drawing bad looking circles, but it would be much nicer to have actual sprites
- parameterized random name generation
  - I need a random name generator that can somehow be parameterized at runtime. One idea was to use a random distribution over a set of tokens and repeatedly sample from that distribution to get names.
- game features that are not yet fully designed
- doc
- unittests
- find a way to make it look like a computer monitor from an old scifi movie (picture the computer screens of the Nostromo in Alien) without touching opengl if possible
- client-server architecture
    - my frontend is going to be terrible and I want to be able to exchange it later
    
# build
```sh
# on ubuntu
sudo apt install cmake libsdl2-dev libsdl2-ttf-dev libboost-dev libglm-dev

mkdir build
cd build
cmake ..
make
```
