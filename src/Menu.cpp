#include "Menu.h"
#include <bits/stdc++.h>
#define NANHO 8
#include <iostream>
#include "LZW.h"
#include <stdlib.h>
#include <fstream>
#include <vector>

Menu::Menu()
{
    //ctor
}

Menu::~Menu()
{
    //dtor
}

void Menu::leitura(vector<registro>& lista, vector<string>& linha, vector<vector<string>>& linhas,int N){

    fstream file, bgg;
    if(N>17065)
	cout<<"Nanho especificado maior do que o arquivo games_detailed_info.csv"<<endl;
    file.open("games_detailed_info.csv", ios::in);
    bgg.open("bgg-13m-reviews.csv", ios::in);
    //cout<<"abrindo bgg-13m-reviews.csv"<<endl;
    if(bgg.is_open()){
        int num = 0;//numero de linhas no arquivo
        string aux,str,palavra;
        //reinicializando o arquivo
        bgg.clear();
        bgg.seekg(0, ios::beg);
        getline(bgg,str); // pula cabe�alho
        srand(time(NULL));//inicializando semente de randomizacao
        int random = 17065-N; //pegando TAM linhas dentre as 17065 primeiras linhas do arquivo
        random = rand() % random; //sorteia numero aleatorio
        int contN = 0;
	//seta o ponteiro do arquivo na linha sorteada
	//cout<<random<<endl;
        for(int j = 0; j< random; j++){
	    getline(bgg,str);
	}
	while(getline(bgg, str)){
	    stringstream s(str);//joga a string str para uma stringstream
            registro reg; //cria um registro
            getline(s, palavra, ','); //pega a primeira coluna => id
	    reg.setId(stoi(palavra)); //define id como o numero lido do arquivo
            getline(s, palavra, ','); //pega a segunda coluna => user
            reg.setUser(palavra); //define o user como oo lido do arquivo
	    if(palavra[0] == '"'){
		getline(s, aux, '"');
		palavra = palavra+aux;
		getline(s, aux, ',');
	    }
            getline(s, palavra, ','); //pega a proxima coluna => rating
            reg.setRating(stoi(palavra));
            lista.push_back(reg); // jogar o registro para dentro do vetor passado por parametro da funcao
            contN++;
            s.clear();
	    if(contN>=N)
	    	break;
        }
	//cout<<contN<<endl;
    }
    else
	cout<<"Erro ao abrir arquivo bgg-13m-reviews.csv"<<endl;
    //cout<<"abrindo games_detailed_info.csv"<<endl;
    if(file.is_open()){
	string str;
    	vector<registro>::iterator it;
	int cont = 0;
	file.clear();
	file.seekg(0,ios::beg);//joga o ponteiro pro inicio do arquivo
	getline(file, str); // pula o cabecalho
	int i = 0;
	//cout<<lista.size()<<endl;
	for(it = lista.begin(); it != lista.end(); ++it){ //pega apenas as linhas pegas ao ler o bgg
	   while(i<it->getId()){ //caminha ate a primeira linha que o lida pelo bgg
	   	getline(file,str);
		i++;
	   }
	   //cout<<i<<endl;
	   stringstream s(str); //joga o str pro stringstream
	   getline(s, str, ','); //pega a primeira linha => id
	   //cout<<str<<endl;
	   //i = stoi(str);
	   getline(s, str, '"');
	   //cout<<str<<endl;
	   getline(s, str, '"');//pega o comentario
	   //cout<<str<<endl;
	   it->setDescricao(str);//joga pro registro
	   cont++;

       }
    }
    else
	 cout<<"erro ao abrir o arquivo info"<<endl;
    file.close();
    bgg.close();
}



