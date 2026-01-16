#include <iostream>
#include <vector>

char board[15][15];
char display[15][15];

bool correctInput=true;
bool notDead = true;
bool alive;

char again;

int board_x = 2;
int board_y = 2;
int flags = (board_x*board_y)/4;

void init_display(){
    for(int i=0; i<board_x; i++)
        for(int j=0; j<board_y; j++)
            display[i][j]='#';
}

void init_board(){
    int bombs = flags;

    for(int i=0; i<board_x; i++)
        for(int j=0; j<board_y; j++)
            board[i][j]=' ';
    

    while(bombs>0){
        int x = rand()%board_x;
        int y = rand()%board_y;
        if(board[x][y] != 'x'){
            board[x][y] = 'x';
            bombs--;
        }
    }

    for(int i=0; i<board_x; i++)
        for(int j=0; j<board_y; j++){
            if(board[i][j]=='x') continue;

            int count = 0;
            for(int dx=-1; dx<=1; dx++){
                for(int dy=-1; dy<=1; dy++){
                    int ni = i+dx, nj = j+dy;
                    if(ni>=0 && ni<board_x && nj>=0 && nj<board_y && board[ni][nj]=='x'){
                        count++;
                    }
                }
            }
            if(count > 0)
                board[i][j] = '0' + count;
        }
}

void draw_display(){
    system("cls");
    int n=1;

    std::cout<<"Wzór: r / f - x - y"<<std::endl;
    std::cout<<"r - pokaż"<<std::endl;
    std::cout<<"f - flaga"<<std::endl;
    std::cout<<"x, y - pozycja"<<std::endl<<std::endl;

    std::cout<<"0  ";

    // Liczby góra
    for(int j=0; j<board_y*2+1; j++){
        if(j<board_y)std::cout<<j+1<<' ';
        else if(j>board_y)std::cout<<"- ";
        else std::cout<<std::endl<<"   ";
    }

    std::cout<<std::endl;

    for(int i=0; i<board_x; i++){
        std::cout<<n++<<" |";

        for(int j=0; j<board_y; j++){
            std::cout<<display[i][j]<<' ';
        }

        std::cout<<std::endl;
    }

    if(!correctInput) std::cout<<"Podaj poprawny wzór"<<std::endl;
}

void get_input(char& c, int& x, int& y){
    std::cin>>c>>x>>y;
    x--, y--;

    if ((c!='r' && c!='f') || x<0 || x>=board_x || y<0 || y>=board_y) {
        correctInput = false;
        return;
    }

    if (c=='r' && display[x][y]!='#' && display[x][y]!='F') {
        correctInput = false;
        return;
    }

    correctInput = true;
}

void play_again(std::string win_lose){
    alive=false;
    
    draw_display();
    std::cout<<win_lose<<"\nJeszcze raz? y/n \n";
    std::cin>>again;
    if(again!='y') notDead = false;
}

void reveal_empty(int x, int y){
    if(x<0 || y<0 || x>=board_x || y>=board_y) return;
    if(display[x][y] != '#') return;

    display[x][y]= board[x][y];
    if(board[x][y]==' ')
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++)
                if(i!=0 || j!=0) reveal_empty(x+i, y+j);
}

void reveal(char c, int x, int y){
    if(c=='r'){
        if(board[x][y]=='x'){
            display[x][y]='x';
            play_again("Przegrałeś!");
        } else {
            reveal_empty(x, y);
        }
    }else if(display[x][y]=='#') display[x][y]='F';
    else if(display[x][y] == 'F') display[x][y] = '#';

}

void check_win(){
    for(int i=0; i<board_x; i++){
        for(int j=0; j<board_y; j++){
            if(board[i][j]!='x' && display[i][j]=='#')return;
        }
    }

    play_again("Wygrałeś!!!");
}

void get_and_reveal(){
    char c='*';
    int x=0, y=0;

    get_input(c,x,y);
    reveal(c,x,y);
    check_win();
}


int main(){
    srand(time(0));

    while(notDead){
        alive = true;
        init_display();
        init_board();

        while(alive){
            draw_display();
            get_and_reveal();
        }
    };

    return 0;
}