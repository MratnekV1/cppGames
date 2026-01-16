#include <iostream>
#include <chrono>
#include <thread>

void passed_duration(){
    auto start = std::chrono::steady_clock::now();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout<<"Trwaˆo to: "<<duration.count()<<" ms\n";
}

void timer(){
    std::cin.ignore();
    auto start = std::chrono::steady_clock::now();
    std::cin.ignore();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout<<"Trwaˆo to: "<<duration.count()<<" ms\n";
}

void howFast(){
    std::cout<<"Za chwil© wylosuj© czas... b¥d« got¢w!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rand()%4000));

    auto start = std::chrono::steady_clock::now();

    std::cout<<"Naci˜nij ENTER jak najszybciej!\n";
    std::cin.ignore();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    std::cout<<"Klikn¥ˆe˜ ENTER po "<<duration.count()<<" ms\n";
}

void tickDown(){
    int number;
    std::cout<<"Podaj liczb© do odliczenia.\n";

    std::cin>>number;
    auto start = std::chrono::steady_clock::now();
    std::chrono::seconds last_shown_second(-1);
    while(number>=0){
        auto now = std::chrono::steady_clock::now();
        auto seconds_passed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

        if(seconds_passed.count() != last_shown_second.count()){
            last_shown_second = seconds_passed;
            std::cout<<"Pozostaˆo: "<<number<<"s\n";
            number-=1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));


    }

    std::cout<<"Koniec obliczania!";

}

int main(){
    //passed_duration();
    //timer();
    //howFast();
    tickDown();

    return 0;
}