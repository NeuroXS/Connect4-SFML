#include "Game.h"
#include "Ball.h"
#include "Textures.h"
#include "CONSTANTS.h"
#include "Slot.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;



Game& Game::get_instance()
{
	static Game instance;
	return instance;
}

Game::Game() :
    Slots{ 6, std::vector<Slot>(7, Slot(CIRCLE_RADIUS)) },
    window{ VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Connect-4" },
    gridBackground{ { GRID_WIDTH, GRID_HEIGHT } },
    gameBackground{ {WINDOW_WIDTH, WINDOW_HEIGHT} },
    border{ {WINDOW_WIDTH, WINDOW_HEIGHT} },
    redText{ "Red..", Textures::rechargeFont },
    yellowText{ "Yellow..", Textures::rechargeFont },
    columnBorder{ {(2 * CIRCLE_RADIUS) + HORIZONTAL_SPACE_BETWEEN_CIRCLES, GRID_HEIGHT} },
    redBall{ Ball::Red },
    yellowBall{ Ball::Yellow },
    columns{ HORIZONTAL_CIRCLES },
    turn{ Red },
    currentBall{ &redBall },
    currentText{ &redText }
{
    window.setFramerateLimit(20);

    // set each slot's position
    float y = INITIAL_Y;
    for (int i = 0; i < VERTICAL_CIRCLES; i++)
    {
        float x = INITIAL_X;
        for (int j = 0; j < HORIZONTAL_CIRCLES; j++)
        {
            Slots[i][j].setPosition({ x,y });
            x += (CIRCLE_RADIUS * 2) + HORIZONTAL_SPACE_BETWEEN_CIRCLES;
        }
        y += (CIRCLE_RADIUS * 2) + VERTICAL_SPACE_BETWEEN_CIRCLES;
    }

    // set each slot's diagonal rows data
    
    //upper left
    
        // the right triangle
    for (int i = 0; i < HORIZONTAL_CIRCLES; i++)
    {
        for (int row = 0, column = i; column < HORIZONTAL_CIRCLES && row < VERTICAL_CIRCLES; row++, column++)
        {
            Slots[row][column].upperLeft = { 0, i };
        }
    }
        // left triangle
    for (int j = 1; j < VERTICAL_CIRCLES; j++)
    {
        for (int row = j, column = 0; row < VERTICAL_CIRCLES; row++, column++)
        {
            Slots[row][column].upperLeft = { j, 0 };
        }
    }

    // bottom left

        // the right triangle
    for (int i = 0; i < HORIZONTAL_CIRCLES; i++)
    {
        for (int row = VERTICAL_CIRCLES - 1, column = i; column < HORIZONTAL_CIRCLES && row >= 0; row--, column++)
        {
            Slots[row][column].bottomLeft = { VERTICAL_CIRCLES - 1, i };
        }
    }

        // left triangle
    for (int j = VERTICAL_CIRCLES - 2; j >= 0; j--)
    {
        for (int row = j, column = 0; row >= 0; row--, column++)
        {
            Slots[row][column].bottomLeft = { j, 0 };
        }
    }

    // Background

    gridBackground.setFillColor(Color(31 ,56, 137));
    gridBackground.setPosition(INITIAL_X, INITIAL_Y);
    gridBackground.setOutlineThickness(1.f);

    gameBackground.setTexture(&Textures::gameBackgroundTexture);
    gameBackground.setPosition(0, 0);

    // Border
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(-4);
    border.setOutlineColor(Color::Black);

    columnBorder.setFillColor(Color::Transparent);
    columnBorder.setOutlineThickness(-3);
    columnBorder.setOutlineColor(Color::Red);

    // Text
    redText.setFillColor(Color::Red);
    redText.setPosition({ 6,6 });

    yellowText.setFillColor(Color::Yellow);
    yellowText.setPosition({6, 6});

    // Columns
    columns[0].xi = INITIAL_X;
    columns[0].xf = INITIAL_X + COLUMN_WIDTH - ((float)HORIZONTAL_SPACE_BETWEEN_CIRCLES / 2.f);

    for (int i = 1; i < HORIZONTAL_CIRCLES; i++)
    {
        float endOfLast = columns[i - 1].xf;
        columns[i].xi = endOfLast;
        columns[i].xf = endOfLast + COLUMN_WIDTH;
    }


}

void Game::start()
{
    game_handler();
}

void Game::draw_content()
{
    window.clear();

    window.draw(gameBackground);

    window.draw(gridBackground);

    // draw balls
    for (int i = 0; i < VERTICAL_CIRCLES; i++)
    {
        for (int j = 0; j < HORIZONTAL_CIRCLES; j++)
        {
            window.draw(Slots[i][j]);
        }
    }

    window.draw(border);

    Vector2i position = Mouse::getPosition(window);
    int column = determine_column({ position.x, position.y });

    window.draw(*currentText);

    if (column != RED_FLAG)
    {
        window.draw(*currentBall);
        set_column(column);
        window.draw(columnBorder);
    }
}

void Game::game_handler()
{
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
            else if (e.type == Event::MouseButtonPressed)
            {
                if (e.mouseButton.button == Mouse::Left)
                {
                    int x = e.mouseButton.x;
                    int y = e.mouseButton.y;
                    int column = determine_column({ x, y });
                    if (play(column))
                    {
                        // x represents columns and y represents rows
                        static std::pair<Vector2i, Vector2i> winBounds;
                        static WinType type;

                        if (has_won(column, winBounds, type))
                        {
                            display_win_screen(winBounds, type);
                        }

                        if (turn == Red)
                        {
                            turn = Yellow;
                            columnBorder.setOutlineColor(Color::Yellow);
                            currentText = &yellowText;
                            currentBall = &yellowBall;
                        }
                        else
                        {
                            turn = Red;
                            columnBorder.setOutlineColor(Color::Red);
                            currentText = &redText;
                            currentBall = &redBall;
                        }
                    }
                }
            }
        }

        draw_content();
        window.display();
    }
}

