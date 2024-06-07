#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <vector>
#include <ctime>

using namespace std;
//unsigned int microseconds;
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))
const int WIDTH=10+2;
const int HEIGHT=20+1;
const int FIGURE = 2;
const int BORDER = 1;
const int EMPTY = 0;
//void setColor(int text){
//    cout<<"\e[38;5;"<<text<<"m";
//}
class Exit{
private:
    int ex = 0;
public:
    int getStatusOfExit(){
        return ex;
    }
    void changeStatusOfExit(){
        ex=!ex;
    }
};
class Score{
private:
    int tempScore = 0;
public:
    Score(){
        tempScore = 0;
    }
    void show(){
        MOVETO(0,30);
        cout<<"SCORE: "<<tempScore;
    }
    void plusScore(int changeScore){
        tempScore += changeScore;
    }
    int scoreGet(){
        return tempScore;
    }
    void gameOver(){
        MOVETO(9,66);
        cout<<"Your score: "<<tempScore;
    }
};
struct Point{
    int x;
    int y;
};
class Figure{
private:
    vector <Point> vec;
    vector <Point>* vecPtr = &vec;
    int newFig=1;
    int color = 231;
public:
    void push(int x, int y){
        Point temp;
        temp.x=x;
        temp.y=y;
        vec.push_back(temp);
    }
    void change(int x, int y, int index){
        vec[index].x = x;
        vec[index].y = y;
    }
    int hide(){
        return 0;
    }
    int show(){
        return FIGURE;
    }
    void moveDown(){
        for(int i=0;i<vec.size();i++){
            vec[i].y++;
        }
    }
    void moveRight(){
        for(int i=0;i<vec.size();i++){
            vec[i].x++;
        }
    }
    void moveLeft(){
        for(int i=0;i<vec.size();i++){
            vec[i].x--;
        }
    }
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
    bool isUp(){
        for(int i=0;i<vec.size();i++){
            if(vec[i].y==0)
                return 1;
        }
        return 0;
    }
    int getStatusOfNewFig(){
        return newFig;
    }
    void changeStatusOfNewFig(){
        newFig=!newFig;
    }
    vector <Point>* getPtrOnPoint(){
        return vecPtr;
    }
    void changeColor(int newColor){
        color = newColor;
    }
    int getColor(){
        return color;
    }
};
class Field{
private:
    vector<vector<vector<int> > > vec;
    vector<vector<vector<int> > >::iterator it;
    int fieldColor;
public:
    Field(int fieldColor_value = 163){
        fieldColor = fieldColor_value;
    }
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
    void coutColor(string ch, int color){
        cout<<"\e[38;5;"<<color<<"m";
        cout<<ch;
        cout<<"\e[38;5;"<<231<<"m";
    }
    bool canPutFigureInField(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==FIGURE)
                return 0;
        }
        return 1;
    }
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
    void changeField(vector <Point>* vecPtr, int hideOrShow, int color){
        for(int i=0;i<(*vecPtr).size();i++){
            vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]=hideOrShow;
            vec[(*vecPtr)[i].y][(*vecPtr)[i].x][1]=color;
        }
    }
    int stopMoveDown(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y+1][(*vecPtr)[i].x][0]!=EMPTY)
                return 1;
        }
        return 0;
    }
    bool canNotMoveRight(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x+1][0]==BORDER or vec[(*vecPtr)[i].y][(*vecPtr)[i].x+1][0]==FIGURE)
                return 1;
        }
        return 0;
    }
    bool canNotMoveLeft(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x-1][0]==BORDER or vec[(*vecPtr)[i].y][(*vecPtr)[i].x-1][0]==FIGURE)
                return 1;
        }
        return 0;
    }
    bool canNotTurn(vector <Point>* vecPtr){
        for(int i=0;i<(*vecPtr).size();i++){
            if (vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==BORDER or (*vecPtr)[i].y <= 0 or vec[(*vecPtr)[i].y][(*vecPtr)[i].x][0]==FIGURE)
                return 1;
        }
        return 0;
    }
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

class Game{
private:
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
    void showLogo(){
        system("clear");
        MOVETO(8,68);
        cout<<"Tetris"<<endl;
        sleep(2);
        system("clear");
    }
    void gameOver(){
        MOVETO(8,68);
        cout<<"Game over"<<endl;
        sleep(2);
    }
public:
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

