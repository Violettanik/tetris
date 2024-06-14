#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>
#include <ctime>

using namespace std;
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))
/**
* @var int WIDTH
* Значение ширины поля игры.
*/
const int WIDTH=10+2;

/**
* @var int HEIGHT
* Значение высоты поля игры.
*/
const int HEIGHT=20+1;

/**
* @var int HEIGHT
* Значение точки поля, занятой частью фигуры.
*/
const int FIGURE = 2;

/**
* @var int BORDER
* Значение точки, являющейся частью границы.
*/
const int BORDER = 1;

/**
* @var int EMPTY
* Значение точки поля, являющейся пустой.
*/
const int EMPTY = 0;

/** 
* @brief Класс Exit для управления статусом выхода.
* 
* Этот класс позволяет управлять статусом выхода, 
* предоставляя методы для получения текущего статуса и изменения его значения.
*/
class Exit {
private:
    int ex = 0; ///< Переменная для хранения статуса выхода. 0 - выход закрыт, 1 - выход открыт.
public:
    /**
    * @brief Метод getStatusOfExit возвращает текущий статус выхода.
    * 
    * @return Текущий статус выхода (0 или 1).
    */
    int getStatusOfExit() {
        return ex;
    }

    /**
    * @brief Метод changeStatusOfExit изменяет статус выхода на противоположный.
    * 
    * Если выход был закрыт (статус 0), после вызова метода выход станет открытым (статус 1) и наоборот.
    */
    void changeStatusOfExit() {
        ex = !ex;
    }
};

/**
* @brief Класс Score для управления счетом игры.
* 
* Этот класс представляет собой счетчик игровых очков и позволяет управлять им:
* увеличивать счет, отображать текущее значение, получать текущее значение и выводить счет в конце игры.
*/
class Score {
private:
    int tempScore = 0; ///<Переменная для хранения текущего счета.
public:
    /**
    * @brief Конструктор класса Score. Устанавливает начальное значение счета.
    */
    Score() {
        tempScore = 0;
    }

    /**
    * @brief Метод show отображает текущий счет на экране.
    */
    void show() {
        MOVETO(0,30);
        cout << "SCORE: " << tempScore;
    }

    /**
    * @brief Метод plusScore увеличивает текущий счет на указанное значение.
    *
    * @param changeScore Значение, на которое увеличивается счет.
    */
    void plusScore(int changeScore) {
        tempScore += changeScore;
    }

    /**
    * @brief Метод scoreGet возвращает текущее значение счета.
    * 
    * @return Текущее значение счета.
    */
    int scoreGet() {
        return tempScore;
    }

    /**
    * @brief Метод gameOver выводит сообщение с итоговым счетом в конце игры.
    */
    void gameOver() {
        MOVETO(9,66);
        cout << "Your score: " << tempScore;
    }
};

/**
* @brief Структура Point для хранения координат точки в двумерном пространстве.
*
* Эта структура позволяет хранить координаты точки по оси X и Y.
*/
struct Point {
    int x; ///< Координата X точки в двумерном пространстве.
    int y; ///< Координата Y точки в двумерном пространстве.
};

/**
* @brief Класс Figure для представления фигуры в игре.
*
* Класс Figure представляет собой фигуру, состоящую из точек (Point), цвет и методы для управления фигурой.
*/
class Figure {
private:
    vector<Point> vec; ///< Вектор точек, составляющих фигуру.
    vector<Point>* vecPtr = &vec; ///< Указатель на вектор точек.
    int newFig = 1; ///< Флаг новой фигуры (1 - новая фигура, 0 - не новая фигура).
    int color = 231; ///< Цвет фигуры в игре.
public:
    /**
    * @brief Метод push добавляет новую точку в вектор.
    * 
    * @param x Координата X точки.
    * @param y Координата Y точки.
    */
    void push(int x, int y){
        Point temp;
        temp.x=x;
        temp.y=y;
        vec.push_back(temp);
    }

    /**
    * @brief Метод change изменяет координаты точки с заданным индексом.
    * 
    * @param x Новая координата X точки.
    * @param y Новая координата Y точки.
    * @param index Индекс точки в векторе.
    */
    void change(int x, int y, int index){
        vec[index].x = x;
        vec[index].y = y;
    }

    /**
    * @brief Метод hide скрывает фигуру.
    * 
    * @return Всегда возвращает 0.
    */
    int hide(){
        return 0;
    }

    /**
    * @brief Метод show отображает фигуру.
    * 
    * @return Всегда возвращает константу FIGURE.
    */
    int show(){
        return FIGURE;
    }

    /**
    * @brief Метод moveDown перемещает фигуру вниз.
    */
    void moveDown(){
        for(int i=0;i<vec.size();i++){
            vec[i].y++;
        }
    }

