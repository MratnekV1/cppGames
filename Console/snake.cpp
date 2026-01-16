#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cctype>
#include <array>
#include <string>
using namespace std;

bool alive;
int gameMode;
int sleepTime;

const int width = 30; 
const int height = 15;

int pos_x, pos_y, tail_x[width*height], tail_y[width*height];
int fruit_x[5], fruit_y[5], tail_length, score;
string fruitAmmount = "1";

enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
Direction direction;

bool gamesMixed[4] = {false};
vector<string> onOffNames = { "\033[38;5;196mOFF\033[0m", "\033[38;5;46mON\033[0m" };

vector<pair<int, string>> games = {
    {0, "\033[38;5;245mSettings\033[0m"},
    {1, "\033[38;5;46mClassic Mode\033[0m"},
    {2, "\033[38;5;82mWarp Mode\033[0m"},
    {3, "\033[38;5;214mInfinity Mode\033[0m"},
    {4, "\033[38;5;202mSpeed Mode\033[0m"},
    {5, "\033[38;5;196mReverse Mode\033[0m"}
};

vector<pair<int, string>> mixedModesStatus = {
    {0, "return"},
    {1, "Immortal"},
    {2, "Reversed"},
    {3, "Speed"},
    {5, "Warp"},
};

vector<pair<int, string>> settingsOptions = {
    {0, "\033[38;5;245mReturn\033[0m"},
    {1, "\033[38;5;46mSnake Color\033[0m"},
    {2, "\033[38;5;196mFruit Ammount\033[0m"},
    {3, "\033[38;5;129mMix Mode\033[0m"},
};

vector<pair<int, string>> colorOptions = {
    {0, "\033[38;5;245mReturn\033[0m"},
    {1, "\033[38;5;46mGreen\033[0m"},
    {2, "\033[38;5;226mYellow\033[0m"},
    {3, "\033[38;5;39mBlue\033[0m"},
    {4, "\033[38;5;201mMagenta\033[0m"},
    {5, "\033[38;5;51mAqua\033[0m"},
    {6, "\033[38;5;15mWhite\033[0m"},
    {7, "\033[38;5;196mRed\033[0m"},
};

std::array<std::array<std::string, 4>, 7> snakeColors {{
    { "\033[38;5;28m", "\033[38;5;34m", "\033[38;5;40m", "\033[38;5;46m" },    // zielony (jasnozielony, ciemnozielony, zielony, jasnociemnozielony)
    { "\033[38;5;226m", "\033[38;5;220m", "\033[38;5;214m", "\033[38;5;208m" },   // żółty
    { "\033[38;5;31m", "\033[38;5;37m", "\033[38;5;43m", "\033[38;5;50m" },    // niebieski
    { "\033[38;5;201m", "\033[38;5;200m", "\033[38;5;199m", "\033[38;5;198m" },   // magenta
    { "\033[38;5;51m",  "\033[38;5;50m",  "\033[38;5;49m",  "\033[38;5;48m" },    // turkusowy
    { "\033[38;5;231m", "\033[38;5;255m", "\033[38;5;250m", "\033[38;5;245m" },    // biały
    { "\033[38;5;196m", "\033[38;5;160m", "\033[38;5;124m", "\033[38;5;88m" }    // czerwony
}};

int snakeColor = 0;

bool isFruitPositionInvalid(int fruitX, int fruitY);
bool isFruitOverlappingOtherFruits(int index, int fruitX, int fruitY);

