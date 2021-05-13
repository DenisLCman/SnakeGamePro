#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio/Music.hpp>
#include <iostream>
#include <vector>
#include <windows.h>
#include <math.h>
using namespace sf;
using namespace std;

const int W = 800;
const int H = 800;
const float rad = 25.0f;
int SPEED_GAME = 250;

struct ChangePoint {
	int x;
	int y;
	int direction;
	bool pressed = false;
	ChangePoint(int _x, int _y, int _direction) : x(_x), y(_y), direction(_direction){}
};


struct Apple {
	int x;
	int y;
	CircleShape shape;
	Apple() {
		shape.setRadius(rad);
		int rad_int = (int)rad * 2;
		cout << rad_int << endl;
		x = rand() % (W / rad_int) * rad_int;
		y = rand() % (H / rad_int) * rad_int;
	}
};


struct Cell {
	int x;
	int y;
	int direction;
	int speed = rad*2;
	CircleShape shape;
	vector <ChangePoint> PointList;
	Cell(int _x, int _y, int _direction) {
		this->x =_x;
		this->y = _y;
		this->direction = _direction;
		shape.setRadius(rad);
	}
	void move() {
		if (this->direction == 1) {
			this->x += speed;
		}
		if (direction == 2) {
			this->y -= speed;
		}
		if (direction == 3) {
			this->x -= speed;
		}
		if (direction == 4) {
			this->y += speed;
		}
	}
};

void keyPress(RenderWindow &window, Event &event, int &direct, vector<Cell> &SnakeList, bool &game_run, bool &lock_run) {
	if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
		window.close();
	}
	if (event.key.code == Keyboard::Right) {
		if (((direct != 3) and (SnakeList.size() == 1)) or ((direct != 3))) {
			if (lock_run == false) {
				direct = 1;
				lock_run = true;
			}
			for (int j = 0;j < SnakeList.size();j++) {
				SnakeList[j].PointList.push_back(ChangePoint(SnakeList[0].x, SnakeList[0].y, 1));
			}

		}
	}
	if (event.key.code == Keyboard::Up) {
		if (((direct != 4) and (SnakeList.size() == 1)) or ((direct != 4))) {
			if (lock_run == false) {
				direct = 2;
				lock_run = true;
			}
			for (int j = 0;j < SnakeList.size();j++) {
				SnakeList[j].PointList.push_back(ChangePoint(SnakeList[0].x, SnakeList[0].y, 2));
			}
		}
	}
	if (event.key.code == Keyboard::Left) {
		if (((direct != 1) and (SnakeList.size() == 1)) or ((direct != 1))) {
			if (lock_run == false) {
				direct = 3;
				lock_run = true;
			}
			for (int j = 0;j < SnakeList.size();j++) {
				SnakeList[j].PointList.push_back(ChangePoint(SnakeList[0].x, SnakeList[0].y, 3));
			}
		}
	}
	if (event.key.code == Keyboard::Down) {
		if (((direct != 2) and (SnakeList.size() == 1)) or ((direct != 2))) {
			if (lock_run == false) {
				direct = 4;
				lock_run = true;
			}
			for (int j = 0;j < SnakeList.size();j++) {
				SnakeList[j].PointList.push_back(ChangePoint(SnakeList[0].x, SnakeList[0].y, 4));
			}
		}
	}
	if (event.key.code == Keyboard::Space) {
		direct = 0;
		for (int j = 0;j < SnakeList.size();j++) {
			SnakeList[j].PointList.push_back(ChangePoint(SnakeList[0].x, SnakeList[0].y, 0));
		}
	}
	if (event.key.code == Keyboard::R) {
		SnakeList.clear();
		for (int i = 0;i < 3; i++) {
			SnakeList.push_back(Cell(300 - rad * 2 * i, 300, 1));
		}
		SnakeList.front().shape.setFillColor(Color::Blue);
		direct = 1;
		game_run = true;
	}
	if (event.key.code == Keyboard::W) {
		SPEED_GAME -= 25;
	}
	if (event.key.code == Keyboard::S) {
		SPEED_GAME += 25;
	}
	lock_run = false;
}



