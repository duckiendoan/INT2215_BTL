cmake_minimum_required(VERSION 3.20)
project(LearnSDL2)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/SDL2/include)
link_directories(${PROJECT_SOURCE_DIR}/SDL2/lib)
add_executable(LearnSDL2 main.cpp GameObject.cpp RenderWindow.cpp Game.cpp Ball.cpp Button.cpp Sprite.cpp BrickManager.cpp Animator.cpp SoundPlayer.cpp SoundPlayer.h ToggleButton.cpp ToggleButton.h Game.Button.cpp)
target_link_libraries(LearnSDL2 mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})