build:
	g++ -g -std=c++17 ./src/*.cpp -o Solitaire_test -lsfml-graphics -lsfml-window -lsfml-system

run:
	./Build/Solitaire_test

clean:
	rm ./Build/Solitaire_test
