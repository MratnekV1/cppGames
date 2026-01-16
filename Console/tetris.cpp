#include <iostream>
#include <windows.h>
#include <chrono>

const int grid_x = 10, grid_y = 20;

void gotoxy(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class GameBoard{
    private:
        char grid[grid_y][grid_x];
        static const int shapes[7][4][4][4];

        std::chrono::steady_clock::time_point lastRotation;
        std::chrono::steady_clock::time_point lastFall;

        int current_rotation;
        bool blockSpawned = false;
        int blockType;
        int posX = 3, posY = 2;
        int posY_Bottom = 0;
    public:
        //GameBoard(int x, int y): posX(x), posY(y)  {}

        bool alive = true;

        void init_board(){
            for(int i=0; i<grid_y;i++)
                for(int j=0; j<grid_x;j++){
                    if(i==0 || i==grid_y-1 || j==0 || j==grid_x-1)
                        grid[i][j]='#';
                    else grid[i][j]=' ';

                    //if(i==6) grid[i][j]='#';
                }
        }

        int bottom_row(){
            int n = 0;

            for(int i=3; i>0;i--)
                for(int j=3; j>0;j--)
                    if(shapes[blockType][current_rotation][i][j]){
                        n=i;
                        return n+1;
                    }

            return n+1;
        }

        bool canMove(int newX, int newY, int rotation) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (shapes[blockType][rotation][i][j]) {
                        int x = newX + j;
                        int y = newY + i;
                        if (x < 1 || x >= grid_x - 1) return false; // poza plansz¥ (lewo/prawo)
                        if (y < 0 || y >= grid_y) return false;    // poza plansz¥ (g¢ra/d¢ˆ)
                        if (grid[y][x] == '#') return false;       // kolizja z innym blokiem
                    }
                }
            }
            return true;
        }


        void Collision(){
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    if(shapes[blockType][current_rotation][i][j]){
                        if(grid[posY+i+1][posX+j] == '#' && !shapes[blockType][current_rotation][i+1][j]){
                            blockSpawned = false;
                        }
                    }
                }
            }
        }

        void clearFullLines(int y) {
            for (int i = y; i > 1; i--) {
                for (int j = 1; j < grid_x - 1; j++) {
                    grid[i][j] = grid[i - 1][j];
                }
            }

            for (int j = 1; j < grid_x - 1; j++) {
                grid[1][j] = ' ';
            }
        }

        void fullLines() {
            for (int i = grid_y - 2; i > 0; i--) {
                int n = 0;
                for (int j = 1; j < grid_x - 1; j++) {
                    if (grid[i][j] == '#') n++;
                }

                if (n == grid_x - 2) {
                    clearFullLines(i);
                    i++;
                }
            }
        }

        void printBoard() const{
            gotoxy(0, 0);
            for(int i=0; i<grid_y;i++){
                for(int j=0; j<grid_x;j++){
                    std::cout<<grid[i][j];
                    std::cout<<' ';
                }
                std::cout<<std::endl;
            }
        }

        void spawnBlock(){
                blockSpawned=true;
                posY=0;
                posX= grid_x/3;
                blockType = rand()%7;
                current_rotation = rand()%4;
            
        }

        void drawBlock(){
            if(!blockSpawned)
                spawnBlock();
            
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    if(shapes[blockType][current_rotation][i][j]){
                        grid[posY+i][posX+j] = '#';
                    }
                }
            }
        }

        void moveBlock(){
            if(blockSpawned){
                int oldX = posX, oldY = posY, oldRot = current_rotation;

                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastRotation).count();
                auto moved = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastRotation).count();
                auto roatated = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastRotation).count();
                auto falling = std::chrono::duration_cast<std::chrono::milliseconds>(now-lastFall).count();

                clearGrid(oldX, oldY, oldRot);

                // Falling
                if (falling > 300){
                    if (canMove(posX, posY + 1, current_rotation)) {
                        posY++;
                        lastFall = now;
                    }
                }

                // Rotation
                if (GetAsyncKeyState('Q') & 0x8000 && roatated > 150){
                    int newRot = (current_rotation + 3) % 4;
                    if (canMove(posX, posY, newRot)) {
                        current_rotation = newRot;
                        lastRotation = now;
                    }
                }
                if (GetAsyncKeyState('R') & 0x8000 && roatated > 150){
                    int newRot = (current_rotation + 1) % 4;
                    if (canMove(posX, posY, newRot)) {
                        current_rotation = newRot;
                        lastRotation = now;
                    }
                }

                // Moving
                if (GetAsyncKeyState('D') & 0x8000 && moved > 150){
                    if (canMove(posX + 1, posY, current_rotation)) {
                        posX++;
                    }
                }
                if (GetAsyncKeyState('A') & 0x8000 && moved > 150){
                    if (canMove(posX - 1, posY, current_rotation)) {
                        posX--;
                    }
                }

                // Acceleration
                if ((GetAsyncKeyState('S') & 0x8000) && elapsed > 50){
                    if (canMove(posX, posY + 1, current_rotation)) {
                        posY++;
                        lastRotation = now;
                    }
                }
            }
        }

        void clearGrid(int oldX, int oldY, int oldRot) {
            if (!blockSpawned) return;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (shapes[blockType][oldRot][i][j] && oldY + i>0) {
                        grid[oldY + i][oldX + j] = ' ';
                    }
                }
            }
        }

};

void game(){
    GameBoard gameBoard;
    gameBoard.init_board();
    
    while(gameBoard.alive){
        gameBoard.drawBlock();
        gameBoard.printBoard();
        gameBoard.Collision();
        gameBoard.moveBlock();
        gameBoard.fullLines();

        Sleep(30);
    }
}

int main(){
    srand(time(0));
    bool stillPlaying = true;
    while(stillPlaying)
        game();

    return 0;
}

const int GameBoard::shapes[7][4][4][4] = {
    // I
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
    },
    // O
    {
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        // O jest symetryczne, wszystkie rotacje takie same
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // T
    {
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },
    // S
    {
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
    },
    // Z
    {
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },
    // J
    {
        {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },
    // L
    {
        {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    }
};
