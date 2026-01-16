#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>

class Character{
    public:
        int health;
        int maxHealth=100;
        
        void takeDamage(int damage){
            health-=damage;
        }
        void heal(int ammount){
            if(health<maxHealth){
                int healAmmount = std::min(ammount, maxHealth-health);
                health+=healAmmount;
                std::cout<<"Uleczono o "<<healAmmount<<" punktów zdrowia\n";
            }else{
                std::cout<<"Nie uleczono - osiągnięto maksymalne HP\n";
            }
        }

        bool isAlive(){
            return health>0;
        }
};

class Player: public Character{
    public:
        std::vector <std::string> Inventory;
        void attack(Character& target, int damage){
            target.takeDamage(damage);
            std::cout<<"Zadałeś "<<damage<<" DMG!\n";
        }
};

class Enemy: public Character{
    public:
        std::string name;

        void attack(Character& target, int damage){
            target.takeDamage(damage);
            std::cout<<name<<" zadał "<<damage<<" DMG!\n";
        }
};

class Game{
    public:
        bool running = true;


        void fight(Player& player, Enemy& enemy){
            int damage = 0;
            int heal = 0;

            std::cout<<"\n--- WALKA --- \n";
            std::cout<<"Twoje HP: "<<player.health<<"\n";
            std::cout<<"Goblin HP: "<<enemy.health<<"\n";

            std::cout<<"Wybierz akcję (1: Atakuj, 2: Leczenie, 0: Wyjście): \n";

            int choice;
            if(!(std::cin>>choice)){
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout<<"Niepoprawny wybór. Spróbuj ponownie. \n";
                return;
            }

            switch (choice){
                case 1:
                    damage = rand() % 15 + 5;
                    player.attack(enemy, damage);
                    break;
                case 2:
                    heal = rand() % 10 + 1;
                    player.heal(heal);
                    break;
                case 0:
                    running = false;
                    return;
                    break;
            }

            if(enemy.isAlive()){
                damage = rand() % 7;
                enemy.attack(player, damage);
            }else{
                std::cout<<"\nZabiłeś "<<enemy.name<<" Brawo!\n";
                running = false;
                return;
            }

            if(!player.isAlive()){
                std::cout<<"\nZdechłeś. Game Over.\n";
                running = false;

            }
        }
};

int main(){
    srand(time(0));

    Game game;
    Player player;
    Enemy goblin;

    player.health = 100;
    goblin.health = 50;
    goblin.name = "Goblin";

    while(game.running){
        game.fight(player, goblin);
    }

    return 0;
}