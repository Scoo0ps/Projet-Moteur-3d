Pour compiler : 
g++ -std=c++17 -O2 main.cpp Sdl.cpp Scene.cpp Geometry.cpp -o moteur3d `pkg-config --cflags --libs sdl2`

Pour lancer : 
./moteur3d
