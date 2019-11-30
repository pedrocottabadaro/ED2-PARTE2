#include "huffman.h"
#include "noHuff.h"
#include <iostream>
#include <queue>
#include <unordered_map> //biblioteca de hash table
#include <vector>

using namespace std;

huffman::huffman(string texto1){


   this->texto = texto1;
   noHuff *raiz = new noHuff();
   raiz = nullptr;
   taxaCompressao = 0;
   bytesComprimido = 0;
   comprimido = "";
}

huffman::~huffman(){

    auxDestrutor(this->raiz);
}

void huffman::criaArvore(){


    //a variavel frequencia armazena a frequencia de cada um dos caracteres em uma estrutura de map
    unordered_map<char, int> frequencia;
    for (char ch:texto) {
        frequencia[ch]++;


    }


    //cria uma fila de prioridade para armazenar os n�s da arvore
    priority_queue<noHuff*, vector<noHuff*>, comp> fp;

    //Cria um n� folha para cada caractere e adiciona a fila de prioridade
    for (auto pair:frequencia) {
        noHuff* no = new noHuff(pair.first,pair.second);
        fp.push(no);


    }

    while (fp.size() > 1)
    {
        // Remove os n�s com menor frequencia da fila
        noHuff *esq = new noHuff();
        esq = fp.top(); fp.pop();
        noHuff *dir = new noHuff();
        dir = fp.top(); fp.pop();


        // Cria um n� interno cuja frequencia � a soma
        // da frequencia dos dois n�s
        // e coloca esses n�s como filhos,
        // adicionando o novo n� a fila
        int soma = esq->getFreq() + dir->getFreq();
        noHuff *no = new noHuff('\0', soma, esq, dir);
        fp.push(no);


    }

    //coloca o ultimo n� da fila como raiz
    raiz = fp.top();

}

void huffman::auxDestrutor(noHuff* no){

    if(no!=NULL){
    	auxDestrutor(no->getNoDir());
	auxDestrutor(no->getNoEsq());
	delete no;
    }
}

void huffman::chamaCodificador(){

    //codifica cada caractere
    codificador(raiz, "");
    //codifica a string completa
    for(char ch : texto){
    	comprimido = huffmanCode[ch];
    }
    taxaCompressao = comprimido.size();
    bytesComprimido = (float)taxaCompressao/8.0;
}

/*
 *******************************************************************
 *                                                                 *
 * Fun�ao que codifica o texto de acordo com a arvore ja existente *
 *                                                                 *
 *******************************************************************
 */
void huffman::codificador(noHuff* raiz, string str){

    if(raiz == NULL)
	return;
    //encontra folha
    if(!raiz->getNoEsq() && !raiz->getNoDir())
	huffmanCode[raiz->getData()] = str;
    codificador(raiz->getNoEsq(), str + "0");
    codificador(raiz->getNoDir(), str + "1");
}

void huffman::imprimeCodigoHuffman(){
    cout<<"imprime Codigo Huffman"<<endl;
    cout<<comprimido<<endl;
}

float huffman::getBytesComprimido(){
    cout<<"Num de Bytes Comprimido"<<endl;
    return bytesComprimido;
}

float huffman::getTaxaCompressao(){
   cout<<"Taxa de Compressao"<<endl;
   taxaCompressao = ((float)taxaCompressao/(float)(texto.size()*8));
   return taxaCompressao;
}
