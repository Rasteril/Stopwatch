#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <signal.h>
#include <time.h>


char key;
int run = 0;
int i = 1;

void * key_listener()
{
	while(key = getch())
	{
		if(key == 'p') run = 0;
		else if(key == 'r') i = 0;
	}
}

char * format_time(int i)
{
	int hrs, mins, secs;
	int r1, r2;
	char *buffer = malloc(20);
	hrs = i / 3600;
	r1 = i - hrs * 3600;
	mins = r1 / 60;
	r2 = i - mins * 60;
	secs = r2;
	sprintf(buffer, "%02d:%02d:%02d", hrs, mins, secs); 
	return buffer;


}

int main()
{
	initscr();
	cbreak();
	noecho();
	
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 100000000L; // 100 000 000ns == 0,1s

	pthread_t listener_id;
	
	char initial_input;

	WINDOW *clock_win;
	clock_win = newwin(2, 10, 2, 2);
	box(clock_win, 0, 0);
	
	wprintw(stdscr, "'s' to start; 'p' to stop; 'r' to reset; CTRL+C to exit");

	while(1)
	{
		initial_input = getch();
		if (initial_input == 's')
		{
			run = 1;
			pthread_create(&listener_id, NULL, key_listener, NULL);
		
			while(run)
			{
				wclear(clock_win);
				wprintw(clock_win, "%s", format_time(i));
				wrefresh(clock_win);
				i++;
				nanosleep(&ts, NULL);


			}

			pthread_cancel(listener_id);
		}
	}

	endwin();


	return 0;
}
