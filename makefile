all: clean link

link: studio action customer trainer workout main
	g++ -o bin/studio bin/main.o bin/Action.o bin/Customer.o bin/Studio.o bin/Trainer.o bin/Workout.o

studio:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Studio.o src/Studio.cpp

action:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Action.o src/Action.cpp

customer:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Customer.o src/Customer.cpp

trainer:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Trainer.o src/Trainer.cpp

workout:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/Workout.o src/Workout.cpp

main:
	g++ -g -Wall -Weffc++ -c -Iinclude -o bin/main.o src/main.cpp


clean:
	rm -f bin/*