    /**
    * @brief Метод moveRight перемещает фигуру вправо.
    */
    void moveRight(){
        for(int i=0;i<vec.size();i++){
            vec[i].x++;
        }
    }

    /**
    * @brief Метод moveLeft перемещает фигуру влево.
    */
    void moveLeft(){
        for(int i=0;i<vec.size();i++){
            vec[i].x--;
        }
    }

    /**
    * @brief Метод turn поворачивает фигуру.
    * 
    * @return 1 если фигуру необходимо повернуть ещё 2 раза, иначе 0.
    */
    int turn(){
        if(vec[2].x == (vec[1].x - 1) and vec[2].y == (vec[1].y + 1))
            return 0;
        for(int i = 0; i < vec.size(); i++){
            if (i == 1)
                continue;
            else if(vec[i].y == vec[1].y){
                vec[i].y += vec[1].x - vec[i].x;
                vec[i].x = vec[1].x;
            }
            else {
                vec[i].x -= vec[1].y - vec[i].y;
                vec[i].y = vec[1].y;
            }
        }
        if(vec[0].x == vec[2].x && vec[0].x == vec[1].x && vec[0].x == vec[3].x){
            return 1;
        }   
        return 0;
    }

    /**
    * @brief Метод isUp проверяет, находится ли фигура сверху.
    * 
    * @return true если хотя бы одна точка фигуры находится на верхней границе, иначе false.
    */
    bool isUp(){
        for(int i=0;i<vec.size();i++){
            if(vec[i].y==0)
                return 1;
        }
        return 0;
    }

    /**
    * @brief Метод getStatusOfNewFig возвращает статус новой фигуры.
    * 
    * @return Статус новой фигуры (1 - новая, 0 - не новая).
    */
    int getStatusOfNewFig(){
        return newFig;
    }

    /**
    * @brief Метод changeStatusOfNewFig изменяет статус новой фигуры.
    */
    void changeStatusOfNewFig(){
        newFig=!newFig;
    }

    /**
    * @brief Метод getPtrOnPoint возвращает указатель на вектор точек фигуры.
    * 
    * @return Указатель на вектор точек.
    */
    vector <Point>* getPtrOnPoint(){
        return vecPtr;
    }

    /**
    * @brief Метод changeColor меняет цвет фигуры.
    * 
    * @param newColor Новый цвет для фигуры.
    */
    void changeColor(int newColor){
        color = newColor;
    }

    /**
    * @brief Метод getColor возвращает текущий цвет фигуры.
    * 
    * @return Текущий цвет фигуры.
    */
    int getColor(){
        return color;
    }
};
/**
* @brief Класс Field
* 
* Этот класс представляет собой игровое поле.
*/
class Field{
private:
    vector<vector<vector<int>>> vec; ///< 3D вектор, представляющий поле.
    vector<vector<vector<int>>>::iterator it; ///< Итератор для вектора поля.
    int fieldColor; ///< Цвет поля.

public:
    /**
    * Конструктор для класса Field.
    * 
    * @param fieldColor_value Цвет поля (значение по умолчанию - 163).
    */
    Field(int fieldColor_value = 163){
        fieldColor = fieldColor_value;
    }

    /**
    * @brief Метод initField инициализирует поле с границами и пустыми пространствами.
    */
    void initField(){
        vector<vector<int> > temp1;
        vector<int> temp2;
        temp2.push_back(BORDER);
        temp2.push_back(fieldColor);
        temp1.push_back(temp2);
        temp2.clear();
        for(int i=1;i<WIDTH-1;i++){
            temp2.push_back(EMPTY);
            temp2.push_back(fieldColor);
            temp1.push_back(temp2);
            temp2.clear();
        }
        temp2.push_back(BORDER);
        temp2.push_back(fieldColor);
        temp1.push_back(temp2);
        for(int i=0;i<HEIGHT-1;i++)
            vec.push_back(temp1);
        temp1.clear();
        for(int i=0;i<WIDTH;i++)
            temp1.push_back(temp2);
        vec.push_back(temp1);
    }

    /**
    * @brief Метод coutColor выводит цветной текст в консоль.
    * 
    * @param ch Символ для вывода.
    * @param color Код цвета текста.
    */
    void coutColor(string ch, int color){
        cout<<"\e[38;5;"<<color<<"m";
        cout<<ch;
        cout<<"\e[38;5;"<<231<<"m";
    }

