#include<fstream>
#include<iostream>
#include<string>
#include<list>

using namespace std;

class Util{
    public:
        int ReadGraph(string dataset_path,int **&Graph, int *&degree, int &bipartite);
        int ReadGraph(string dataset_path,int **&Graph, int *&degree); 
};

int Util::ReadGraph(string dataset_path,int **&Graph, int *&degree, int &bipartite){
    ifstream read;
    read.open(dataset_path);
    string temp;
    read>>temp;
    int graph_size=stoi(temp);
    Graph=new int*[graph_size]; 
    delete []degree;
    degree=new int[graph_size];
    read>>temp;
    int B_index=stoi(temp);
    bipartite=B_index;
    read>>temp;
    int index=0;
    int *neg=new int[graph_size];
    char a;
    int temp_count=0;
    bool first=true;
    while(!read.eof()){
        if(first){
            read>>temp;
            first=false;
        }       
        read.get(a);
        if(a=='\n'){
            if(index>=graph_size)
                break;
            degree[index]=temp_count;
            int *temp_array=new int[temp_count];
            for(int i=0;i<temp_count;++i){
                temp_array[i]=neg[i];
            }
            Graph[index]=temp_array;
            temp_count=0;
            index++;
            first=true;
            continue;
        }
        read>>temp;
        neg[temp_count]=stoi(temp);
        temp_count++;
        
    }
    delete []neg;
    return graph_size;
}

int Util::ReadGraph(string dataset_path,int **&Graph, int *&degree){
    ifstream read;
    read.open(dataset_path);
    string temp;
    read>>temp;
    int graph_size=stoi(temp);
    Graph=new int*[graph_size]; 
    delete []degree;
    degree=new int[graph_size];
    read>>temp;
    int index=0;
    int *neg=new int[graph_size];
    char a;
    int temp_count=0;
    bool first=true;
    while(!read.eof()){
        if(first){
            read>>temp;
            first=false;
        }       
        read.get(a);
        if(a=='\n'){
            if(index>=graph_size)
                break;
            degree[index]=temp_count;
            int *temp_array=new int[temp_count];
            for(int i=0;i<temp_count;++i){
                temp_array[i]=neg[i];
            }
            Graph[index]=temp_array;
            temp_count=0;
            index++;
            first=true;
            continue;
        }
        read>>temp;
        neg[temp_count]=stoi(temp);
        temp_count++;
        
    }
    delete []neg;
    return graph_size;
}