void Menu::exibirMenu()
{


    int escolha = -1;
    ofstream arquivo;

    cout << "Insira o que deseja fazer: " << endl;
    while(escolha != 0)
    {

        cout << "1. Cenario 1" << endl;
        cout << "2. Cenario 2" << endl;
        cout << "3. Cenario 3" << endl;
        cout << "4. Cenario 4" << endl;
        cout << "0 Encerrar o programa" << endl << endl;
        cin >> escolha;
        switch(escolha)
        {
        case 1: ///Cenario 1
        {

            int numeroN = 0; //armazena o n�mero de N's do arquivo
            ifstream infile ("entrada.txt");
            int i=0;

            while(infile)
            {

                if(i==0)
                {
                    string s;
                    getline(infile,s);
                    numeroN = atoi(s.c_str()); //pega o n�mero de N's que est�o no arquivo.
                    break;
                }
            }

            int vetN[numeroN]; //cria um vetor para salvar os N's que s�o quantidades de n�meros que ser�o testados.

            while(infile)
            {

                string s;
                if(i==0)  //nao pega o primeiro elemento, pois ele indica a quantidade de N's.
                {
                }
                else
                {
                    if(!getline(infile,s))
                        break;
                    vetN[i-1]=atoi(s.c_str()); //pega o valor de N e salva no vetor vetN.


                }
                i++;
            }
            infile.close();
                std::ofstream saida ("saida.txt");
                vector<registro> lista;
                vector<string> linha;
                vector<vector<string>> linhas;
                vector<registro>::iterator it;

                double tempoTotal = 0;
                unsigned long int vetTotalBytes[numeroN];
                unsigned long int vetTotalBytesComp[numeroN];
                double vetorTempoTotal[numeroN];
                double vetorRazaoCompressao[numeroN];
                string BytesTexto;
                unsigned long int totalBytes = 0; //total de bytes nao compimidos
                unsigned long int totalBytesComp = 0; //total de bytes comprimidos
                double razaoCompressao = 0;
                std::chrono::time_point<std::chrono::system_clock> start, end;
                string descricao;

               for(int k=0;k<numeroN;k++)
               {



                    leitura(lista,linha, linhas,vetN[k]);
                    for(it = lista.begin(); it != lista.end(); ++it)
                    {

                        descricao=it->getDescricao();
                        if(descricao!="")
                        {

                            LZW *lzw = new LZW(descricao);
                            start = std::chrono::system_clock::now();
                            lzw->codificar();
                            end = std::chrono::system_clock::now();
                            tempoTotal += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
                            totalBytesComp += lzw->getBytesComprimido(); //adiciona o total de bytes do texto apos comprimir
                            razaoCompressao += lzw->getTaxaCompressao(); //soma as razoes de compressao.
                            totalBytes+=descricao.length();

                        }
                    }


                    vetTotalBytes[k] = totalBytes;
                    vetTotalBytesComp[k] = totalBytesComp;
                    vetorRazaoCompressao[k] = razaoCompressao/(float)(vetN[k]);
                    vetorTempoTotal[k] = tempoTotal;

                    totalBytes = 0;
                    totalBytesComp = 0;
                    razaoCompressao = 0;
                    tempoTotal = 0;

               }

                cout<<"----METRICAS LZW --"<<endl;

                saida<<"----METRICAS LZW --"<<endl;
                for(int k=0; k<numeroN ; k++)
                {

                    cout<<endl<<endl;
                    cout<<" N = " << vetN[k] << ": "<<endl;
                    cout<<"*     METRICAS       *            LZW           *" <<endl;
                    cout<<"*     TOTAL BYTES    *"<< vetTotalBytes[k] << "\t\t\t*" <<endl;
                    cout<<"*  TOTAL BYTES COMP  *"<< vetTotalBytesComp[k] <<"\t\t\t*"<<endl;
                    cout<<"*     RAZAO COMP     *"<< vetorRazaoCompressao[k] <<"\t\t\t* "<<endl;
                    cout<<"*     TEMPO TOTAL    *"<< vetorTempoTotal[k] <<" ms \t\t\t* "<<endl;



                    saida<<endl<<endl;
                    saida<<" N = " << vetN[k] << ": "<<endl;
                    saida<<"*     METRICAS       *            LZW           " <<endl;
                    saida<<"*     TOTAL BYTES    *"<< vetTotalBytes[k] << "\t\t\t*" <<endl;
                    saida<<"*  TOTAL BYTES COMP  *"<< vetTotalBytesComp[k] <<"\t\t\t*"<<endl;
                    saida<<"*     RAZAO COMP     *"<< vetorRazaoCompressao[k] <<"\t\t\t* "<<endl;
                    saida<<"*     TEMPO TOTAL    *"<< vetorTempoTotal[k] <<" ms \t\t\t* "<<endl;





                    saida.close();

                }
            break;
        }

        case 2: ///Cenario 2
        {

        }

        case 3: ///Cenario 3
        {

        }
        case 4: ///Cenario 4
        {

        }
        case 0:
            break;
        default:
            cout << endl <<  "Por favor insira um valor correto" << endl;
        }
    }
    arquivo.close();
}









