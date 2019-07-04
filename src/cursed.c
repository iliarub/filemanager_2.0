

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <ncursesw/curses.h>
#include "handler.h"
#define STR 40
#define STOLB 70

	WINDOW* tabwnd;
	WINDOW* wname;
	WINDOW* wsize;

	void wintab1();
	void wintab2();

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char*) &size);
	resizeterm(size.ws_row, size.ws_col);
}
void getcurrent(int* current, int listsize, int* ch)
{
		if(*ch==KEY_DOWN && *current<listsize-1)
			{ *current=*current+1;}
		if(*ch==KEY_UP && *current!=0) { *current=*current-1;}
}

char *upscreen(struct fileatr* list, int listsize, int* tab)
{
	int current=0;
	int ch=0;
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	int i;
	while(ch!=10)
	{

		for( i=0; i<listsize; i++)
		{
			wmove(wname, i+1, 1);
			wmove(wsize, i+1, 1);
			if(i == current)
			{
				wattron(wname, A_BOLD);
				wattron(wname, COLOR_PAIR(2));
				wattron(wsize, A_BOLD);
				wattron(wsize, COLOR_PAIR(2));
				wprintw(wname,"%s\n", list[i].fname);
				wprintw(wsize,"%d\n", list[i].fsize);
				wattroff(wname, A_BOLD);
				wattroff(wname, COLOR_PAIR(2));
				wattroff(wsize, A_BOLD);
				wattroff(wsize, COLOR_PAIR(2));
			}
			else
			{
				wprintw(wname,"%s\n", list[i].fname);
				wprintw(wsize,"%d\n", list[i].fsize);
			}
		}
		box(tabwnd, 0, 0);
		box(wsize, 0, 0);
		wrefresh(tabwnd);
		wrefresh(wname);
		wrefresh(wsize);

		ch=getch();
		if (ch=='\t'&& *tab==0)
		{
			*tab=1;
			return ".";
		}
		else if (ch=='\t'&& *tab==1)
		{
			*tab=0;
			return".";
		}

		if(ch == 'q')
		{
			free(list);
			exit(1);
		}
		getcurrent(&current, listsize, &ch);
		wmove(wname, 1, 1);
		wclrtobot(wname);
		wmove(wsize, 1, 1);
		wclrtobot(wsize);

	}

	return list[current].fname;
}


void file_manager()
{
	char path1[255];
	char path2[255];
	int tab=0;
	while(1)
	{

		int listsize;
		struct fileatr* list=NULL;
		if(tab==0)
		{
			chdir(path1);
			wintab1();
			list =odir(".", &listsize);
			chdir(upscreen(list, listsize, &tab));
			getcwd(path1, 255);
}
		if(tab==1)
		{
			chdir(path2);
			wintab2();
			list =odir(".", &listsize);
			chdir(upscreen(list, listsize, &tab));
			getcwd(path2, 255);
		}

		free(list);
	}
}


void wintab1()
{
	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	curs_set(0);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	tabwnd =newwin(STR, STOLB, 0, 0);
	wbkgd(tabwnd, COLOR_PAIR(1));
	wname= derwin(tabwnd, STR, STOLB-(2*STOLB/10), 0, 0);
	wsize=derwin(tabwnd, STR, STOLB-(8*STOLB/10), 0, STOLB*8/10);
	wprintw(wsize, "size\n");
	wprintw(wname, "name\n");
	box(tabwnd, 0, 0);
	box(wsize, 0,0);
	wrefresh(tabwnd);
	wrefresh(wname);
	wrefresh(wsize);
	keypad(stdscr, TRUE);
	refresh();

}
void wintab2()
{
	tabwnd =newwin(STR, STOLB, 0, STOLB+1);
	wbkgd(tabwnd, COLOR_PAIR(1));
	wname= derwin(tabwnd, STR, STOLB-(2*STOLB/10), 0, 0);
	wsize=derwin(tabwnd, STR, STOLB-(8*STOLB/10), 0, STOLB*8/10);
	wprintw(wsize, "size\n");
	wprintw(wname, "name\n");
	box(tabwnd, 0, 0);
	box(wsize, 0, 0);
	wrefresh(tabwnd);
	wrefresh(wname);
	wrefresh(wsize);
	refresh();

}

void ex()
{	curs_set(1);
	keypad(stdscr, 0);
	delwin(wname);
	delwin(wsize);
	delwin(tabwnd);
	endwin();
}
int main()
{
atexit(ex);
wintab1();
wintab2();
file_manager();

}
