#include "tetris.cpp"
#include <algorithm>
#include <doctest.h>

TEST_CASE("score test 1")
{
    Score score;
    score.plusScore(1);
    REQUIRE( score.scoreGet() == 1);
}

TEST_CASE("score test 2")
{
    Score score;
    score.plusScore(4);
    REQUIRE( score.scoreGet() == 4);
}

TEST_CASE("score test 3")
{
    Score score;
    score.plusScore(100);
    REQUIRE( score.scoreGet() == 100);
}

TEST_CASE("exit test 1")
{
    Exit exit;
    exit.changeStatusOfExit();
    REQUIRE( exit.getStatusOfExit() == 1);

}

TEST_CASE("exit test 2")
{
    Exit exit;
    exit.changeStatusOfExit();
    exit.changeStatusOfExit();
    REQUIRE( exit.getStatusOfExit() == 0);
}

TEST_CASE("figure test push")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 5 && vec[0].y == 0 && vec[1].x == 6 && vec[1].y == 0 && vec[2].x == 5 && vec[2].y == 1 && vec[3].x == 6 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test change")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    fig.change(5,0,0);
    fig.change(5,1,1);
    fig.change(5,2,2);
    fig.change(5,3,3);
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 5 && vec[0].y == 0 && vec[1].x == 5 && vec[1].y == 1 && vec[2].x == 5 && vec[2].y == 2 && vec[3].x == 5 && vec[3].y == 3) == 1);
}

TEST_CASE("figure test hide")
{
    Figure fig;
    REQUIRE(fig.hide() == 0);
}

TEST_CASE("figure test show")
{
    Figure fig;
    REQUIRE(fig.show() == FIGURE);
}

TEST_CASE("figure test moveDown")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    fig.moveDown();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 5 && vec[0].y == 1 && vec[1].x == 6 && vec[1].y == 1 && vec[2].x == 5 && vec[2].y == 2 && vec[3].x == 6 && vec[3].y == 2) == 1);
}

TEST_CASE("figure test moveRight")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    fig.moveRight();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 6 && vec[0].y == 0 && vec[1].x == 7 && vec[1].y == 0 && vec[2].x == 6 && vec[2].y == 1 && vec[3].x == 7 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test moveLeft")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    fig.moveLeft();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 4 && vec[0].y == 0 && vec[1].x == 5 && vec[1].y == 0 && vec[2].x == 4 && vec[2].y == 1 && vec[3].x == 5 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test turn 1")
{
    Figure fig;
    fig.push(5,0);
    fig.push(6,0);
    fig.push(5,1);
    fig.push(6,1);
    fig.turn();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 5 && vec[0].y == 0 && vec[1].x == 6 && vec[1].y == 0 && vec[2].x == 5 && vec[2].y == 1 && vec[3].x == 6 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test turn 2")
{
    Figure fig;
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    fig.push(5,3);
    fig.turn();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 4 && vec[0].y == 1 && vec[1].x == 5 && vec[1].y == 1 && vec[2].x == 6 && vec[2].y == 1 && vec[3].x == 7 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test turn 3")
{
    Figure fig;
    fig.push(5,0);
    fig.push(5,1);
    fig.push(4,1);
    fig.push(5,2);
    fig.turn();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 4 && vec[0].y == 1 && vec[1].x == 5 && vec[1].y == 1 && vec[2].x == 5 && vec[2].y == 2 && vec[3].x == 6 && vec[3].y == 1) == 1);
}

TEST_CASE("figure test turn 4")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    fig.turn();
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 5 && vec[0].y == 1 && vec[1].x == 5 && vec[1].y == 0 && vec[2].x == 6 && vec[2].y == 0 && vec[3].x == 7 && vec[3].y == 0) == 1);
}

TEST_CASE("figure test isUp 1")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    REQUIRE(fig.isUp() == 1);
}

TEST_CASE("figure test isUp 2")
{
    Figure fig;
    fig.push(4,1);
    fig.push(5,1);
    fig.push(5,2);
    fig.push(5,3);
    REQUIRE(fig.isUp() == 0);
}

TEST_CASE("figure test changeStatusOfNewFig")
{
    Figure fig;
    fig.changeStatusOfNewFig();
    REQUIRE(fig.getStatusOfNewFig() == 0);
}

TEST_CASE("field test canPutFigureInField")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    REQUIRE(field.canPutFigureInField(fig.getPtrOnPoint()) == 1);
}

TEST_CASE("field test stopMoveDown 1")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    REQUIRE(field.stopMoveDown(fig.getPtrOnPoint()) == 0);
}


TEST_CASE("field test stopMoveDown 2")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    while(!field.stopMoveDown(fig.getPtrOnPoint())){
        fig.moveDown();
    }
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 4 && vec[0].y == 17 && vec[1].x == 5 && vec[1].y == 17 && vec[2].x == 5 && vec[2].y == 18 && vec[3].x == 5 && vec[3].y == 19) == 1);
}

TEST_CASE("field test canNotMoveRight 1")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    REQUIRE(field.canNotMoveRight(fig.getPtrOnPoint()) == 0);
}

TEST_CASE("field test canNotMoveRight 2")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    while(!field.canNotMoveRight(fig.getPtrOnPoint())){
        fig.moveRight();
    }
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 9 && vec[0].y == 0 && vec[1].x == 10 && vec[1].y == 0 && vec[2].x == 10 && vec[2].y == 1 && vec[3].x == 10 && vec[3].y == 2) == 1);
}

TEST_CASE("field test canNotMoveLeft 1")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    REQUIRE(field.canNotMoveLeft(fig.getPtrOnPoint()) == 0);
}

TEST_CASE("field test canNotMoveLeft 2")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    Field field;
    field.initField();
    while(!field.canNotMoveLeft(fig.getPtrOnPoint())){
        fig.moveLeft();
    }
    vector<Point> vec = *fig.getPtrOnPoint();
    REQUIRE((vec[0].x == 1 && vec[0].y == 0 && vec[1].x == 2 && vec[1].y == 0 && vec[2].x == 2 && vec[2].y == 1 && vec[3].x == 2 && vec[3].y == 2) == 1);
}

TEST_CASE("field test canNotTurn 1")
{
    Figure fig;
    fig.push(4,0);
    fig.push(5,0);
    fig.push(5,1);
    fig.push(5,2);
    fig.moveDown();
    Field field;
    field.initField();
    REQUIRE(field.canNotTurn(fig.getPtrOnPoint()) == 0);
}

TEST_CASE("field test canNotTurn 2")
{
    Figure fig;
    fig.push(10,1);
    fig.push(11,1);
    fig.push(11,2);
    fig.push(11,3);
    Field field;
    field.initField();
    REQUIRE(field.canNotTurn(fig.getPtrOnPoint()) == 1);
}

/*
TEST_CASE("can iterate not full")
{
    tree t{{{1, 2, nullptr}, 3, {nullptr, 4, 5}}};
    int v = 1;
    for (int i : t) {
        CAPTURE(i);
        CHECK(i == v++);
    }
    REQUIRE(v == 6);
}

TEST_CASE("can reverse iterate")
{
    tree t{{{1, 2, 3}, 4, {5, 6, 7}}};
    int v = 7;
    std::for_each(std::make_reverse_iterator(t.end()),
                  std::make_reverse_iterator(t.begin()), [&v](int i) {
                      CAPTURE(i);
                      CHECK(i == v--);
                  });
    REQUIRE(v == 0);
}
*/
