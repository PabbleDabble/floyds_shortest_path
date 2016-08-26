# You have to have tabs at the beginning of the command lines
all: main

.PHONY main: main.o
	g++ main.o -o main


main.o: main.cpp
	clear
	g++ -c main.cpp 
	
test: main
#	clear;
#	./main 3_graph.txt
#	./main 4_bad_diagonals.txt
	./main 4_base.txt
#	./main 4_neg_cycle.txt
#	./main 8_all_inf.txt
#	./main 8_all_zeroes.txt
#	./main 8_cw_loop.txt
#	./main 8_ccw_loop.txt
#	./main 50_random.txt
#	./main 50_all_pos_random.txt
#	./main 50_random_some_inf.txt	  #I didn't make this yet?

start_end: main
	./main 4_base.txt 0 1
	./main 4_base.txt 1 2
	./main 4_base.txt 2 3
	
	./main 4_base.txt 3 0
	./main 4_base.txt 3 1
	./main 4_base.txt 3 2
	./main 4_base.txt 3 3
	
	./main 4_base.txt 4 5
	./main 4_base.txt 22 3
	./main 4_base.txt 0 10
	./main 4_base.txt 0 -1
	./main 4_base.txt 0 1

clean:
	rm -rf *.o main *~ *\# 

#.PHONY makes main recompile even if no changes are found
