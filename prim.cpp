//Programa para encontrar uma MST de um grafo
//A MST foi feita com base no algoritmo de prim desta implementação: https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/
//Adaptado para leitura de arquivo e para aceitar a entrada de grafo genérico;
//No caso da atividade a MST do grafo apresentado utilizando o algoritmo de prim possui grau máximo 3,
//porém somente o algoritmo de prim não garante uma restrição de grau para qualquer arvore
//adicionar tal resdtrição no algoritmo o torna um NP-difícil.  

#include <iostream>
#include <fstream>
#include <climits>

using namespace std;
ifstream file;


//Gera matriz genérica preenchida com zero:
int **geraMatriz(int l, int c){
    int **matriz;
    matriz = new int * [l];
    for (int i=0; i<l;i++){
        matriz[i] = new int [c];
    }
    for (int i=0; i<l;i++){
        for(int j =0; j<c;j++){
            matriz[i][j] = 0 ; 
        }
    }
    return matriz;
}

//------------------------------------------------------------------------------------------------
//Gera uma matriz com as ligações dos vértices de acordo com o arquivo:
int **ligacao(int aresta){
    int ** liga;
    string texto;
    liga = geraMatriz(aresta,2);
    for(int i = 0; i<aresta;i++){
        getline(file, texto);
        liga[i][0] = stoi(texto.substr(0, texto.find(";")));
        liga[i][1] = stoi(texto.substr(texto.find(";")+1, texto.size()-texto.find(";")));
    }

    cout<<"\n-----------------ligacoes--------------\t\n";
    for (int i=0; i<aresta;i++){
        for(int j =0; j<2; j++){
            cout<<liga[i][j]<<"\t";
        }cout<<endl;
    }

    return liga; 

}
//------------------------------------------------------------------------------------------------
//Gera um vetor com os pesos das arestas de acordo com o arquivo:

int * vPeso(int aresta){
	int*pesos;
	pesos = new int[aresta];
	fstream arquivo;
	arquivo.open("grafos.txt");
	string texto;

	getline(arquivo, texto);
	getline(arquivo, texto);
	for(int i = 0; i<aresta;i++){
    	getline(arquivo, texto);
        pesos[i] = stoi(texto.substr(texto.find(";")+4, texto.size()));

    }
    arquivo.close();
	cout<<"\n-----------------Pesos--------------\t\n";
    for (int i=0; i<aresta;i++){
            cout<<pesos[i]<<"\t";
        }cout<<endl;
    return pesos;

}
//cria uma matriz de adjacencia ponderada de acordo com a matriz de ligações:
int **matrizAdj(int **ligacao,int*pesos, int aresta, int vertice){
    int **matrizAdj = geraMatriz(vertice, vertice);

    for(int i=0; i<aresta; i++){
            matrizAdj[ligacao[i][0]-1][ligacao[i][1]-1] = pesos[i];
            matrizAdj[ligacao[i][1]-1][ligacao[i][0]-1] = pesos[i];
        }
    cout<<"\n-----------------Matriz de Adjacencia Ponderada--------------\t\n";
    for (int i=0; i<vertice;i++){
        for(int j =0; j<vertice; j++){
            cout<<matrizAdj[i][j]<<"\t";
        }cout<<endl;
    }
    return matrizAdj;
}


//Função que imprime o resultado do algoritmo de prim, considerando que o rótulo dos vértices começam de 1;
void imprimeAgm(int *pai, int **mAdj, int vertice){

    cout << "\n"<<"Aresta \tPeso\n";

    for(int i = 1; i < vertice; i++){

        cout << pai[i]+1 << " - " << i+1 << "\t" << mAdj[i][pai[i]] << endl;
    }
}

int chaveMinima(int *chave, bool *agmSet, int vertice){

    int min = INT_MAX;
    int indiceMin;

    for(int i = 0; i < vertice; i++){

        if(agmSet[i] == false && chave[i] < min){
            min = chave[i];
            indiceMin = i;
        }
    }

    return indiceMin;
}

void primAgm(int **mAdj, int vertice){

    int *pai = new int[vertice];

    int *chave = new int[vertice];

    bool *agmSet = new bool[vertice];

    for(int i = 0; i < vertice; i++){
        chave[i] = INT_MAX;
        agmSet[i] = false;
    }

    chave[0] = 0;
    pai[0] = 0;

    for(int i = 0; i < vertice - 1; i++){
        int u = chaveMinima(chave, agmSet, vertice);
        agmSet[u] = true;



        for(int j = 0; j < vertice; j++){

            if(mAdj[u][j] && agmSet[j] == false && mAdj[u][j] < chave[j]){
                pai[j] = u;
                chave[j] = mAdj[u][j];
            }
        }
    }

    imprimeAgm(pai, mAdj, vertice);


}

int main(){
    int vertice;
    int aresta;
    int **mAdj;
    int **mInc;
    int *vetorGrau;
    int **ligacoes;
	int *pesos;
    string texto;

    //leitura de arquivo:
    file.open("grafos.txt");
    if(!file.is_open()){
        cout<<"Não foi possivel abrir o arquivo"<<endl;
        exit(0);
    }else{
        getline(file, texto);
        vertice = stoi(texto);
        getline(file, texto);
        aresta = stoi(texto);
        ligacoes = ligacao(aresta);
        file.close();
    }
	pesos = vPeso(aresta);
    mAdj = matrizAdj(ligacoes, pesos, aresta, vertice);

    primAgm(mAdj, vertice);

    return 0;
}