int Game::determine_column(const Vector2i& mouseCord) const
{
    int x = mouseCord.x;

    if (mouseCord.y < INITIAL_Y || x < INITIAL_X || x > INITIAL_X + GRID_WIDTH)
        return RED_FLAG;

    for (int i = 0, end = columns.size() - 1; i < end; i++)
    {
        if (x < columns[i].xf)
        {
            return i;
        }
    }

    return columns.size() - 1;
}

void Game::set_column(int column)
{
    float columnXi = columns[column].xi;

    columnBorder.setPosition({ columnXi, INITIAL_Y });
    currentBall->setPosition({ columnXi, HOFB });
}

bool Game::play(int column)
{
    if (column == RED_FLAG || columns[column].isFilled)
        return false;

    int row = columns[column].topRow;
    columns[column].update_row();

    if (turn == Red)
    {
        Slots[row][column].setTexture(&Textures::redBallTexture);
    }
    else
    {
        Slots[row][column].setTexture(&Textures::yellowBallTexture);
    }

    return true;
}

bool Game::has_won(int column, std::pair<Vector2i, Vector2i>& winBounds, WinType& winType)
{
    // check the column vertically (starting bottom up)
    for (int i = VERTICAL_CIRCLES - 1; i >= 3; i--)
    {
        bool flag = true;
        const Texture* tex = Slots[i][column].getTexture();

        if (!tex)
        {
            break;
        }

        for (int j = i-1, end = i - 3; j >= end; j--)
        {
            if (Slots[j][column].getTexture() != tex)
            {
                flag = false;
                i = j + 1; // +1 because it's gonna be decreased in the for loop
                break;
            }
        }

        if (flag)
        {
            winBounds.first.x = column;
            winBounds.first.y = i;

            winBounds.second.x = column;
            winBounds.second.y = i - 3;
            winType = Vertical;
            return true;
        }
    }

    // check the row horizontally
    int row = columns[column].topRow + 1;

    for (int j = 0, end = HORIZONTAL_CIRCLES - 3; j < end; j++)
    {
        bool flag = true;
        const Texture* tex = Slots[row][j].getTexture();

        if (!tex)
            continue;

        for (int i = j+1, end = j + 3; i <= end; i++)
        {
            if (Slots[row][i].getTexture() != tex)
            {
                flag = false;
                j = i - 1; // -1 because it's gonna increase in the for loop def
                break;
            }
        }

        if (flag)
        {
            winBounds.first.x = j;
            winBounds.first.y = row;

            winBounds.second.x = j + 3;
            winBounds.second.y = row;

            winType = Horizontal;
            return true;
        }
    }

    // check diagonally (only the 'X' of the played ball)
    static int xEnd = VERTICAL_CIRCLES - 4;
    static int yEnd = HORIZONTAL_CIRCLES - 4;

        // starting the first x line from upper left
    Vector2i topLeft = Slots[row][column].upperLeft;

    for (int x = topLeft.x, y = topLeft.y; x <= xEnd && y <= yEnd; x++, y++)
        {
            const Texture* tex = Slots[x][y].getTexture();
            bool flag = true;

            if (!tex)
                continue;

            for (int x1 = x, y1 = y, counter = 0; counter < 4; counter++, x1++, y1++)
            {
                if (Slots[x1][y1].getTexture() != tex)
                {
                    flag = false;
                    x = x1 - 1; // -1 because it's gonna be incremented in upper loop
                    y = y1 - 1;
                    break;
                }
            }
            if (flag)
            {
                winBounds.first.x = y;
                winBounds.first.y = x;

                winBounds.second.x = y + 3;
                winBounds.second.y = x + 3;

                winType = UpperDiagonal;
                return true;
            }
        }

        // starting the second x line from bottom left
    Vector2i bottomLeft = Slots[row][column].bottomLeft;

    static int bottomXEnd = 3;

    for (int x = bottomLeft.x, y = bottomLeft.y; x >= bottomXEnd && y <= yEnd; x--, y++)
        {
            const Texture* tex = Slots[x][y].getTexture();
            bool flag = true;

            if (!tex)
                continue;

            for (int x1 = x, y1 = y, counter = 0; counter < 4; counter++, x1--, y1++)
            {
                if (Slots[x1][y1].getTexture() != tex)
                {
                    flag = false;
                    x = x1 + 1; // +1 because it's gonna be decremented in upper loop
                    y = y1 - 1;
                    break;
                }
            }
            if (flag)
            {
                winBounds.first.x = y;
                winBounds.first.y = x;

                winBounds.second.x = y + 3;
                winBounds.second.y = x - 3;

                winType = BottomDiagonal;
                return true;
            }
        }


    return false;
}

