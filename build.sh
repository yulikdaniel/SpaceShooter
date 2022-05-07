g++ -c model_creator.cpp -fsanitize=address,undefined -g
g++ model_creator.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fsanitize=address,undefined -g