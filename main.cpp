#include <iostream>

#include <LZW.h>

#include <string>


void codLzw(){
    int numN=2;
    int *vetN=new int[numN]; //cria um vetor para salvar os N's que s�o quantidades de n�meros que ser�o testados.


//******************************************************
    //*                                                    *
    //*                Compressao LZW                      *
    //*                                                    *
    //******************************************************

    /**
        - Leitura do Arquivo

       A leitura de arquivo funciona a partir de um vector de vector chamado linhas que vai guardar todas as linhas do arquivo.
       Existe um vector chamado linha que armazena a informacao de cada coluna do arquivo.
       Assim é feito a leitura e armazenamento nesses vectors, obedecendo os chars delimitadores e enclosures.

    */
    fstream file("bgg-13m-reviews.csv");
    if(!file)
    {
        cout<<"bgg nao encontrado"<<endl;
        exit(0);
    }
    ofstream file2("saidaLZW.txt",ios::out);
    string buffer;
    vector<vector<string> > linhas; //vetor de vetor para toads as linhas

    char delimitador = ',';
    char delimitador_2 = '"'; //Se tem ',' dentro de um " " essa virgula nao significa outra coluna.
    int i=0;
    while(!file.eof())
    {
        getline(file, buffer); //ler cada linha
        stringstream ss(buffer); //colocar a linha lida num stringstream

        vector<string> linha; //iniciar o vetor da linha
        bool flag = false;
        while(getline(ss, buffer)) { //ler cada coluna

            int start = 0;

            for(i = 0; i < buffer.length(); i++) {

                if(buffer[i] == delimitador) {

                    linha.push_back(buffer.substr(start, i-start));

                    start = i+1;
                }

                else if(buffer[i] == delimitador_2)
                {
                    for(i = i+1; i <= buffer.length(); i++)
                    {
                        if(i==buffer.length())
                        {
                            flag = true;
                            linha.push_back(buffer.substr(start, (i-1)-start));
                            break;
                        }

                        if(buffer[i] == delimitador_2 && buffer[i+1] == delimitador)
                        {
                            linha.push_back(buffer.substr(start, i-start+1));
                            i++;
                            start = i+1;
                            break;
                        }
                    }
                    if(i>buffer.length())
                       break;
                }
            }

            if(i - start != 0 && flag ==false)
            {
                    linha.push_back(buffer.substr(start, (i-1)-start));
            }
        }
        linhas.push_back(linha);
    }


    /**
        Finaliza Leitura e Armazenamento
    */


    /**
        - Elaboracao LZW

        Gera um número aleatório de qual registro(linha) será pego e acessa a coluna 9, que é a que está a sinopse.
        Após isso é feita a compressao do texto e armazenado as métricas para comparacao.
        Como LZW é uma forma de compressão mais característica em imagens, a alternativa para o problema encontrado foi
        a compressao de um conjunto de 50 sinopses e após isso a realizaçao de outra compressão com mais 50 sinopses,fazendo
        isso até a quantidade de sinopses máxima ser atingida,o que possibilita a identificaão de mais padroes no dicionario,
        reduzindo a razão de compressão.
    */

    cout<< "------------  Compressao LZW ----------------"<<endl<<endl;
    int numLinhas = linhas.size();

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();    //seed para gerar numeros aleatorios sempre (relogio do computador)
    std::default_random_engine generator(seed);                                         //gerador de numeros aleatorios com a semente
    std::uniform_int_distribution<int> distribution(1,numLinhas);                       //gera um numero aleatorio

    unsigned long int vetTotalBytes[numN]; //Armazena os totais de bytes nao comprimidos
    unsigned long int vetTotalBytesComp[numN];  //Armazena os totais de bytes comprimidos
    double vetTempoTotal[numN];  //Armazena os tempos totais
    double vetRazaoCompressao[numN];

    unsigned long int totalBytes = 0; //total de bytes nao compimidos
    unsigned long int totalBytesComp = 0; //total de bytes comprimidos
    double razaoCompressao = 0;
    double tempoTotal = 0;

    std::chrono::time_point<std::chrono::system_clock> start, end; //inicializa as variaveis para guardar o tempo
    int aleatorio = 0;

    for(int k=0; k<numN ; k++)
    {
        cout<<"N: "<<vetN[k]<<endl<<endl;

        totalBytes = 0; //total de bytes nao compimidos
        totalBytesComp = 0; //total de bytes comprimidos
        razaoCompressao = 0;
        tempoTotal = 0;
        string textoTotal;

        for(int l=1; l<=vetN[k] ; l++)
        {
            aleatorio = distribution(generator); //pega a linha aleatoria
            bool flg = false;

            while(flg == false)
            {
                if(linhas[aleatorio].size() == 24) //Checa qual registro está completo,ou seja, apresenta um numero de colunas igual dos indices, ja que registros incompletos causam crash's
                {

                    if(linhas[aleatorio][32].size() > 3) //Checa se sinopse nao está vazia
                    {
                        flg = true;
                        string texto = linhas[aleatorio][9].substr(1, linhas[aleatorio][9].size() - 2); //mostrar sem as " "

                        if(l%50 == 0)
                        {
                                    cout<<"11111111"<<endl;

                            start = std::chrono::system_clock::now(); //inicia cronometro
                            LZW *lzw = new LZW(textoTotal);
                            lzw->codificar();
                            end = std::chrono::system_clock::now(); //para cronometro
                            tempoTotal += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //calcula tempo decorrido

                            totalBytesComp += lzw->getBytesComprimido(); //adiciona o total de bytes do texto apos comprimir
                            razaoCompressao += lzw->getTaxaCompressao(); //soma as razoes de compressao.

                            textoTotal.erase(0,textoTotal.length());
                        }
                        else
                        {
                            textoTotal+=texto;
                            totalBytes += textoTotal.length(); //adiciona o total de bytes do texto sem comprimir
                        }

                        break;
                    }

                }
                aleatorio = distribution(generator); //pega a linha aleatoria
            }
        }

        cout<<"Total Bytes: "<<totalBytes<<endl;
        cout<<"Total Bytes Comprimido: "<<totalBytesComp<<endl;
        cout<<"Razao de Compressao Media: "<<razaoCompressao/(float)vetN[k]<<endl;
        cout<<"Tempo Total: "<<tempoTotal<<"ms"<<endl;

        vetTotalBytes[k] = totalBytes;
        vetTotalBytesComp[k] = totalBytesComp;
        vetRazaoCompressao[k] = razaoCompressao/(float)(vetN[k]);
        vetTempoTotal[k] = tempoTotal;
        cout<<endl<<"-----------------"<<endl;
    }


    cout<<"----------------- METRICAS LZW ------------------"<<endl;
    file2<<"----------------- METRICAS LZW ------------------"<<endl;
    for(int k=0; k<numN ; k++)
    {

        cout<<endl<<endl;
        cout<<" N = " << vetN[k] << ": "<<endl;

        cout<<"     METRICAS       |            LZW           " <<endl;

        cout<<"|     TOTAL BYTES    |"<< vetTotalBytes[k] << "\t\t\t|" <<endl;

        cout<<"|  TOTAL BYTES COMPRESSAO  |"<< vetTotalBytesComp[k] <<"\t\t\t|"<<endl;

        cout<<"|     RAZAO DA COMPRESSAO     |"<< vetRazaoCompressao[k] <<"\t\t\t| "<<endl;

        cout<<"|     TEMPO TOTAL    |"<< vetTempoTotal[k] <<"  \t\t\t| "<<endl;


         file2<<endl<<endl;
        file2<<" N = " << vetN[k] << ": "<<endl;
        file2<<"|     METRICAS       |            LZW           |" <<endl;

        file2<<"|     TOTAL BYTES    |"<< vetTotalBytes[k]<<endl;

        file2<<"|  TOTAL BYTES COMPRESSAO  |"<< vetTotalBytesComp[k]<<endl;

        file2<<"|     RAZAO DA COMPRESSAO     |"<< vetRazaoCompressao[k]<<endl;

        file2<<"|     TEMPO TOTAL    |"<< vetTempoTotal[k]<<endl;







    }


}

using namespace std;

int main()
{
    codLzw();


}