int settings(int y){
    int settingsChange;
    int x = 1;
    char sChange;
    bool changingSettings = true;
    do{
        system("cls");
        if (changingSettings && x--<1) cout<<"\033[31mPodaj poprawny numer ustawienia.\033[0m"<<endl;
        cout<<"Ustawienia:\n1." << settingsOptions[1].second << "\n2." << settingsOptions[2].second 
        << "\n3."<< settingsOptions[3].second << "\n0."<< settingsOptions[0].second<< endl;
        cin>>sChange;
        settingsChange = isdigit(sChange) ? sChange - '0' : -1;

        if (isdigit(sChange)){
            for(int i=0; i < (int)settingsOptions.size(); i++){
                if(settingsChange == settingsOptions[i].first){
                    changingSettings = false;
                    break;
                }
            }
        }
    }while(changingSettings);

    if (settingsChange == settingsOptions[0].first){
        gameMode = -1;
        return y=2;
    }

    if (settingsChange == settingsOptions[1].first){
        changingSettings = true;
        char cChoice;
        x=1;
        while(changingSettings){
            system("cls");
            if ((snakeColor != 1 || snakeColor != 2 || snakeColor != 3 || snakeColor != 4 || snakeColor != 5|| snakeColor != 6|| snakeColor != 7) && x<1) 
                cout<<"\033[31mPodaj poprawny numer.\033[0m"<<endl;
            cout<<"Wybierz kolor: \n1." << colorOptions[1].second << "\n2." << colorOptions[2].second 
            << "\n3."<< colorOptions[3].second << "\n4."<< colorOptions[4].second << "\n5."<< colorOptions[5].second 
            << "\n6."<< colorOptions[6].second  << "\n7."<< colorOptions[7].second << "\n0."<< colorOptions[0].second<< endl;
            cin>>cChoice;
            cChoice = isdigit(cChoice) ? cChoice - '0' : -1;
            if (cChoice == 0) changingSettings = false;
            else if (cChoice == 1 || cChoice == 2 || cChoice == 3 || cChoice == 4 || cChoice == 5 || cChoice == 6 || cChoice == 7) {
                snakeColor = cChoice - 1;
                changingSettings = false;
            }
            x--;
        }
        settings(y);
    }

    if (settingsChange == settingsOptions[2].first){
        changingSettings = true;
        char fChoice;
        x=1;
        while(changingSettings){
            system("cls");
            if ((fruitAmmount != "1" || fruitAmmount != "2" || fruitAmmount != "3" || fruitAmmount != "4" || fruitAmmount != "5") && x<1) 
                cout<<"\033[31mPodaj poprawny numer.\033[0m"<<endl;
            cout<<"Wybierz liczbe \033[38;5;196mowocow\033[0m (1-5). \033[38;5;245m0. Return\033[0m"<<endl;
            cin>>fChoice;
            if (fChoice == '0') changingSettings = false;
            else if (fChoice == '1' || fChoice == '2' || fChoice == '3' || fChoice == '4' || fChoice == '5') {
                fruitAmmount = fChoice;
                changingSettings = false;
            }
            x--;
        }
        settings(y);
    }

    if (settingsChange == settingsOptions[3].first){
        changingSettings = true;
        x=1;
        while(changingSettings){
            char mChoice;
            system("cls");
            if ((mChoice != 1 || mChoice != 2 || mChoice != 3 || mChoice != 4) && x<1) 
                cout<<"\033[31mPodaj poprawny numer.\033[0m"<<endl;

            cout<<"1. " << mixedModesStatus[1].second <<": "<< onOffNames[gamesMixed[0]] <<"\n2. " << mixedModesStatus[2].second <<": "<< onOffNames[gamesMixed[1]]
            <<"\n3. " << mixedModesStatus[3].second <<": "<< onOffNames[gamesMixed[2]] <<"\n4. " << mixedModesStatus[4].second <<": "<< onOffNames[gamesMixed[3]]
            << "\n0. \033[38;5;245mReturn\033[0m" << endl;

            cin>>mChoice;
            mChoice = isdigit(mChoice) ? mChoice - '0' : -1;
            if (mChoice == 0) changingSettings = false;
            else if (mChoice == 1 || mChoice == 2 || mChoice == 3 || mChoice == 4) {
                gamesMixed[mChoice-1] = !gamesMixed[mChoice-1];
                x=2;
            }
            x--;
        }
        settings(y);
    }

    return y=2;
}