    /**
    * @brief Метод canPutFigureInField проверяет, можно ли разместить фигуру на поле по заданным точкам.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return True, если фигуру можно разместить на поле, иначе false.
    */
    bool canPutFigureInField(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==FIGURE)
                return 0;
        }
        return 1;
    }

    /**
    * @brief Метод showField показывает текущее состояние поля в консоли.
    */
    void showField(){
        MOVETO(0,0);
        for(int i=0;i<vec.size();i++){
            for(int j=0;j<vec[i].size();j++){
                if (vec[i][j][0]==BORDER)
                    coutColor("# ",vec[i][j][1]);
                else if (vec[i][j][0]==EMPTY)
                    coutColor("  ",vec[i][j][1]);
                else if (vec[i][j][0]==FIGURE)
                    coutColor("* ",vec[i][j][1]);
            }
            cout<<endl;
        }
    }

    /**
    * @brief Метод changeField изменяет состояние поля в заданных точках для скрытия или показа фигуры.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @param hideOrShow 0 для скрытия фигуры, 1 для показа фигуры.
    * @param color Цвет фигуры
    */
    void changeField(vector <Point>* vecPtr, int hideOrShow, int color){
        for(int i=0;i<(*vecPtr).size();i++){
            vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]=hideOrShow;
            vec[(*vecPtr)[i].y][(*vecPtr)[i].x][1]=color;
        }
    }

    /**
    * @brief Метод stopMoveDown проверяет, что фигура не может двигаться вниз дальше.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return 1, если фигура не может двигаться вниз дальше, иначе 0.
    */
    int stopMoveDown(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y+1][(*vecPtr)[i].x][0]!=EMPTY)
                return 1;
        }
        return 0;
    }

    /**
    * @brief Метод canNotMoveRight проверяет, что фигура не может двигаться вправо.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return True, если фигура не может двигаться вправо, иначе false.
    */
    bool canNotMoveRight(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x+1][0]==BORDER or vec[(*vecPtr)[i].y][(*vecPtr)[i].x+1][0]==FIGURE)
                return 1;
        }
        return 0;
    }

    /**
    * @brief Метод canNotMoveRight проверяет, что фигура не может двигаться влево.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return True, если фигура не может двигаться влево, иначе false.
    */
    bool canNotMoveLeft(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x-1][0]==BORDER or vec[(*vecPtr)[i].y][(*vecPtr)[i].x-1][0]==FIGURE)
                return 1;
        }
        return 0;
    }

    /**
    * @brief Метод canNotTurn проверяет, что фигуру нельзя повернуть.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return True, если фигуру нельзя повернуть, иначе false.
    */
    bool canNotTurn(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==BORDER or (*vecPtr)[i].y <= 0 or vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==FIGURE)
                return 1;
        }
        return 0;
    }

    /**
    * @brief Метод canNotTurn удаляет завершенные строки на поле.
    * 
    * @param vecPtr Указатель на вектор точек, представляющих фигуру.
    * @return Количество удаленных строк.
    */
    int deleteStr(vector <Point>* vecPtr){
        int numberOfDeletedStr = 0;
        int k = 0;

        for(int i=0;i<(*vecPtr).size();i++){
            for(int j=1;j<vec[(*vecPtr)[i].y].size()-1;j++){
                if(vec[(*vecPtr)[i].y][j][0] == FIGURE)
                    k++;
            }
            if(k == vec[(*vecPtr)[i].y].size()-2){
                numberOfDeletedStr++;
                it = vec.begin() + (*vecPtr)[i].y;
                vec.erase(it);
                vector<vector<int> > temp1;
                vector<int> temp2;
                temp2.push_back(BORDER);
                temp2.push_back(fieldColor);
                temp1.push_back(temp2);
                temp2.clear();
                for(int i=1;i<WIDTH-1;i++){
                    temp2.push_back(EMPTY);
                    temp2.push_back(fieldColor);
                    temp1.push_back(temp2);
                    temp2.clear();
                }
                temp2.push_back(1);
                temp2.push_back(fieldColor);
                temp1.push_back(temp2);
                vec.insert(vec.begin(), temp1);
            }
            k = 0;
        }
        return numberOfDeletedStr;
    }
};

/**
* @brief Класс Game
* Этот класс представляет контроллер Тетрис.
*/
class Game{
private:
    /**
    * @brief Метод kbhit проверяет, была ли нажата клавиша.
    * 
    * @return true, если была нажата клавиша, false в противном случае.
    */
    bool kbhit()
    {
        termios term;
        tcgetattr(0, &term);

        termios term2 = term;
        term2.c_lflag &= ~ICANON;
        tcsetattr(0, TCSANOW, &term2);

        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting);

        tcsetattr(0, TCSANOW, &term);

