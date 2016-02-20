mine_sweeper.out : mine_sweeper.o main.o
	gcc -Wall -g -o mine_sweeper.out mine_sweeper.o main.o    

main.o : mine_sweeper.h main.c
	gcc -Wall -g -c -o main.o main.c

mine_sweeper.o : mine_sweeper.h mine_sweeper.c
	gcc -Wall -g -c -o mine_sweeper.o mine_sweeper.c

clean:
	rm *.o *.out