void Game::display_win_screen(const std::pair<Vector2i, Vector2i>& winBounds, const WinType& winType)
{
    columnBorder.setOutlineColor(Color::Transparent);

    const static int winLineThickniss = 5;
    Vector2f firstP{ Slots[winBounds.first.y][winBounds.first.x].getPosition() };
    Vector2f secondP{ Slots[winBounds.second.y][winBounds.second.x].getPosition() };

    RectangleShape winLine{ {winLineThickniss, distance_between_2points(firstP, secondP)}};
    winLine.setFillColor(Color::Green);
    winLine.setPosition({firstP.x + CIRCLE_RADIUS, firstP.y+CIRCLE_RADIUS});

    // rotation
    switch (winType)
    {
    case Vertical:
        winLine.setRotation(180.f);
        break;
    case Horizontal:
        winLine.setRotation(270.f);
        break;
    case UpperDiagonal:
        winLine.setRotation(315.f);
        break;
    case BottomDiagonal:
        winLine.setRotation(225.f);
        break;
    }


    Text winText;
    winText.setFont(Textures::rechargeFont);
    std::string winTextString = "THE WINNER IS ";

    Texture* winTrophy = new Texture();
    

    if (turn == Red)
    {
        winTextString.append("RED!!");
        winText.setFillColor(Color::Red);

        if (!winTrophy->loadFromFile("resources\\redTrophy.png"))
        {
            throw std::exception("Red trophy image couldn't be loaded");
        }
    }
    else
    {
        winTextString.append("YELLOW!!");
        winText.setFillColor(Color::Yellow);

        if (!winTrophy->loadFromFile("resources\\yellowTrophy.png"))
        {
            throw std::exception("Red trophy image couldn't be loaded");
        }
    }

    winText.setString(winTextString);
    winText.setOutlineThickness(2.f);
    winText.setOutlineColor(Color::Green);
    winText.setCharacterSize(40);

    FloatRect winTextBounds = winText.getGlobalBounds();

    winText.setPosition({ (WINDOW_WIDTH - winTextBounds.width) / 2 ,0 });

    // Trophy
    
    const float blankSideWidth = ((WINDOW_WIDTH - GRID_WIDTH) / 2);

    RectangleShape trophy({ blankSideWidth * 80/100, WINDOW_HEIGHT * 30/100});
    trophy.setTexture(winTrophy);

    FloatRect trophyBounds = trophy.getGlobalBounds();
    trophy.setPosition({ blankSideWidth * 5/100, WINDOW_HEIGHT -  trophyBounds.height});

    // hide things
    currentText->setFillColor(Color::Transparent);
    currentBall->setFillColor(Color::Transparent);

    draw_content();

    window.draw(winLine);
    window.draw(winText);

    window.draw(trophy);

    trophy.setPosition({ (WINDOW_WIDTH - (blankSideWidth * 5 / 100) - trophyBounds.width),
        WINDOW_HEIGHT - trophyBounds.height });

    window.draw(trophy);


    window.display();

    while(window.isOpen())
    {
        sf::sleep(sf::milliseconds(Int32(100)));
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
        }
    }

    delete winTrophy;
}

float distance_between_2points(const Vector2f& p1, const Vector2f& p2)
{
    return std::sqrt(pow(p1.x - p2.x, 2.f) + std::pow(p1.y - p2.y, 2.f));
}