void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setup(){
    alive = true;
    direction = STOP;
    pos_x = width / 2;
    pos_y = height / 2;
    score = 0;
    gameMode = 0;
    sleepTime = 50;
    tail_length = 2;

    for (int i = 0; i < width*height; i++) {
        tail_x[i] = -500;
        tail_y[i] = -500;
    }

    for(int i=0; i<5; i++){
        do{
            fruit_x[i] = rand() % (width - 2) + 1;
            fruit_y[i] = rand() % height;
        } while (isFruitPositionInvalid(fruit_x[i], fruit_y[i]) || isFruitOverlappingOtherFruits(i, fruit_x[i], fruit_y[i]));
    }
}

bool isFruitPositionInvalid(int fruitX, int fruitY) {
    for (int i = 0; i < tail_length; i++) {
        if (tail_x[i] == fruitX && tail_y[i] == fruitY) {
            return true;
        }
    }

    return false;
}

bool isFruitOverlappingOtherFruits(int index, int fruitX, int fruitY) {
    int fruitCount = stoi(fruitAmmount);
    for (int j = 0; j < fruitCount; j++) {
        if (j == index) continue;
        if (fruit_x[j] == fruitX && fruit_y[j] == fruitY) {
            return true;
        }
    }
    return false;
}

void draw(){
    gotoxy(0, 0);
    // cout<<pos_x<<' '<<pos_y<<' '<<width<<' '<<height<<endl;
    cout<<"TRYB GRY: "<<games[gameMode].second<<endl;

    for (int i = 0; i < width+2; i++) alive ? cout<<'#' : cout<<"\033[38;5;196m" << '#' << "\033[0m";
    cout<<endl;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width+1; j++){
            if (j==0 || j == width) alive ? cout<<'#' : cout<<"\033[38;5;196m" << '#' << "\033[0m";
            
            if (i==pos_y && j==pos_x) cout<< snakeColors[snakeColor][3] << '='  << "\033[0m";
            else if (tail_x[0] == j && tail_y[0] == i){
                cout << snakeColors[snakeColor][2] << '^' << "\033[0m";
            }else{
            	bool printed = false;
                for(int k=0; k<stoi(fruitAmmount); k++) 
                    if (i==fruit_y[k] && j==fruit_x[k]){
                        cout<<"\033[38;5;196m" << '@' << "\033[0m";
                        printed = true;
                        break;
                    }
                for (int k = 0; k < tail_length; k++){
                    if (tail_x[k] == j && tail_y[k] == i){
                        if (k % 4 == 0) {
                            cout << snakeColors[snakeColor][3] << 'o' << "\033[0m";
                        }else if (k % 4 == 1) {
                            cout << snakeColors[snakeColor][2] << 'o' << "\033[0m";
                        }else if (k % 4 == 2) {
                            cout << snakeColors[snakeColor][0] << 'o' << "\033[0m";
                        }else {
                            cout << snakeColors[snakeColor][1] << 'o' << "\033[0m";
                        }
                        printed = true;
                        break;
                    }
                }
                if (!printed)
                    cout << ' ';
			}
        }
        cout<<endl;
    }

    for (int i = 0; i < width+2; i++) alive ? cout<<'#' : cout<<"\033[38;5;196m" << '#' << "\033[0m";
    cout<<endl;
    
    cout<<"Wynik to: "<<score<<endl;
}

void input(){
    if (_kbhit()){
        switch (_getch()){
            case 'a':
            case 75:
                if (direction != RIGHT) direction = LEFT;
                break;
            case 'd':
            case 77:
                if (direction != LEFT) direction = RIGHT;
                break;
            case 'w':
            case 72:
                if (direction != DOWN) direction = UP;
                break;
            case 's':
            case 80:
                if (direction != UP) direction = DOWN;
                break;
            case 'r':
                alive = false;
                break;
        }
    }
}

