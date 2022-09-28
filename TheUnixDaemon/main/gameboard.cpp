#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>

#include "gameboard.hpp"
#include "logger.hpp"
#include "later.hpp"

using namespace std;

Gameboard::Gameboard( int pos1, int pos2, char **argv){
	wnd = nullptr;
	stw = nullptr;
	initscr();

	wnd = loadGameboard( pos1, pos2, argv);
	stw = loadStatus(pos1, pos2);

	cbreak();
	noecho();
	clear();

	refresh();


	keypad(wnd, true);
	curs_set(0);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);		//
	init_pair(2, COLOR_GREEN, COLOR_BLACK);		//player
	init_pair(3, COLOR_GREEN, COLOR_BLACK);	//goal
	init_pair(4, COLOR_BLUE, COLOR_BLACK);		//Box
	init_pair(5, COLOR_GREEN, COLOR_GREEN);		//Timmer
	init_pair(6, COLOR_YELLOW, COLOR_YELLOW);	//Timer
	init_pair(7, COLOR_RED, COLOR_RED);			//Timer
	init_pair(8, COLOR_BLACK, COLOR_BLACK);			//Timer
	wbkgd(wnd, COLOR_PAIR(1));

	calculateTime(argv);

	mvwaddch(stw, 1, 16, ' '|COLOR_PAIR(5));
	mvwaddch(stw, 1, 17, ' '|COLOR_PAIR(6));
	mvwaddch(stw, 1, 18, ' '|COLOR_PAIR(7));

	Later green(time[0], true, &callbackGreen, stw);
	Later yellow(time[1], true, &callbackYellow, stw);
	Later red(time[2], true, &callbackRed, stw);
}

Gameboard::~Gameboard() {
}

WINDOW *Gameboard::getWindowHandle() {
	return wnd;
}

WINDOW *Gameboard::getStatusWindowHandle(){
	return stw;
}

WINDOW *Gameboard::loadGameboard(int pos1, int pos2, char **argv){

	WINDOW *w;

	vector<string> vec;

	// open stream for reading..
	fstream f(argv[1], ios::in);
	if (f.good())
	{
		while (!f.eof())
		{
			string str;
			getline(f, str);
			if (string::npos == str.find(';'))
			{
				vec.push_back(str);
				for(string xachse : vec)
				{
					int xcheck = xachse.length();
					if(xcheck > max.x)
					{
						max.x = xachse.length();
					}
				}
			}
		}
		max.y = vec.size();

		w = newwin(max.y, max.x, pos1, pos2);

		int adx = 0;
		int ady = 0;
		for(string xachse : vec)
		{
			for(char print : xachse)
			{
				if(print == '@'){
					player.y = ady;
					player.x = adx;
					mvwaddch(w, ady, adx, print|COLOR_PAIR(2));

				}
				else if(print == '.'){
					goals.push_back({adx,ady});
					mvwaddch(w, ady, adx, print|COLOR_PAIR(3));

				}
				else if(print == '$'){
					mvwaddch(w, ady, adx, print|COLOR_PAIR(4));

				}
				else{
					mvwaddch(w, ady, adx, print);
				}
				adx++;

			}
			adx = 0;
			ady++;
		}
		f.close();

		return w;
	} else {
		cout << "Map Existiert nicht!" << endl;
		endwin();
	}
}

WINDOW *Gameboard::loadStatus(int pos1, int pos2){
	int x = 20;

	stw = newwin(2, x, max.y+pos1, pos2);

	displayStatus();

	return stw;
}

Point Gameboard::getPlayer(){
	return player;
}
void Gameboard::movePlayer(Point move){
	char ch = mvwinch(wnd, move.y, move.x) & A_CHARTEXT;
	if('#' == ch || '$' == ch){

	}else{
		mvwaddch(wnd, move.y, move.x,'@'|COLOR_PAIR(2));
		mvwaddch(wnd, player.y, player.x, prestau);
		player = move;
		prestau = ' ';
		status.x++;
	}
	if(ch == '.' ){
		prestau = '.';
	}
}
char Gameboard::getItem(Point pt){
	char ch = mvwinch(wnd, pt.y, pt.x) & A_CHARTEXT;
	return ch;
}
void Gameboard::moveItem(Point from, Point to){
	char ch = mvwinch(wnd, to.y, to.x) & A_CHARTEXT;

	if('#' != ch && '$' != ch){
		mvwaddch(wnd, to.y, to.x, '$'|COLOR_PAIR(4));
		mvwaddch(wnd, from.y, from.x, irestau);
		status.y++;
	}

}
void Gameboard::displayGoals(){
	for (Point goal : goals){
		char ch = mvwinch(wnd, goal.y, goal.x) & A_CHARTEXT;
		if('@' != ch && '$' != ch){
			mvwaddch(wnd, goal.y, goal.x, '.'|COLOR_PAIR(3));
		}
	}
}
void Gameboard::displayStatus(){
	mvwprintw(stw, 0, 0, "Moves : %4d", status.x);
	mvwprintw(stw, 1, 0, "Pushes: %4d", status.y);
}
bool Gameboard::areGoalsComplete(){
	int zahler = 0;
	for (Point check : goals){
		char ch = mvwinch(wnd, check.y, check.x) & A_CHARTEXT;
		if(ch == '$'){
			zahler++;
		}
		else{
			return false;
		}
	}
	if (zahler == goals.size()){
		return true;
	}
}

void Gameboard::calculateTime(char **argv){
	int Time = stoi(argv[2]);

	Time /= 100;

	time.push_back(Time*70);
	time.push_back(Time*90);
	time.push_back(Time*100);
	time.push_back(5000);
}

void Gameboard::callbackGreen(WINDOW *stw){
	mvwaddch(stw, 1, 16, ' '|COLOR_PAIR(8));
}

void Gameboard::callbackYellow(WINDOW *stw){
	mvwaddch(stw, 1, 17, ' '|COLOR_PAIR(8));
}

void Gameboard::callbackRed(WINDOW *stw){
	mvwaddch(stw, 1, 18, ' '|COLOR_PAIR(8));
}