        return byteswaiting > 0;
    }
    
    /**
    * @brief Метод _getch получает символ из потока ввода без ожидания новой строки.
    * 
    * @param c Символ, считанный из потока ввода.
    * @return Символ, считанный из потока ввода.
    */
    int _getch(int c){
        static struct termios oldt, newt;
        tcgetattr( STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
        c=getchar();
        return c;
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    }

    /**
    * @brief Метод showLogo отображает логотип игры на экране.
    */
    void showLogo(){
        system("clear");
        MOVETO(8,68);
        cout<<"Tetris"<<endl;
        sleep(2);
        system("clear");
    }

    /**
    * @brief Метод gameOver отображает сообщение "Game Over" на экране.
    */
    void gameOver(){
        MOVETO(8,68);
        cout<<"Game over"<<endl;
        sleep(2);
    }
    
public:
    /**
    * @brief Метод run запускает игровой цикл.
    * 
    * @return 0 после завершения игры.
    */
    int run(){
        showLogo();
        Field field(150);
        field.initField();
        field.showField();
        int ch=0;
        Exit exit;
        Score score;
        Figure fig;
        fig.push(5,0);
        fig.push(6,0); 
        fig.push(5,1);
        fig.push(6,1);
        while(!exit.getStatusOfExit()){
            ch= _getch(ch);
            switch(ch){
                case 100:
                    if (!field.canNotMoveRight(fig.getPtrOnPoint()))
                        fig.moveRight();
                    break;//вправо
                case 97:
                    if (!field.canNotMoveLeft(fig.getPtrOnPoint()))
                        fig.moveLeft();
                    break;//влево
                case 32:
                    if(fig.turn() == 1){
                        fig.turn();
                        fig.turn();
                    }
                    if (field.canNotTurn(fig.getPtrOnPoint())){
                        fig.turn();
                        fig.turn();
                        fig.turn();
                    }
                    break;//пробел
                case 115:
                    while(!field.stopMoveDown(fig.getPtrOnPoint())){
                        field.changeField(fig.getPtrOnPoint(),fig.show(),fig.getColor());
                        field.showField();
                        score.show();
                        usleep(10*1000);
                        field.changeField(fig.getPtrOnPoint(),fig.hide(),fig.getColor());
                        field.showField();
                        score.show();
                        fig.moveDown();
                    }
                    break;//вниз
            }
            while(!kbhit() and !exit.getStatusOfExit()){
                if(fig.getStatusOfNewFig()){
                    fig.changeStatusOfNewFig();
                    switch(int typeOfFig=rand()%(4)){
                        case 0:
                            fig.change(5,0,0);
                            fig.change(6,0,1);
                            fig.change(5,1,2);
                            fig.change(6,1,3);
                            break;
                        case 1:
                            fig.change(5,0,0);
                            fig.change(5,1,1);
                            fig.change(5,2,2);
                            fig.change(5,3,3);
                            break;
                        case 2:
                            fig.change(5,0,0);
                            fig.change(5,1,1);
                            fig.change(4,1,2);
                            fig.change(5,2,3);
                            break;
                        case 3:
                            fig.change(4,0,0);
                            fig.change(5,0,1);
                            fig.change(5,1,2);
                            fig.change(5,2,3);
                            break;
                        }
                    switch(int newColor = rand()%(7)){
                        case 0:
                            fig.changeColor(228);
                            break;
                        case 1:
                            fig.changeColor(147);
                            break;
                        case 2:
                            fig.changeColor(75);
                            break;
                        case 3:
                            fig.changeColor(125);
                            break;
                        case 4:
                            fig.changeColor(218);
                            break;
                        case 5:
                            fig.changeColor(192);
                            break;
                        case 6:
                            fig.changeColor(55);
                            break;
                        case 7:
                            fig.changeColor(159);
                            break;
                    }
                    if(!field.canPutFigureInField(fig.getPtrOnPoint())){
                        fig.changeColor(1);
                    }
                }
                if(!fig.getStatusOfNewFig()){
                    if(!field.stopMoveDown(fig.getPtrOnPoint())){
                        field.changeField(fig.getPtrOnPoint(),fig.show(),fig.getColor());
                        field.showField();
                        score.show();
                        usleep(500*1000);
                        field.changeField(fig.getPtrOnPoint(),fig.hide(),fig.getColor());
                        field.showField();
                        score.show();
                        fig.moveDown();
                    }
                    else {
                        fig.changeStatusOfNewFig();
                        field.changeField(fig.getPtrOnPoint(),fig.show(),fig.getColor());
                        field.showField();
                        score.show();
                        score.plusScore(field.deleteStr(fig.getPtrOnPoint()));
                        usleep(500*1000);
                    }
                    if(fig.isUp()){
                        exit.changeStatusOfExit();
                        gameOver();
                        score.gameOver();
                    }

                }
            }
            score.show();
            MOVETO(HEIGHT+5,0);
        }
        return 0;
    }
};
