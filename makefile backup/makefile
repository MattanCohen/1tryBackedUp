all: clean compile link


link:
	g++ -o bin/studio bin/main.o bin/Action.o bin/Customer.o bin/Studio.o bin/Trainer.o bin/Workout.o

compile:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Studio.o src/Studio.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Trainer.o src/Trainer.cpp
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Workout.o src/Workout.cpp


clean:
	rm -f bin/*

