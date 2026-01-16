#include <iostream>
#include <algorithm>
#include <string>

void init_board(char board[3][3]){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            board[i][j] = ' ';
        }
    }
}

void draw_board(char board[3][3]){
    int n=1;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            std::cout<<' ';

            if(board[i][j]==' ') std::cout<<n;
            else std::cout<<board[i][j];
                
            std::cout<<' ';
            if(j<2)std::cout<<'|';
            n++;
        }

        std::cout<<std::endl;

        if(i<2)
            std::cout<<"---+---+---"<<std::endl;
    }
}

bool get_input(char board[3][3], char player){
    std::string choice;

    std::getline(std::cin, choice);
    for(int i=1; i<=9; i++){
        if(choice== std::to_string(i)){
            int x=(i-1)/3,y=(i-1)%3;
            if(board[x][y]==' '){
                board[x][y]=player;
                return true;
            }
        }
    }

    return false;
}

int is_winner(char board[3][3]){
    for(int i=0; i<3; i++){
        if(board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0]!=' ')
            return 1;
        else if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i]!=' ')
            return 1;
    }
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0]!=' ')
            return 1;
    else if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2]!=' ')
        return 1;

    return 0;
}

int is_draw(char board[3][3]){
    int empty=0;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j]==' ') empty++;
        }   
    }

    return empty;
}

bool handle_lost_draw(int isWin, char player){
    if(isWin==1){
        std::cout<<"Wygrywa "<<player<<std::endl;
        std::cout<<"Jeszcze raz?"<<std::endl<<"y/n"<<std::endl;

        return false;
    }else if(isWin==3){
        std::cout<<"Brak ruchów - remis"<<std::endl;
        std::cout<<"Jeszcze raz?"<<std::endl<<"y/n"<<std::endl;

        return false;
    }

    return true;
}

bool check_result(bool correct, char board[3][3], char player, bool& notDead){
    int isWin=0;
    bool alive=true;
    if(correct){
        isWin = is_winner(board);
        int empty = is_draw(board);

        if(empty==0 && isWin!=1) isWin=3;
    }


    system("cls");
    draw_board(board);
    alive = handle_lost_draw(isWin, player)    ;

    if(!alive){
        char again;
        std::cin>>again;
        if(again!='y') notDead=false;
    }

    return alive;

}

void change_player(char& player, bool& correct){
    if(correct){
        if(player=='X') player='O';
        else player='X';
    }
}

int main(){
    bool notDead=true;
    while(notDead){
        bool alive=true, correct=false;
        char board[3][3];
        char player='X';
        init_board(board);

        while(alive){
            system("cls");
            draw_board(board);
            correct = get_input(board, player);
            alive = check_result(correct, board, player, notDead);
            change_player(player, correct);
        }
    }

    return 0;
}