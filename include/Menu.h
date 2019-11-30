#ifndef MENU_H
#define MENU_H
#include "registro.h"
#include <bits/stdc++.h>
#include <list>
#include <iterator>
#include <vector>

using namespace std;

class Menu
{
    public:
        Menu();
        ~Menu();
        void exibirMenu();
        void leitura(vector<registro>& lista,vector<string>& linha,vector<vector<string>>& linhas,int N);
    private:
};

#endif // MENU_H