void move_all(vector <Cell> &SnakeList) {
	for (int i = 1;i < SnakeList.size();i++) {
		SnakeList[i].move();
		if (SnakeList[i].x >= W) {
			SnakeList[i].x = 0;
			continue;
		}
		if (SnakeList[i].x < 0) {
			SnakeList[i].x = W-rad*2;
			continue;
		}
		if (SnakeList[i].y >= H) {
			SnakeList[i].y = 0;
			continue;
		}
		if (SnakeList[i].y  < 0) {
			SnakeList[i].y = H - rad*2;
			continue;
		}
	}
}

void move_head(vector <Cell> &SnakeList) {
	SnakeList.front().move();
	if (SnakeList.front().x >= W) {
		SnakeList.front().x = 0;
		return;
	}
	if (SnakeList.front().x < 0) {
		SnakeList.front().x = W - rad * 2;
		return;
	}
	if (SnakeList.front().y >= H) {
		SnakeList.front().y = 0;
		return;
	}
	if (SnakeList.front().y < 0) {
		SnakeList.front().y = H - rad * 2;
		return;
	}
}


void drawning(CircleShape &shape, int rad, Color col, RenderWindow &window, int x, int y) {
	shape.setFillColor(col);
	shape.setPosition(x, y);
	window.draw(shape);
}


void drawning_all(RenderWindow &window, vector <Cell> &SnakeList, Apple &point_ball) {
	for (int i = 1;i < SnakeList.size();i++) {
		drawning(SnakeList[i].shape, rad, Color::Green, window, SnakeList[i].x, SnakeList[i].y);
	}
	drawning(point_ball.shape, rad, Color::Red, window, point_ball.x, point_ball.y);
}


void change_direct_cell(vector <Cell> &SnakeList) {
	for (int i = 1;i < SnakeList.size();i++) {
		for (int j = 0;j < SnakeList[i].PointList.size();j++) {
			if (SnakeList[i].PointList[j].pressed == false) {
				if (abs(sqrt(pow((SnakeList[i].x - SnakeList[i].PointList[j].x), 2) + pow((SnakeList[i].y - SnakeList[i].PointList[j].y), 2))) < rad+5) {
					SnakeList[i].direction = SnakeList[i].PointList[j].direction;
					SnakeList[i].PointList[j].pressed = true;
				}
			}
			else {
				if (SnakeList[i].PointList.size() > 30) {
					SnakeList[i].PointList.erase(SnakeList[i].PointList.cbegin());
				}
			}
		}
	}
}


void contack_with_apple(vector <Cell> &SnakeList, Apple &point_ball, vector <ChangePoint> tmp_list) {
	if (abs(sqrt(pow((SnakeList.front().x - point_ball.x), 2) + pow((SnakeList.front().y - point_ball.y), 2))) < rad+5) {
		int rad_int = (int)rad * 2;
		point_ball.x = rand() % (W / rad_int) * rad_int;
		point_ball.y = rand() % (H / rad_int) * rad_int;
		bool flagoverread = false;
		do{
			for (int i = 0;i < SnakeList.size();i++) {
				if (point_ball.x == SnakeList[i].x and point_ball.y == SnakeList[i].y) {
					flagoverread = false;
					point_ball.x = rand() % (W / rad_int) * rad_int;
					point_ball.y = rand() % (H / rad_int) * rad_int;
					break;
				}
				else {
					flagoverread = true;
				}
			}
		} while (flagoverread == false);
		
		
		if (SnakeList.back().direction == 1) {
			Cell tmp(SnakeList.back().x - rad * 2, SnakeList.back().y, SnakeList.back().direction);
			tmp.PointList = tmp_list;
			SnakeList.push_back(tmp);
		}
		if (SnakeList.back().direction == 2) {
			Cell tmp(SnakeList.back().x, SnakeList.back().y + rad * 2, SnakeList.back().direction);
			tmp.PointList = tmp_list;
			SnakeList.push_back(tmp);
		}
		if (SnakeList.back().direction == 3) {
			Cell tmp(SnakeList.back().x + rad * 2, SnakeList.back().y, SnakeList.back().direction);
			tmp.PointList = tmp_list;
			SnakeList.push_back(tmp);
		}
		if (SnakeList.back().direction == 4) {
			Cell tmp(SnakeList.back().x, SnakeList.back().y - rad * 2, SnakeList.back().direction);
			tmp.PointList = tmp_list;
			SnakeList.push_back(tmp);
		}
	}
}

