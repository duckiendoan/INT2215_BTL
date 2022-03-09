export LIBRARY_PATH="$LIBRARY_PATH:/opt/homebrew/lib"
g++ *.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -o game.out -lSDL2 -lSDL2_image -lSDL2_ttf
./game.out