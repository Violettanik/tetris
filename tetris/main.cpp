#include "tetris.cpp"
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
int main(){
    srand(time(0));
    system("clear");
    Game game;
    game.run();
}


