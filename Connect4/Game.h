#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Column.h"
#include "Slot.h"

using namespace sf;
using namespace std;

//Game is a singleton
class Game
{
public:
	static Game& get_instance();
	void start();

	// delete copy constructor and assignment operator
	Game(const Game&) = delete;
	Game& operator=(Game&) = delete;
private:
	Game();

	void game_handler();

	int determine_column(const Vector2i& mouseCord) const;

	void set_column(int column);

	bool play(int column);

	void draw_content();

	enum WinType{Horizontal, Vertical, UpperDiagonal, BottomDiagonal};

	bool has_won(int column, std::pair<Vector2i, Vector2i>& winBounds, WinType& winType);
	
	void display_win_screen(const std::pair<Vector2i, Vector2i>& winBounds, const WinType& winType);

	// ball slots
	std::vector<std::vector<Slot>> Slots;
	// window
	RenderWindow window;

	// grid background
	RectangleShape gridBackground;
	RectangleShape gameBackground;

	// border
	
		// full window border
	RectangleShape border;

		// column border
	RectangleShape columnBorder;
	
	// text
	Text redText;
	Text yellowText;

	// moving balls
	Ball redBall;
	Ball yellowBall;

	// columns
	std::vector<Column> columns;


	// turn
	enum Turn{Red, Yellow};

	Turn turn;

	// abstraction pointers
	Ball* currentBall;
	Text* currentText;
};

float distance_between_2points(const Vector2f& p1, const Vector2f& p2);

