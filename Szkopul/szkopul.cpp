#include <iostream>
#include <vector>
int addNext(std::vector<long long> pizze, long long currentIndex){
    return pizze[currentIndex+1];
}

int main() {
    std::ios_base::sync_with_stdio(0); std::cin.tie(0);

    int iloscPizz, minPizz;
    
    std::cin>>iloscPizz>>minPizz;
    
    std::vector<long long> pizze;
    while(iloscPizz--){
        long long x;
        std::cin>>x;
        pizze.push_back(x);
    }

    long long maxSmak=-2e18;

    for(int i=0; i<(int)pizze.size(); i++){
        long long aktualnaSuma = 0;

        for(int j=i; j<(int)pizze.size(); j++){
            aktualnaSuma += pizze[j];
            
            int dlugoscFramentu = j-i + 1;

            if(dlugoscFramentu >= minPizz)
                if(aktualnaSuma > maxSmak) 
                    maxSmak = aktualnaSuma;
        }
    }

    std::cout<<maxSmak;
    

    return 0;
}