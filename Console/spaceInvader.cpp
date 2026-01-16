#include <iostream>
#include <algorithm>
#include <cmath>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <chrono>

bool alive;
bool notDead;
char board[15][45];
bool alien[6][11];

int board_x = 15;
int board_y = 45;

double alien_offset_x;
int alien_offset_y;

int player_x;
int player_y;

int points;

enum Direction {RIGHT, LEFT, STOP};
Direction alienDirection;

std::chrono::steady_clock::time_point lastShotTime;

void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


class Projectile{
    public:
    int y, dir;
    double x;
    bool active;
    bool hit;

    Projectile(int x_,int y_, int dir_){
        x = x_;
        y = y_;
        dir = dir_;
        active = true;
    }
};

std::vector<Projectile> projectiles;

void end_game(bool win_or_lose);

void init_board(){
    for(int i=0; i<board_x; i++){
        for(int j=0; j<board_y; j++){
            if(i==0 || j==0 || j==board_y-1 || i==board_x-1) board[i][j]='#';
            else board[i][j]=' ';
        }
    }
};

void init_alien(){
    for(int i=0; i<6; i++){
        for(int j=0; j<11; j++){
            if(i!= 0)alien[i][j]=true;
            else alien[i][j]=false;
        }
    }
};


void alienAttack(int x, int y){
    projectiles.push_back(Projectile(x+1, y, 1));
}

void render_aliens(){
    for(int i=0; i<6; i++){
        for(int j=0; j<11; j++){
            if(alien[i][j]){
                int x= alien_offset_x + j * 2;
                int y = alien_offset_y +i;
                if(x>=0 && x<board_y && y>=0 && y<board_x)
                    board[y][x]='@';
                if (rand()%1000==0) alienAttack(y, x);
            }
        }
    }
}

void render_projectiles(){
    for (auto& p : projectiles) {
        if(p.x>=board_x-1) p.active=false;
        if (p.active){
            if(board[(int)p.x][p.y]=='@' && p.dir==-1){
                p.active=false;
                points++;

                int alienX = round((p.y - alien_offset_x)/ 2.0);
                int alienY = (int)p.x - alien_offset_y;

                if (alienY >= 0 && alienY < 6 && alienX >= 0 && alienX < 11)
                    alien[alienY][alienX]=false;

            }else if (player_x==p.x && player_y==p.y && p.dir==1){
                end_game(false);
            }else if (p.dir==-1) board[(int)p.x][p.y]='|';
            else  board[(int)p.x][p.y]='$';
        }
}
}

void draw_board(){
    gotoxy(0, 0);

    for(int i=0; i<board_x; i++){
        for(int j=0; j<board_y; j++){
            if(i==player_x && j==player_y)std::cout<<'^';
            else std::cout<<board[i][j];
            
        }
        std::cout<<std::endl;
    }
};

void attack(){
    projectiles.push_back(Projectile(player_x, player_y, -1));
}

void input(){
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastShotTime).count();

    if (GetAsyncKeyState('A') & 0x8000) 
        if(player_y>1) player_y--;
    if (GetAsyncKeyState('D') & 0x8000) 
                if(player_y<board_y-2) player_y++;
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && elapsed > 350){
        attack();
        lastShotTime = now;
    }
};

void end_game(bool win_or_lose){
    char choice;
    alive=false;
    draw_board();

    win_or_lose ? std::cout<<"Wygrałeś!!\n" : std::cout<<"Przegrałeś.\n";

    std::cout<<"Zdobyłeś "<<points<<"pkt!\n";
    std::cout<<"Jeszcze raz? (y/n)\n";

    while (_kbhit()) _getch();

    std::cin>>choice;
    choice=='n' ? notDead=false : system("cls");
}

void update_position(){
    init_board();
    if(alienDirection == RIGHT) alien_offset_x+=0.1;
    else if(alienDirection == LEFT) alien_offset_x-=0.1;

    int n=0;
    int o=INT_MAX;

    for(int i=0; i<6; i++){
        for(int j=0; j<11; j++){
            if(alien[i][j]==true){
                if(j>n) n=j;
                if(j<o) o=j;
            }
        }
    }

    if(alien_offset_x+n>33+10-n && alienDirection != LEFT){
        alien_offset_y++;
        alienDirection = LEFT;
    }
    else if(alien_offset_x+o<2 && alienDirection != RIGHT){
        alien_offset_y++;
        alienDirection = RIGHT;
    }

    for(auto& p: projectiles){
        if(p.active)
        p.x += p.dir;
    }
};

void check_collision(){
    projectiles.erase(
    std::remove_if(projectiles.begin(), projectiles.end(),
                   [](Projectile& p){ return !p.active || p.x < 1; }),
    projectiles.end()
    );

    int n=0;

    for(int i=0; i<6; i++){
        for(int j=0; j<11; j++){
            if(alien[i][j]==true){
                n=i;
                break;
            }
        }
    }
    if(alien_offset_y+n == player_x) end_game(false);
};

void check_win(){

    for(int i=0; i<6; i++){
        for(int j=0; j<11; j++){
            if(alien[i][j]==true){
                return;
            }
        }
    }

    end_game(true);

    return;
};

void init(){
    alive = true;
    alienDirection = RIGHT;
    
    points = 0;

    player_x = board_x-2;
    player_y = board_y/2;

    alien_offset_x = 2;
    alien_offset_y = 2;

    projectiles.clear();
    init_board();
    init_alien();

    lastShotTime = std::chrono::steady_clock::now();
}

int main(){
    notDead=true;
    while(notDead){
        init();
        
        while(alive){
            render_aliens();
            render_projectiles();
            input();
            draw_board();
            update_position();
            check_collision();
            check_win();

            Sleep(30);
        }
    }
    return 0;
}