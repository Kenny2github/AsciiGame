#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

constexpr int WIDTH = 40, HEIGHT = 20;

int x = 0, y = 0;
bool game_over = false;

struct {
	string map[HEIGHT];
	string next_map;
} map;
struct {
	char me = '\0'; // char representing self
	char topleft = '\0'; // border corner
	char topright = '\0';
	char bottomright = '\0';
	char bottomleft = '\0';
	char top = '\0'; // border
	char right = '\0';
	char bottom = '\0';
	char left = '\0';
	char stop = '\0'; // movement stop marker
	char danger = '\0'; // deadly obstacle
	string barriers; // all wall characters
} config;


int load_map(string name) {
	ifstream mapfile(name);
	if (!mapfile.is_open()) return -1;
	for (register int i = 0; i < HEIGHT; i++) {
		getline(mapfile, map.map[i]);
	}
	getline(mapfile, map.next_map);
	mapfile.close();
	return 0;
}
int load_config() {
	ifstream configfile("config.txt");
	if (!configfile.is_open()) return -1;
	char dummy;
	configfile >> noskipws >> config.me >> config.topleft >> config.topright;
	configfile >> noskipws >> config.bottomright >> config.bottomleft;
	configfile >> noskipws >> config.top >> config.right >> config.bottom >> config.left;
	configfile >> noskipws >> config.stop >> config.danger;
	configfile >> noskipws >> dummy;
	configfile >> config.barriers;
	configfile.close();
	return 0;
}

void border(bool bottom = false) {
	cout << (bottom ? config.bottomleft : config.topleft);
	for (register int i = 0; i < WIDTH; i++) {
		cout << (bottom ? config.bottom : config.top);
	}
	cout << (bottom ? config.bottomright : config.topright);
	cout << ('\n');
}

void render() {
	system("cls");
	border();
	for (register int j = 0; j < HEIGHT; j++) {
		cout << (config.left);
		for (register int i = 0; i < WIDTH; i++) {
			if (i == x && j == y) {
				cout << (config.me); // plot me at my pos
			}
			else {
				cout << (map.map[j][i]); // plot map at other pos
			}
		}
		cout << (config.right);
		cout << ('\n');
	}
	border(true);
	cout << "Next map: " << map.next_map << endl;
}

void movement(char key) {
	int xv = 0, yv = 0;
	switch (key) {
	case 'w':
		yv = -1;
		break;
	case 'a':
		xv = -1;
		break;
	case 's':
		yv = +1;
		break;
	case 'd':
		xv = +1;
		break;
	}
	bool oob = false, // out of bounds?
		stop = false, // stop marker
		danger = false, // deadly obstacle
		barrier = false; // wall
	do {
		x += xv;
		y += yv;
	} while (!(
		(oob = (x < 0 ||
			x >= WIDTH ||
			y < 0 ||
			y >= HEIGHT)) ||
		(stop = (map.map[y][x] == config.stop)) ||
		(danger = (map.map[y][x] == config.danger)) ||
		(barrier = (config.barriers.find(map.map[y][x]) != string::npos))
		));
	x -= xv;
	y -= yv;
	if (!oob) {
		if (stop) {
			x += xv;
			y += yv;
		}
		if (danger) {
			game_over = true;
			return;
		}
	}
	if (x == (WIDTH - 1) && y == (HEIGHT - 1)) {
		load_map(map.next_map + ".txt");
		x = y = 0;
	}
}

int main() {
	char k;
	load_config();
	if (load_map("map1.txt") < 0) return 1;
	render();
	while ((k = _getch()) != 'q' && !game_over) {
		movement(k);
		render();
	}
	x = y = -1;
	load_map("game_over.txt");
	render();
	return 0;
}