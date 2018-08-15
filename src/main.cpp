#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
#include <stdexcept> 
#include <sstream>
#include "City.h"
#include "Map.h"
#include "Tour.h"
#include "Population.h"
#include "ImportData.h"
#include "ExportData.h"

/*
using faz com que cout possa ser referenciado apenas como cout
Todas as classes e objetos globais da biblioteca padrão da linguagem C++ estão declarados dentro do namespace std.
Assim, para referenciarmos uma classe ou um objeto da biblioteca padrão C++ precisamos especificar que pertencem ao namespace std.
*/

using std::cout;//cout = exibe mensagem na tela
using std::endl; //end1 = \n
using std::cin; //cin =  scanf
using std::stoi; //Convert string to integer
using std::invalid_argument; //Para tratamento de excessão

typedef std::chrono::high_resolution_clock Clock;

const long int LIM_UNCHANGED{5000};
const long int LIMIT_ALL{50000};

//Protótipos de Funções
void start();
bool end(Population&,long int&);

//Variáveis Globais?
string nome{""};//Arquivo TSP
int popSize{0};//Tamanho da População

int main(int argc, char *argv[]){
    if(argc == 2 || argc > 3){
        cout<<"numero de parametros incorreto"<<endl;
        return(0);
    }else if(argc == 3){
        try{
            nome = argv[1];
            popSize = stoi(argv[2]);
        }catch(invalid_argument &i_a){
            cout<<"Numero de cidades invalido! "<<i_a.what()<<endl;
            return(0);
        }
    }else{
        cout<<"----------------------------TSP----------------------------"<<endl;
        cout<<"Digite o nome do arquivo .tsp sem a extensao: ";
        cin>>nome;
        cout<<"Digite o tamanho de sua populacao: ";
        cin>>popSize;
    }
    start();//Começa a Rodar o programa
    return(0);
}



void start(){
    long int i{0},mapSize{0},endCond{0};
    
    srand(time(NULL));//Gerador Randômico

    auto tInicial=Clock::now();  //startando cronometro, a variável 'automaticamente' vai ter seu tipo definido
    
        //Importa os Arquivos
        cout<<"Comecando o algoritmo ..."<<endl;
        ImportData dataFile(nome);
        ExportData writeLog(nome);
        dataFile.printInfos();
    
        Map map(dataFile.getCitiesCoord()); //Cria variável do tipo mapa
    
        for(City c : map.getCityList()){ //Mostra lista de cidade, x e y
            cout<<c<<endl;
        }
    
        cout<<"comecando a processar..."<<endl;
    
        Population *pop = new Population(map,popSize);
    
        cout<<"Primeira populacao: "<<endl;
        cout<<(*pop)<<endl;
        std::ostringstream input;
        input<<dataFile.getInfos()<<endl;
        writeLog.writeFile(input);
        input.str("");
        input.clear();
        input << "Primeira população:" <<endl;
        writeLog.writeFile(input);
        input.str("");
        input.clear();
        input << (*pop).getPopulation()[(bestFitness((*pop).getPopulation()))] <<endl;
        writeLog.writeFile(input);
        cout<<"Melhor fitness: "<<maxFitness((*pop).getPopulation())<<endl;
        cout<<"distancia: "<<(1/maxFitness((*pop).getPopulation())*10000)<<endl;
        
    
        while(!end(*pop,endCond)){   // Roda enquanto nenhuma das condições de parada forem atendidas ----Chama bool end)
    
            Population *oldPop = pop;
            pop = (*pop).newGeneration();
            delete oldPop;
            ++i;
            if(i%5000==0){
                cout<<"Geracao: "<<i<<" e rodando..."<<endl;
                input.str("");
                input.clear();
                input << "Geração:" <<i<< endl;
                input << (*pop).getPopulation()[(bestFitness((*pop).getPopulation()))] <<endl;
                writeLog.writeFile(input);
            }
        } 
    
        auto tFinal=Clock::now();
    
        
        //cout<<(*pop)<<endl;
        
        if(endCond==1){
            cout<<"População convergiu!"<<endl;
        }else if(endCond==2){
            cout<<"Fitness inalterada com "<<LIMIT_ALL<<" de gerações!"<<endl;
        }
    
        cout<<i<<" geracoes depois!"<<endl;
        cout<<"Fitness maxima: "<<maxFitness((*pop).getPopulation())<<endl;
        cout<<"distancia: "<<(1/maxFitness((*pop).getPopulation())*10000)<<endl;
    
        cout<<"-----------------------------" << endl;
    
        cout<<"Tempo de execucao: "<<std::chrono::duration_cast<std::chrono::seconds>(tFinal - tInicial).count()<<" segundos!"<<endl;
        cout << endl;
        dataFile.printInfos();
       
} 

bool end(Population &pop,long int &eC){
    static long int genWithoutChanges{0};
    static double maxFitEver{0.0};
    double maxFit{maxFitness(pop.getPopulation())}, minFit{minFitness(pop.getPopulation())};

    if(maxFit > maxFitEver){  // contagem de gerações inalteradas
        maxFitEver = maxFit;
        genWithoutChanges = 0;
        cout<<"aumentou a fitness maxima! fitness: "<<maxFitEver<<" distancia: "<<((1/maxFitEver)*10000)<<endl;
    }else{
        genWithoutChanges++;
    }

    if((maxFit-minFit) < (maxFit*0.01)){
        eC=1;
        return(true);
    }else if(genWithoutChanges >= LIM_UNCHANGED){
        if(genWithoutChanges >= LIMIT_ALL){
            eC=2;
            return(true);
        }
        return(false);
    }else{
        return(false);
    }
}