bool crash_wall(vector <Cell> &SnakeList) {
	if (SnakeList.front().x > W-rad or SnakeList.front().x < 0 or SnakeList.front().y > H-rad or SnakeList.front().y < 0) {
		SnakeList.clear();
		return false;
	}
	return true;
}

bool crash_cell(vector <Cell> &SnakeList) {
	for (int i = 1;i < SnakeList.size();i++) {
		if (SnakeList.front().x == SnakeList[i].x and SnakeList.front().y == SnakeList[i].y) {
			SnakeList.clear();
			return false;
		}
	}
	return true;
}





void tick(RenderWindow &window, vector <Cell> &SnakeList, int &direct, Apple &point_ball, bool &game_run, Sprite deadback, bool &lock_run) {
	if (!SnakeList.empty() and game_run) {
		vector <ChangePoint> tmp_list = SnakeList.back().PointList;
		if ((SnakeList.front().direction + 2 != direct) or (SnakeList.front().direction - 2 != direct)) {
			SnakeList.front().direction = direct;
		}
		move_head(SnakeList);
		drawning(SnakeList.front().shape, rad, Color::Blue, window, SnakeList.front().x, SnakeList.front().y);
		change_direct_cell(SnakeList);
		move_all(SnakeList);
		contack_with_apple(SnakeList, point_ball, tmp_list);
		drawning_all(window, SnakeList, point_ball);
		//game_run = crash_wall(SnakeList);
		game_run = crash_cell(SnakeList);
	}
	else {
		window.draw(deadback);
	}
	Sleep(SPEED_GAME);
}


void music_go(Music &classic_music, Music &rage_music) {
	if (SPEED_GAME < 150) {
		if (classic_music.getStatus()) {
			classic_music.stop();
		}
		if (!rage_music.getStatus()) {
			rage_music.play();
		}
	}
	else {
		if (rage_music.getStatus()) {
			rage_music.stop();
		}
		if (!classic_music.getStatus()) {
			classic_music.play();
		}
	}

}


int main()
{
	srand(time(NULL));
	vector <Cell> SnakeList;
	Apple point_ball;
	bool game_run = true;
	bool lock_run = false;


	String n;

	Font font;
	font.loadFromFile("times.ttf");
	
	Text score_text("", font, 20);
	score_text.setPosition(50, 50);

	Texture texture;
	texture.loadFromFile("endback.png");
	Sprite deadback(texture);

	Music classic_music;
	classic_music.openFromFile("sound_game.ogg");
	classic_music.setVolume(5);
	classic_music.setLoop(true);

	Music rage_music;
	rage_music.openFromFile("megalovania.ogg");
	rage_music.setVolume(5);
	rage_music.setLoop(true);

	

	RenderWindow window(VideoMode(W, H), "SnakeGame", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	int direct = 1;
	for (int i = 0;i < 3; i++) {
		SnakeList.push_back(Cell(300 - rad * 2 * i, 300, 1));
	}
	SnakeList.front().shape.setFillColor(Color::Blue);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			keyPress(window, event, direct, SnakeList, game_run, lock_run);
		}
		window.clear();
		cout << SnakeList.size() << endl;
		music_go(classic_music, rage_music);
		n = std::to_string(SnakeList.size());
		score_text.setString(n);
		window.draw(score_text);
		tick(window, SnakeList, direct, point_ball, game_run, deadback, lock_run);
		
		window.display();
	}
}