#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

double alfa = 0.3;
double beta = 0.2;
double gama = 0.1;
double delta = 0.3;

void draw(char *array, int N){
	char c;
	move(0,0);
	for(int i=0;  i<N; ++i){
		for(int j=0; j<N; ++j){
			c = *array;
			switch(c){
				case 'E': attron(COLOR_PAIR(4)); printw("E|"); break;
				case 'I': attron(COLOR_PAIR(1)); printw("I|"); break;
				case 'R': attron(COLOR_PAIR(3)); printw("R|"); break;
				default : attron(COLOR_PAIR(2)); printw("S|"); break;
			}
			array++;
		}
		printw("\n");
	}
}

void export(FILE *fp, char *array, int N, int infected){
	int sn=0, en=0, in=0, rn=0;
	char c;
	for(int i=0;  i<N; ++i){
		for(int j=0; j<N; ++j){
			c = *array;
			switch(c){
				case 'E': ++en; break;
				case 'I': ++in; break;
				case 'R': ++rn; break;
				default : ++sn; break;
			}
			array++;
		}

	}
	fprintf(fp,"%d %d %d %d %d\n", sn, en, in, rn, infected);
}

void evolve(char *array, int N, int *infected){
	int NN = N*N;
	int index, new_E = 0;
	double norm = 1.0/RAND_MAX;
	for(int i=0; i<NN; i++){
		if(array[i]=='E' || array[i]=='I'){

			index = i-1;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
			index = i+1;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;

			index = i-N-1;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
			++index;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
			++index;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;

			index = i+N-1;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
			++index;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
			++index;
			if(index>=0 && index <NN && i%N != 0 && i%N != (N-1))
				if(array[index]=='S' && rand()*norm<alfa) array[index]='E'; ++new_E;
		}
		if(array[i]=='E' && rand()*norm<beta) array[i] = 'I';
		if(array[i]=='I' && rand()*norm<gama) array[i] = 'R';
	}
	*infected = new_E;
}

int main(int argc, char *argv[]){
	int N = atoi(argv[1]);
	alfa = atof(argv[2]);
	int n = 1000;
	double norm = 1.0/RAND_MAX;
	char state[N*N];
	srand(time(NULL));
	int infected;
	char mode='V'; //Export or Visual

	int NN = N*N;
	while(NN){
		if(rand()*norm < delta) state[NN-1] = 'R';
		else 			state[NN-1] = 'S';
		--NN;
	}
	state[(int)(N*N*0.5+N*0.5)] = 'I';

	if(mode == 'V'){
		initscr();
		start_color();
		use_default_colors();
		init_pair( 1, COLOR_RED, COLOR_BLACK );
		init_pair( 2, COLOR_GREEN, COLOR_BLACK );
		init_pair( 3, COLOR_BLUE, COLOR_BLACK );
		init_pair( 4, COLOR_YELLOW, COLOR_BLACK );
		attron(A_BOLD);
	}
	FILE *fp = fopen("output", "w");
	while(n){
		evolve(state, N, &infected);
		if(mode == 'E') export(fp, state, N, infected);
		else{
			draw(state, N);
			refresh();
			usleep(100000);
//			getch();
		}
		--n;
	}

	fclose(fp);
	endwin();

	return 0;
}