void logic(){
	int prevX = pos_x, prevY = pos_y;
    int prev2X, prev2Y;
    
    if (direction != STOP){
        for (int i = 0; i < tail_length; i++){
            prev2X = tail_x[i];
            prev2Y = tail_y[i];
            tail_x[i] = prevX;
            tail_y[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        
    }
	
    if (gameMode != 5 || !gamesMixed[1])
        switch (direction){
            case LEFT:
                pos_x--;
                break;
            case RIGHT:
                pos_x++;
                break;
            case UP:
                pos_y--;
                break;
            case DOWN:
                pos_y++;
                break;
            case STOP:
                break;
        }
    else
        switch (direction){
            case LEFT:
                pos_x++;
                break;
            case RIGHT:
                pos_x--;
                break;
            case UP:
                pos_y++;
                break;
            case DOWN:
                pos_y--;
                break;
            case STOP:
                break;
        }

    if (gameMode == 2 || gamesMixed[3]){
        if (pos_x > width-2) pos_x = 0;
        if (pos_x < 0) pos_x = width - 2;
        if (pos_y > height-1) pos_y = 0;
        if (pos_y < 0) pos_y = height-1;
    }

    if (gameMode == 3 || gamesMixed[0]){
        if (pos_x > width-2) pos_x--;
        if (pos_x < 0) pos_x++;
        if (pos_y > height-1) pos_y--;
        if (pos_y < 0) pos_y++;
    }

    if (gameMode == 4 || gamesMixed[2]) sleepTime = (int)sleepTime/2;

    if((pos_x > width-2 || pos_x < 0 || pos_y > height-1 || pos_y < 0) && !gamesMixed[0]) {
        alive = false;

    }

    for(int i=0; i<stoi(fruitAmmount); i++){
        if((int)pos_x==fruit_x[i] && pos_y==fruit_y[i]){
            score++;
            tail_length++;
            
                do {
                    fruit_x[i] = rand() % (width - 2) + 1;
                    fruit_y[i] = rand() % height;
                } while (isFruitPositionInvalid(fruit_x[i], fruit_y[i]) || isFruitOverlappingOtherFruits(i, fruit_x[i], fruit_y[i]));
        }
    }
	
    if (gameMode != 3 && !gamesMixed[0])
        for (int i = 0; i < tail_length; i++){
            if (tail_x[i] == pos_x && tail_y[i] == (int)pos_y){
                alive = false;
                
            }
        }
}

int main(){
    srand(time(0));
    char playAgain;
    do{
        setup();
        bool amI = true;
        int y=1;
        while (amI){
            system("cls");
            if (amI && y<1) cout<<"\033[31mPodaj poprawny tryb.\033[0m"<<endl;
            char gMode;
            cout<<"Wybierz tryb gry!\n1." << games[1].second << " 2." << games[2].second << "\n3."
            << games[3].second << " 4." << games[4].second << "\n5." << games[5].second<< " 0." << games[0].second<<endl;
            cin>>gMode;
            gameMode = isdigit(gMode) ? gMode - '0' : -1;

            if (isdigit(gMode)){
                for(int i=0; i < (int)games.size(); i++){
                    if (gameMode == games[0].first) y = settings(y);
                    else if(gameMode == games[i].first){
                        amI = false;
                        break;
                    }
                }
            }
            y--;
        }

        while (alive){
            input();
            logic();
            draw();
            Sleep(sleepTime);
        }
        
        Sleep(1000);
        system("cls");
        cout<<"\033[38;5;196m" << "Game Over!" << "\033[0m" <<endl;
        cout<<"Ostateczny wynik to: \033[38;5;220m"<<score<< "\033[0m"<<endl;
        cout<<"Play again?"<<endl;
        cout<<"(Y/N)";
        cin>>playAgain;
        if (playAgain == 'Y' || playAgain == 'y') alive = true;
    }while (alive);
    
    return 0;
}
