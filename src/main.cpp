#include <iostream>
#include "RBTREE.h"
#include "RBTREE.cpp"
using namespace std;
int main(){
    system("chcp 65001");      //用UTF-8编码解释文字流
    rbtree<int>* a = new rbtree<int>();
    char command='E';
    while(1){
        cin>>command;    
        if(command == 'E')exit(0);
        else if(command == 'I'){
            int n;
            int x;
            cin>>n;
            for(int i=0;i<n;i++){
                cin>>x;
                a->Insert(x);
            }
        } else if(command == 'P'){
            a->LevelOrder();
        } else if (command == 'R'){
            int remove;
            cin>>remove;
            a->Remove(remove);
        }
        
    }

    delete a;
    return 0;
}