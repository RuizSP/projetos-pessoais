//Algoritmo para encontrar centro, anticentro, mediana, somatório, máximo e minimo de um grafo não direcionado
//O programa utiliza o algoritmo de dijkstra para encontrar o menor caminho entre cada vertice
//O algoritmo de dijkstra foi feito com base nessa implementação: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
//Adapatada para aceitar uma entrada qualquer de forma dinamica e retornar um vetor com a distancia de um vertice de referencia e cada um dos outros vertices 

#include <iostream>
#include <fstream>
#include <climits>

using namespace std;
ifstream file;

void imprimiVet(int * vet, int tamanho){
    for(int i=0;i < tamanho; i++){
        cout<<vet[i]<<" ";
    }cout<<endl;
}



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
//------------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------------
//Encontra o menor caminho entre um vertice de referencia e todos os outros vertices do graffo:
int distanciaMinima(int * dist, bool * sptSet, int vertice){
    int min = INT_MAX, indiceMinimo;
    for(int i =0; i< vertice; i++){
        if(sptSet[i]==false && dist[i]<= min){
            min = dist[i];
            indiceMinimo = i;
        }
    }
    return indiceMinimo;
}
int * dijkstra(int **mAdj, int vReferencia, int vertice ){
    int * dist= new int[vertice];
    bool * sptSet = new bool[vertice];
    for(int i=0; i<vertice; i++){
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
   
    dist[vReferencia] = 0;

    for(int i = 0; i< vertice-1; i++){
        int u = distanciaMinima(dist, sptSet, vertice);
        sptSet[u] = true;
        for (int j=0; j< vertice; j++ ){
            if(!sptSet[j] && mAdj[u][j] && dist[u] != INT_MAX && dist[u]+mAdj[u][j]< dist[j]){
                dist[j] = dist[u] + mAdj[u][j];
            } 
        }
    }
    return dist;
 }

//------------------------------------------------------------------------------------------------
//Gera a matriz de excentricidade 
//Esta função chama a função dijkstra para cada vertice i, tomando-o como vertice de referencia
//Adiciona cada valor do vetor de distancias dist, para uma matriz;   
 int **matrizExc(int ** mAdj, int vertice){
    int ** mExc = geraMatriz(vertice, vertice);
    for (int i =0; i< vertice; i++){
        int * dist = dijkstra(mAdj, i, vertice);
        for (int j =0; j<vertice; j++){
                mExc[i][j] = dist[j];
                mExc[j][i] = dist[j];
        }
    }
    cout<<"\n-----------------Matriz de excentricidade--------------\t\n";
    for (int i=0; i<vertice;i++){
        for(int j =0; j<vertice; j++){
            cout<<mExc[i][j]<<"\t";
        }cout<<endl;
    }
    return mExc;
 }
//------------------------------------------------------------------------------------------------
 //faz o somatório de cada linha da matriz
int * somaLinha(int **mExcentricidade, int n){
    int soma = 0;
    int* somatorio = new int[n];
    for(int i = 0; i<n;i++){
        soma = 0;
        for(int j=0; j< n;j++){
            soma += mExcentricidade[i][j];
        }
            somatorio[i] = soma;
    }
    return somatorio;
}

//encontra o valor maximo de cada linha e salva em um vetor
int * maxLinha(int **mExcentricidade, int n){
    int * listaMax = new int[n];
    for(int i =0;i<n ;i++){
        listaMax[i] = mExcentricidade[i][0];
        for(int j =0;j<n;j++){
            if (mExcentricidade[i][j]>listaMax[i]){
             listaMax[i] = mExcentricidade[i][j];
            }
        
        }
    }
    return listaMax;
}
 //encontra o valor minimo de cada linha e salva em um vetor
int * minLinha(int **mExcentricidade, int *maximoLinha,  int n){
    int * listaMin = new int[n];
    for(int i =0;i<n ;i++){
        listaMin[i] = maximoLinha[i];
        for(int j =0;j<n;j++){
            if (mExcentricidade[i][j]<listaMin[i] && mExcentricidade[i][j] != 0 ){
             listaMin[i] = mExcentricidade[i][j];
            }        
        }
    }
    return listaMin;
}

//encontrar centro:
void centro(int * maximoLinha, int n){
    int minSom = maximoLinha[0];
    for (int i= 0; i<n;i++ ){
        if (maximoLinha[i]<minSom){
            minSom = maximoLinha[i];
        }
    }
    cout<<"\nCentro: ";
    for(int i =0; i<n;i++){
        if(maximoLinha[i]==minSom){
            cout<<i+1<<" ";
        }
    }cout<<endl;
}
//encontrar mediana:
void mediana(int * somatorioLinha, int n){  
    int minMed = somatorioLinha[0];
    for (int i= 0; i<n;i++ ){
        if (somatorioLinha[i]<minMed){
            minMed = somatorioLinha[i];
        }
    }
    cout<<"\nMediana: ";
    for(int i =0; i<n;i++){
        if(somatorioLinha[i]==minMed){
          cout<<i+1<<" ";
        }
    }cout<<endl;
}
//encontrar anticentro:
void anticentro(int *minimoLinha,int n){
    int maxMin = minimoLinha[0]; 
    for (int i= 0; i<n;i++ ){
        if (minimoLinha[i]>maxMin){
            maxMin = minimoLinha[i];
        }
    }
    cout<<"\nAnticentro: ";
    for(int i =0; i<n;i++){
        if(minimoLinha[i]==maxMin){
           cout<< i+1<<" ";
        }
    }cout<<endl;
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
    int ** mExc = matrizExc(mAdj, vertice);

    int * somatorioLinha = somaLinha(mExc, vertice); 
    int * maximoLinha = maxLinha(mExc, vertice);
    int * minimoLinha = minLinha(mExc, maximoLinha,vertice);
   
    cout<<"\nRespostas: "<<endl;

    cout<<"somatorio de cada linha: ";
    imprimiVet(somatorioLinha, vertice);
    cout<<"máximo de cada linha: ";
    imprimiVet(maximoLinha, vertice);
    cout<<"minimo de cada linha: ";
    imprimiVet(minimoLinha, vertice);
    
    
    centro(somatorioLinha, vertice);
    mediana(maximoLinha, vertice);
    anticentro(minimoLinha, vertice);

    return 0;
}
