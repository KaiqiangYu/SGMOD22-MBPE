#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include "cpp-btree-1.0.1/btree_set.h"
using namespace std;

int OutputResults=0;

class iTraversal
{
public:
    iTraversal(int **Graph, int *degree, int Graph_size, int Bipartite_index, int Epsilon, int total_num);
    void miner();
    void ExtendToMax(vector<int> &X, vector<int> &Y);
    void Enumerate(vector<int> &X, vector<int> &Y, vector<int> &Full_X, vector<int> &FIX_X, int index, int output);
    void TransToString(vector<int> &X, vector<int> &Y, vector<int> &FIX, string &res);
    void TransToString2(vector<int> &X, vector<int> &Y, string &res);
    inline bool FindNext(vector<int> &X, vector<int>::iterator arr[], int S);
    bool checkY(vector<int> &X, int yuj, vector<int> &cons_Y, vector<int>::iterator Inc[], vector<int> &FIX_X);
    void GetSAT();

    int total_num;
    int edges = 0;
    int res_count = 0;

private:
    int **Graph;
    int Graph_size;
    int Bipartite_index;
    int *degree;
    int Epsilon;
    string *str_list;
    char temp_char[15];
    int *G_index;
    int *G_degree;
    int *G_temp;
    int *G_temp2;
    int *G_temp3;
    int *G_temp4;
    int *G_mark;
    int *G_exc;
    int temp_yu;

    btree::btree_set<string> Btree;
    vector<int> X_a, Y_a;
    vector<int>::iterator temp_iter, temp_iter1, temp_iter2, temp_iter3;
    bool temp_check, temp_check1, temp_check2;
    int temp_i, temp_j, temp_v1, temp_v2, temp_count1, temp_count2, temp_node1, temp_node2, ac_node;
};

iTraversal::iTraversal(int **Graph, int *degree, int Graph_size, int Bipartite_index, int Epsilon, int total_num)
{
    this->Graph = Graph;
    this->degree = degree;
    this->Graph_size = Graph_size;
    this->Bipartite_index = Bipartite_index;
    this->Epsilon = Epsilon;
    this->total_num = total_num;

    G_index = new int[Graph_size];
    G_degree = new int[Graph_size];
    G_temp = new int[Graph_size];
    G_temp2 = new int[Graph_size];
    G_temp3 = new int[Graph_size];
    G_mark = new int[Graph_size];
    G_exc = new int[Graph_size];
    str_list = new string[Graph_size];
    G_temp4 = new int[Graph_size];

    bool inital=true;

    for (int i = 0; i < Graph_size; ++i)
    {
        G_index[i] = 0;
        G_degree[i] = 0;
        G_temp[i] = 0;
        G_temp2[i] = 0;
        G_temp3[i] = 0;
        G_mark[i] = 0;
        G_exc[i] = 0;
        G_temp4[i] = 0;
        str_list[i] = string(itoa(i, temp_char, 10)) + "&";
        if(i<Bipartite_index&& degree[i]>(Graph_size-Bipartite_index-Epsilon)){
            inital=false;
        }
    }

    if(inital){
        res_count++;
        //cout<<"all vertices in R"<<endl;
    }
}
inline bool iTraversal::FindNext(vector<int> &X, vector<int>::iterator arr[], int S)
{
    temp_iter = X.end();
    for (temp_i = S - 1; temp_i >= 0; --temp_i)
    {
        ++arr[temp_i];
        if (arr[temp_i] != temp_iter)
        {
            temp_check = true;
            for (temp_j = temp_i + 1; temp_j < S; ++temp_j)
            {
                arr[temp_j] = arr[temp_j - 1];
                ++arr[temp_j];
                if (arr[temp_j] == temp_iter)
                {
                    temp_check = false;
                    break;
                }
            }
            if (temp_check)
                return temp_check;
        }
    }
    return false;
}

void iTraversal::miner()
{
    int node = 0;
    bool extension = true;
    vector<int>::iterator iter;
    vector<int>::iterator iter1;
    bool ext = false;
    int temp_node;
    int output = 0;

    vector<int> X;
    vector<int> Y;
    vector<int> FIX_X;
    vector<int> FIX_Y;
    vector<int> NA, new_X, new_FIX_X;
    vector<int>::iterator Inc[Epsilon];
    bool redo = true;
    bool exc_yu = true;

    for (int povit = 0; povit < Bipartite_index; ++povit)
    {
        if (res_count >= total_num)
        {
            return;
        }
        X.clear();
        Y.clear();
        FIX_X.clear();
        FIX_Y.clear();
        

        X.push_back(povit);
        G_index[povit] = 1;

        
        for (temp_i = 0; temp_i < degree[povit]; ++temp_i)
        {
            node = Graph[povit][temp_i];
            G_index[node] = 1;
            G_degree[node] = 1;
            FIX_Y.push_back(node);
            Y.push_back(node);
            G_mark[node] = 1;
            
            for (temp_j = degree[node] - 1; temp_j >= 0; --temp_j)
            {
                G_degree[Graph[node][temp_j]]++;
            }
        }

        NA.clear();
        for (temp_i = Bipartite_index; temp_i < Graph_size; ++temp_i)
        {
            if (G_index[temp_i] == 0)
            {
                NA.push_back(temp_i);
            }
        }

        int s = ((int)(NA.size()) > Epsilon) ? Epsilon : (int)(NA.size());
        Inc[0] = NA.begin();
        for (temp_i = 1; temp_i < s; ++temp_i)
        {
            Inc[temp_i] = Inc[temp_i - 1];
            Inc[temp_i]++;
        }

        redo = true;
        while (redo)
        {
            for (temp_i = 0; temp_i < s; ++temp_i)
            {
                
                temp_v1 = *Inc[temp_i];
                G_index[temp_v1] = 1;
                for (temp_j = degree[temp_v1] - 1; temp_j >= 0; --temp_j)
                {
                    G_degree[Graph[temp_v1][temp_j]]++;
                }
            }

            extension = true;
            exc_yu = true;
            for (temp_i = 0; temp_i < s; ++temp_i)
            {
                temp_v2 = *Inc[temp_i];
                temp_v1 = degree[temp_v2];
                for (temp_j = 0; temp_j < temp_v1; ++temp_j)
                {
                    node = Graph[temp_v2][temp_j];
                    if (G_index[node] == 0 && G_degree[node] == degree[povit] + s && povit != node)
                    {
                        if (node < povit)
                        {
                            extension = false;
                            break;
                        }
                        FIX_X.push_back(node);
                        G_index[node] = 2;
                    }
                }
            }

            if (extension)
            {
                for (temp_i = 0; temp_i < s; ++temp_i)
                {
                    temp_v1 = *Inc[temp_i];
                    Y.push_back(temp_v1);
                    FIX_Y.push_back(temp_v1);
                    G_mark[temp_v1] = 1;
                }
                
                temp_iter1 = Y.end();
                if (Y.size() + s > Epsilon)
                {
                    for (temp_i = 0; temp_i < s; ++temp_i)
                    {
                        temp_v2 = degree[*Inc[temp_i]];
                        for (temp_j = 0; temp_j < temp_v2; ++temp_j)
                        {
                            node = Graph[*Inc[temp_i]][temp_j];
                            if (G_index[node] == 0 && G_degree[node] >= Y.size() - Epsilon)
                            {
                                ext = true;
                                for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                {
                                    G_temp4[Graph[node][temp_v1]] = 1;
                                }
                                for (temp_iter = Y.begin(); temp_iter != temp_iter1; ++temp_iter)
                                {
                                    if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < (int)(X.size()) - Epsilon + 1)
                                    {
                                        ext = false;
                                        break;
                                    }
                                }
                                for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                {
                                    G_temp4[Graph[node][temp_v1]] = 0;
                                }
                                if (ext)
                                {
                                    if (node < povit)
                                    {
                                        exc_yu = false;
                                        break;
                                    }
                                    X.push_back(node);
                                    G_index[node] = 1;
                                    for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                    {
                                        G_degree[Graph[node][temp_v1]]++;
                                    }
                                }
                            }
                        }
                    }

                    if (degree[povit] != 0 && exc_yu)
                    {
                        int temp_node = Graph[povit][0];
                        for (int j = 0; j < degree[temp_node]; ++j)
                        {
                            node = Graph[temp_node][j];
                            if (G_index[node] == 0 && G_degree[node] >= Y.size() - Epsilon)
                            {
                                ext = true;
                                for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                {
                                    G_temp4[Graph[node][temp_v1]] = 1;
                                }
                                for (iter = Y.begin(); iter != temp_iter1; ++iter)
                                {
                                    if (!G_temp4[*iter] && G_degree[*iter] < X.size() - Epsilon + 1)
                                    {
                                        ext = false;
                                        break;
                                    }
                                }
                                for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                {
                                    G_temp4[Graph[node][temp_v1]] = 0;
                                }
                                if (ext)
                                {
                                    if (node < povit)
                                    {
                                        exc_yu = false;
                                        break;
                                    }
                                    X.push_back(node);
                                    G_index[node] = 1;
                                    for (temp_v1 = degree[node] - 1; temp_v1 >= 0; --temp_v1)
                                    {
                                        G_degree[Graph[node][temp_v1]]++;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    
                    for (temp_i = 0; temp_i < Bipartite_index; ++temp_i)
                    {
                        if (G_index[temp_i] == 0 && G_degree[temp_i] >= (int)Y.size() - Epsilon)
                        {
                            ext = true;
                            for (temp_v1 = degree[temp_i] - 1; temp_v1 >= 0; --temp_v1)
                            {
                                G_temp4[Graph[temp_i][temp_v1]] = 1;
                            }
                            for (iter = Y.begin(); iter != temp_iter1; ++iter)
                            {
                                if (!G_temp4[*iter] && G_degree[*iter] < (int)(X.size()) - Epsilon + 1)
                                {
                                    ext = false;
                                    break;
                                }
                            }
                            for (temp_v1 = degree[temp_i] - 1; temp_v1 >= 0; --temp_v1)
                            {
                                G_temp4[Graph[temp_i][temp_v1]] = 0;
                            }
                            if (ext)
                            {
                                if (temp_i < povit)
                                {
                                    exc_yu = false;
                                    break;
                                }
                                X.push_back(temp_i);
                                G_index[temp_i] = 1;
                                for (temp_v1 = degree[temp_i] - 1; temp_v1 >= 0; --temp_v1)
                                {
                                    G_degree[Graph[temp_i][temp_v1]]++;
                                }
                            }
                        }
                    }
                }
                if (exc_yu)
                {
                    string temp_res;
                    TransToString(X, Y, FIX_X, temp_res);
                    if (Btree.find(temp_res) == Btree.end())
                    {
                        if (output % 2 == 0)
                        {
                            res_count++;
                            if(OutputResults){
                                cout<<temp_res<<endl;
                            }
                            if (res_count >= total_num)
                            {
                                return;
                            }
                            
                        }
                        Btree.insert(temp_res);
                        new_X.clear();
                        new_FIX_X.clear();
                        for (temp_iter = X.begin(), temp_iter1 = X.end(); temp_iter != temp_iter1; ++temp_iter)
                        {
                            new_X.push_back(*temp_iter);
                        }
                        for (temp_iter = FIX_X.begin(), temp_iter1 = FIX_X.end(); temp_iter != temp_iter1; ++temp_iter)
                        {
                            new_FIX_X.push_back(*temp_iter);
                        }
                        Enumerate(new_X, Y, FIX_Y, new_FIX_X, 2, 0);
                        if (res_count >= total_num)
                        {
                            return;
                        }
                        if (output % 2)
                        {
                            res_count++;
                            if (OutputResults){
                                cout<<temp_res<<endl;
                            }
                            if (res_count >= total_num)
                            {
                                return;
                            }
                            
                        }
                    }
                }

                iter = X.begin();
                ++iter;
                temp_iter = X.end();
                for (; iter != temp_iter; ++iter)
                {
                    for (temp_i = degree[*iter] - 1; temp_i >= 0; --temp_i)
                    {
                        G_degree[Graph[*iter][temp_i]]--;
                    }
                    G_index[*iter] = 0;
                }
                temp_iter = FIX_X.end();
                for (iter = FIX_X.begin(); iter != temp_iter; ++iter)
                {
                    G_index[*iter] = 0;
                }
                FIX_X.clear();
                for (temp_i = 0; temp_i < s; ++temp_i)
                {
                    iter = FIX_Y.end();
                    iter--;
                    FIX_Y.erase(iter);
                    G_mark[*Inc[temp_i]] = 0;
                }
                X.clear();
                X.push_back(povit);
                for (temp_i = 0; temp_i < s; ++temp_i)
                {
                    iter = Y.end();
                    iter--;
                    G_index[*iter] = 0;
                    Y.erase(iter);
                }
            }
           
            for (temp_i = s - 1; temp_i >= 0; --temp_i)
            {
                
                temp_node1 = *Inc[temp_i];
                G_index[temp_node1] = 0;
                for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                {
                    --G_degree[Graph[temp_node1][temp_j]];
                }
            }
            for (iter = FIX_X.begin(), temp_iter = FIX_X.end(); iter != temp_iter; ++iter)
            {
                G_index[*iter] = 0;
            }
            FIX_X.clear();
            redo = FindNext(NA, Inc, s);
        }

        
        for (iter = X.begin(), temp_iter = X.end(); iter != temp_iter; ++iter)
        {
            G_index[*iter] = 0;
        }
        for (temp_i = degree[povit] - 1; temp_i >= 0; --temp_i)
        {
            node = Graph[povit][temp_i];
            G_index[node] = 0;
            G_degree[node] = 0;
            G_mark[node] = 0;
            
            for (temp_j = degree[node] - 1; temp_j >= 0; --temp_j)
            {
                G_degree[Graph[node][temp_j]] = 0;
            }
        }
        G_exc[povit] = 1;
    }
}

void iTraversal::GetSAT()
{
    cout << "----------------- Satistical Infomation -----------------" << endl;
    cout << "total number: " << res_count << endl;
    cout << "edges: " << edges << endl;
    cout << "---------------------------------------------------------" << endl;
}

void iTraversal::TransToString(vector<int> &X, vector<int> &Y, vector<int> &FIX, string &res)
{
    ++edges;

    X_a.clear();
    Y_a.clear();
    for (temp_iter = X.begin(), temp_iter1 = X.end(); temp_iter != temp_iter1; ++temp_iter)
    {
        X_a.push_back(*temp_iter);
    }
    for (temp_iter = Y.begin(), temp_iter1 = Y.end(); temp_iter != temp_iter1; ++temp_iter)
    {
        Y_a.push_back(*temp_iter);
    }
    for (temp_iter = FIX.begin(), temp_iter1 = FIX.end(); temp_iter != temp_iter1; ++temp_iter)
    {
        X_a.push_back(*temp_iter);
    }

    res.reserve(2 * (X.size() + Y.size()));

    temp_iter1 = X_a.begin();
    temp_iter2 = X_a.end();
    sort(temp_iter1, temp_iter2);
    for (; temp_iter1 != temp_iter2; ++temp_iter1)
    {
        res += str_list[*temp_iter1];
    }

    temp_iter1 = Y_a.begin();
    temp_iter2 = Y_a.end();
    sort(temp_iter1, temp_iter2);
    for (; temp_iter1 != temp_iter2; ++temp_iter1)
    {
        res += str_list[*temp_iter1];
    }
}

void iTraversal::TransToString2(vector<int> &X, vector<int> &Y, string &res)
{
    ++edges;
    res.reserve(2 * (X.size() + Y.size()));

    temp_iter1 = X.begin();
    temp_iter2 = X.end();
    sort(temp_iter1, temp_iter2);
    for (; temp_iter1 != temp_iter2; ++temp_iter1)
    {
        res += str_list[*temp_iter1];
    }

    temp_iter1 = Y.begin();
    temp_iter2 = Y.end();
    sort(temp_iter1, temp_iter2);
    for (; temp_iter1 != temp_iter2; ++temp_iter1)
    {
        res += str_list[*temp_iter1];
    }
}

void iTraversal::Enumerate(vector<int> &X, vector<int> &Y, vector<int> &Full_Y, vector<int> &FIX_X, int depth, int output)
{
    if (Y.size() == 0 || res_count >= total_num)
        return;
    
    output = 0;
    int count_yu = 0;
    bool check_miss = true;
    bool extension = true;
    bool ext = false;
    bool yu_exc = true;
    vector<int>::iterator Inc[Epsilon];
    vector<int>::iterator fix[Epsilon];
    int s, NB_S;
    int yu_s1 = 0, yu_s2 = 0;
    bool redo = true, redo2;
    vector<int> F_Y, new_FIX_X, new_new_X, cons_Y, opt_Y, new_X, temp_FIX_X, NB;

    
    for (int povit = 0; povit < Bipartite_index; ++povit)
    {
        if (res_count >= total_num)
        {
            return;
        }
        if (G_index[povit] > 0 || G_exc[povit] > 0)
            continue;
        
        cons_Y.clear();
        opt_Y.clear();
        check_miss = true;
        extension = true;

        G_index[povit] = 1;
        for (temp_i = degree[povit] - 1; temp_i >= 0; --temp_i)
        {
            temp_node1 = Graph[povit][temp_i];
            G_temp[temp_node1] = 1;
            ++G_degree[temp_node1];
        }
        for (temp_iter = Y.begin(), temp_iter1 = Y.end(); temp_iter != temp_iter1; ++temp_iter)
        {
            temp_node1 = *temp_iter;
            if (G_temp[temp_node1] == 0)
            {
                opt_Y.push_back(temp_node1);
                G_index[temp_node1] = 0;
                for (temp_i = degree[temp_node1] - 1; temp_i >= 0; --temp_i)
                {
                    G_degree[Graph[temp_node1][temp_i]]--;
                }
            }
            else
            {
                cons_Y.push_back(temp_node1);
            }
        }
        for (temp_i = degree[povit] - 1; temp_i >= 0; --temp_i)
        {
            G_temp[Graph[povit][temp_i]] = 0;
        }

        
        temp_v1 = (int)(X.size());
        temp_v2 = cons_Y.size();
        for (temp_iter = opt_Y.begin(), temp_iter1 = opt_Y.end(); temp_iter != temp_iter1; ++temp_iter)
        {
            if (G_degree[*temp_iter] >= temp_v1 - Epsilon + 1 || temp_v2 == 0)
            {
                check_miss = false;
                break;
            }
        }
        yu_exc = true;

        
        if (check_miss)
        {

            extension = true;
            temp_FIX_X.clear();
            temp_iter = cons_Y.begin();
            temp_v2 = cons_Y.size();
            for (int i = 0; i < degree[*temp_iter]; ++i)
            {
                temp_node1 = Graph[*temp_iter][i];
                if (G_index[temp_node1] == 0 && G_degree[temp_node1] == temp_v2 && temp_node1 != povit)
                {
                    if (G_exc[temp_node1] > 0)
                    {
                        extension = false;
                        break;
                    }
                    temp_FIX_X.push_back(temp_node1);
                    G_index[temp_node1] = 2;
                }
            }

            if (extension)
            {
                
                temp_iter1 = X.end();
                temp_iter3 = FIX_X.end();
                temp_v1 = (int)X.size() - Epsilon + 1;
                temp_v2 = (int)cons_Y.size() - Epsilon + 1;
                if (X.size() > Epsilon)
                {
                    temp_count1 = 0;
                    for (temp_iter = X.begin(); temp_iter != temp_iter1 && temp_count1 <= Epsilon; ++temp_iter)
                    {
                        temp_count1++;
                        for (temp_i = 0; temp_i < degree[*temp_iter]; ++temp_i)
                        {
                            temp_node1 = Graph[*temp_iter][temp_i];
                            if (G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v1)
                            {
                                extension = false;
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 1;
                                }
                                for (temp_iter2 = X.begin(); temp_iter2 != temp_iter1; ++temp_iter2)
                                {
                                    if (!G_temp4[*temp_iter2] && G_degree[*temp_iter2] < temp_v2)
                                    {
                                        extension = true;
                                        break;
                                    }
                                }
                                
                                if (G_temp3[povit] == 0 && G_degree[povit] < temp_v2)
                                {
                                    extension = true;
                                }
                                if (!extension && FIX_X.size() != 0)
                                {
                                    temp_count2 = 0;
                                    for (temp_iter2 = FIX_X.begin(); temp_iter2 != temp_iter3; ++temp_iter2)
                                    {
                                        if (!G_temp4[*temp_iter2])
                                        {
                                            ++temp_count2;
                                        }
                                    }
                                    if (G_degree[temp_node1] < temp_v1 + temp_count2)
                                    {
                                        extension = true;
                                    }
                                }
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 0;
                                }
                                if (!extension)
                                    break;
                            }
                        }
                        if (!extension)
                            break;
                    }
                }
                else
                {
                    for (temp_i = Bipartite_index; temp_i < Graph_size; ++temp_i)
                    {
                        if (G_index[temp_i] == 0 && G_degree[temp_i] >= temp_v1)
                        {
                            extension = false;
                            for (temp_yu = degree[temp_i] - 1; temp_yu >= 0; --temp_yu)
                            {
                                G_temp4[Graph[temp_i][temp_yu]] = 1;
                            }
                            for (temp_iter = X.begin(); temp_iter != temp_iter1; ++temp_iter)
                            {
                                if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v2)
                                {
                                    extension = true;
                                    break;
                                }
                            }
                            
                            if (G_temp4[povit] == 0 && G_degree[povit] < temp_v2)
                            {
                                extension = true;
                            }
                            if (!extension && FIX_X.size() != 0)
                            {
                                temp_count2 = 0;
                                for (temp_iter = FIX_X.begin(); temp_iter != temp_iter3; ++temp_iter)
                                {
                                    if (!G_temp4[*temp_iter])
                                    {
                                        ++temp_count2;
                                    }
                                }
                                if (G_degree[temp_i] < temp_v1 + temp_count2)
                                {
                                    extension = true;
                                }
                            }
                            for (temp_yu = degree[temp_i] - 1; temp_yu >= 0; --temp_yu)
                            {
                                G_temp4[Graph[temp_i][temp_yu]] = 0;
                            }
                            if (!extension)
                                break;
                        }
                    }
                }
            }

            if (extension)
            {
                new_X.clear();
                new_X.push_back(povit);
                temp_iter2 = X.end();
                temp_iter3 = cons_Y.end();
                temp_v1 = (int)X.size() + 2 - Epsilon;
                temp_v2 = (int)cons_Y.size() - Epsilon;

                if (cons_Y.size() > Epsilon)
                {
                    temp_count1 = 0;
                    for (temp_iter = cons_Y.begin(); temp_iter != temp_iter3 && temp_count1 <= Epsilon; ++temp_iter)
                    {
                        temp_count1++;
                        for (temp_i = 0; temp_i < degree[*temp_iter]; ++temp_i)
                        {
                            temp_node1 = Graph[*temp_iter][temp_i];
                            if (temp_node1 != povit && G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v2)
                            {
                                temp_check = true;
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 1;
                                }
                                for (temp_iter1 = cons_Y.begin(); temp_iter1 != temp_iter3; ++temp_iter1)
                                {
                                    if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v1)
                                    {
                                        temp_check = false;
                                        break;
                                    }
                                }
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 0;
                                }
                                if (temp_check)
                                {
                                    if (G_exc[temp_node1])
                                    {
                                        yu_exc = false;
                                        break;
                                    }
                                    new_X.push_back(temp_node1);
                                    ++temp_v1;
                                    G_index[temp_node1] = 1;
                                    for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                    {
                                        G_degree[Graph[temp_node1][temp_j]]++;
                                    }
                                }
                            }
                        }
                        if (G_degree[*temp_iter] == temp_v1 - 1)
                        {
                            break;
                        }
                        if (!yu_exc)
                            break;
                    }
                }
                else
                {
                    temp_check1 = true;
                    for (temp_iter = cons_Y.begin(); temp_iter != temp_iter3; ++temp_iter)
                    {
                        if (G_degree[*temp_iter] == temp_v1 - 1)
                        {
                            temp_check1 = false;
                            break;
                        }
                    }
                    if (!temp_check1)
                    {
                        for (temp_i = 0; temp_i < degree[*temp_iter]; ++temp_i)
                        {
                            temp_node1 = Graph[*temp_iter][temp_i];
                            if (temp_node1 != povit && G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v2)
                            {
                                temp_check = true;
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 1;
                                }
                                for (temp_iter1 = cons_Y.begin(); temp_iter1 != temp_iter3; ++temp_iter1)
                                {
                                    if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v1)
                                    {
                                        temp_check = false;
                                        break;
                                    }
                                }
                                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                {
                                    G_temp4[Graph[temp_node1][temp_yu]] = 0;
                                }
                                if (temp_check)
                                {
                                    if (G_exc[temp_node1])
                                    {
                                        yu_exc = false;
                                        break;
                                    }
                                    new_X.push_back(temp_node1);
                                    ++temp_v1;
                                    G_index[temp_node1] = 1;
                                    for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                    {
                                        G_degree[Graph[temp_node1][temp_j]]++;
                                    }
                                }
                            }
                        }
                        if (!yu_exc)
                            break;
                    }
                    for (temp_node1 = 0; temp_node1 < Bipartite_index && temp_check1; ++temp_node1)
                    {
                        if (temp_node1 != povit && G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v2)
                        {
                            temp_check = true;
                            for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                            {
                                G_temp4[Graph[temp_node1][temp_yu]] = 1;
                            }
                            for (temp_iter = cons_Y.begin(); temp_iter != temp_iter3; ++temp_iter)
                            {
                                if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v1)
                                {
                                    temp_check = false;
                                    break;
                                }
                            }
                            for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                            {
                                G_temp4[Graph[temp_node1][temp_yu]] = 0;
                            }
                            if (temp_check)
                            {
                                if (G_exc[temp_node1])
                                {
                                    yu_exc = false;
                                    break;
                                }
                                new_X.push_back(temp_node1);
                                G_index[temp_node1] = 1;
                                ++temp_v1;
                                for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                {
                                    G_degree[Graph[temp_node1][temp_j]]++;
                                }
                            }
                        }
                    }
                }
                if (yu_exc)
                {

                    X_a.clear();
                    F_Y.clear();
                    for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                    {
                        X_a.push_back(*temp_iter);
                    }
                    for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                    {
                        X_a.push_back(*temp_iter);
                    }
                    for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                    {
                        X_a.push_back(*temp_iter);
                    }
                    for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                    {
                        X_a.push_back(*temp_iter);
                    }
                    for (temp_iter = cons_Y.begin(), temp_iter2 = cons_Y.end(); temp_iter != temp_iter2; ++temp_iter)
                    {
                        F_Y.push_back(*temp_iter);
                    }

                    string temp_res;
                    TransToString2(X_a, F_Y, temp_res);
                    if (Btree.find(temp_res) == Btree.end())
                    {
                        if (output % 2 == 0)
                        {
                            ++res_count;
                            if(OutputResults){
                                cout<<temp_res<<endl;
                            }
                            if (res_count >= total_num)
                            {
                                return;
                            }
                            
                        }
                        Btree.insert(temp_res);
                        new_FIX_X.clear();
                        new_new_X.clear();
                        for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                        {
                            new_new_X.push_back(*temp_iter);
                        }
                        for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                        {
                            new_new_X.push_back(*temp_iter);
                        }
                        for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                        {
                            new_FIX_X.push_back(*temp_iter);
                        }
                        for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                        {
                            new_FIX_X.push_back(*temp_iter);
                        }
                        Enumerate(new_new_X, F_Y, Full_Y, new_FIX_X, depth + 1, output + 1);
                        if (res_count >= total_num)
                        {
                            return;
                        }
                        if (output % 2)
                        {
                            ++res_count;
                            if(OutputResults){
                                cout<<temp_res<<endl;
                            }
                            if (res_count >= total_num)
                            {
                                return;
                            }
                            
                        }
                    }
                }
                
                for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                {
                    if (*temp_iter != povit)
                    {
                        G_index[*temp_iter] = 0;
                        for (temp_i = degree[*temp_iter] - 1; temp_i >= 0; --temp_i)
                        {
                            G_degree[Graph[*temp_iter][temp_i]]--;
                        }
                    }
                }
            }
            for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
            {
                G_index[*temp_iter] = 0;
            }
        }

        s = ((int)(opt_Y.size()) > Epsilon) ? Epsilon : (int)(opt_Y.size());
        yu_s1 = yu_s2 = 0;
        temp_v1 = (int)(X.size()) + 1 - Epsilon;
        for (temp_iter = opt_Y.begin(), temp_iter1 = opt_Y.end(); temp_iter != temp_iter1; ++temp_iter)
        {
            if (G_degree[*temp_iter] >= temp_v1)
            {
                ++yu_s1;
            }
        }
        yu_s2 = (int)opt_Y.size() - yu_s1;

        for (int yuj = 1; yuj <= s; ++yuj)
        {
            Inc[0] = opt_Y.begin();
            for (temp_i = 1; temp_i < yuj; ++temp_i)
            {
                Inc[temp_i] = Inc[temp_i - 1];
                Inc[temp_i]++;
            }
            redo = true;
            while (redo)
            {
                extension = true;
                yu_exc = true;
                
                for (temp_i = 0; temp_i < yuj; ++temp_i)
                {
                    if (G_degree[*Inc[temp_i]] < (int)(X.size()) + 1 - Epsilon)
                    {
                        extension = false;
                        break;
                    }
                }
                if (extension)
                {
                    if ((yuj < s && s < yu_s1) || (s >= yu_s1 && yuj < yu_s1))
                    {
                        redo = FindNext(opt_Y, Inc, yuj); 
                        continue;
                    }
                    temp_FIX_X.clear();
                    for (temp_i = 0; temp_i < yuj; ++temp_i)
                    {
                        temp_node1 = *Inc[temp_i];
                        for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                        {
                            G_degree[Graph[temp_node1][temp_j]]++;
                        }
                    }
                    temp_node1 = *Inc[0];
                    temp_v1 = (int)((int)Y.size() - (int)opt_Y.size()) + yuj;
                    for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                    {
                        temp_node2 = Graph[temp_node1][temp_i];
                        if (G_index[temp_node2] == 0 && G_degree[temp_node2] == temp_v1 && temp_node2 != povit)
                        {
                            if (G_exc[temp_node2] > 0)
                            {
                                extension = false;
                                break;
                            }
                            temp_FIX_X.push_back(temp_node2);
                            G_index[temp_node2] = 2;
                        }
                    }

                    if (extension)
                    {
                        for (temp_i = 0; temp_i < yuj; ++temp_i)
                        {
                            G_index[*Inc[temp_i]] = 1;
                        }
                        new_X.clear();
                        new_X.push_back(povit);
                        X.push_back(povit);
                        temp_check = checkY(X, yuj, cons_Y, Inc, FIX_X);
                        X.pop_back();
                        if (temp_check)
                        {
                            temp_iter2 = cons_Y.end();
                            temp_v1 = (int)((int)Y.size() - (int)opt_Y.size()) - Epsilon + yuj;
                            temp_v2 = (int)(X.size()) + (int)(new_X.size()) + 1 - Epsilon;
                            if (cons_Y.size() + yuj >= Epsilon + 1)
                            {
                                temp_check2 = true;
                                for (temp_i = 0; temp_i < yuj; ++temp_i)
                                {
                                    temp_node1 = *Inc[temp_i];
                                    for (int j = 0; j < degree[temp_node1]; ++j)
                                    {
                                        temp_node2 = Graph[temp_node1][j];
                                        if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                        {
                                            ext = true;
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                            }
                                            for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v2)
                                                {
                                                    ext = false;
                                                    break;
                                                }
                                            }
                                            if (ext)
                                            {
                                                for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                {
                                                    temp_count1 = *Inc[temp_j];
                                                    if (!G_temp4[temp_count1] && G_degree[temp_count1] < temp_v2)
                                                    {
                                                        ext = false;
                                                        break;
                                                    }
                                                }
                                            }
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                            }
                                            if (ext)
                                            {
                                                if (G_exc[temp_node2] > 0)
                                                {
                                                    yu_exc = false;
                                                    break;
                                                }
                                                new_X.push_back(temp_node2);
                                                ++temp_v2;
                                                G_index[temp_node2] = 1;
                                                for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                {
                                                    G_degree[Graph[temp_node2][temp_j]]++;
                                                }
                                            }
                                        }
                                    }
                                    if (G_degree[temp_node1] == temp_v2 - 1)
                                    {
                                        temp_check2 = false;
                                        break;
                                    }
                                }
                                temp_count1 = temp_check ? yuj : Epsilon + 1;
                                for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2 && temp_count1 <= Epsilon && yu_exc; ++temp_iter)
                                {
                                    ++temp_count1;
                                    temp_node1 = *(temp_iter);
                                    for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                                    {
                                        temp_node2 = Graph[temp_node1][temp_i];
                                        if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                        {
                                            ext = true;
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                            }
                                            for (temp_iter3 = cons_Y.begin(); temp_iter3 != temp_iter2; ++temp_iter3)
                                            {
                                                if (!G_temp4[*temp_iter3] && G_degree[*temp_iter3] < temp_v2)
                                                {
                                                    ext = false;
                                                    break;
                                                }
                                            }
                                            if (ext)
                                            {
                                                for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                {
                                                    temp_count2 = *Inc[temp_j];
                                                    if (!G_temp4[temp_count2] && G_degree[temp_count2] < temp_v2)
                                                    {
                                                        ext = false;
                                                        break;
                                                    }
                                                }
                                            }
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                            }
                                            if (ext)
                                            {
                                                if (G_exc[temp_node2] > 0)
                                                {
                                                    yu_exc = false;
                                                    break;
                                                }
                                                new_X.push_back(temp_node2);
                                                ++temp_v2;
                                                G_index[temp_node2] = 1;
                                                for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                {
                                                    G_degree[Graph[temp_node2][temp_j]]++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                temp_check2 = true;
                                for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    if (G_degree[*temp_iter] == temp_v2 - 1)
                                    {
                                        temp_node1 = *temp_iter;
                                        temp_check2 = false;
                                        break;
                                    }
                                }
                                for (temp_i = 0; temp_i < yuj; ++temp_i)
                                {
                                    temp_node1 = *Inc[temp_i];
                                    if (G_degree[temp_node1] == temp_v2 - 1)
                                    {
                                        temp_check2 = false;
                                        break;
                                    }
                                }
                                if (!temp_check2)
                                {
                                    for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                                    {
                                        temp_node2 = Graph[temp_node1][temp_i];
                                        if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                        {
                                            ext = true;
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                            }
                                            for (temp_iter3 = cons_Y.begin(); temp_iter3 != temp_iter2; ++temp_iter3)
                                            {
                                                if (!G_temp4[*temp_iter3] && G_degree[*temp_iter3] < temp_v2)
                                                {
                                                    ext = false;
                                                    break;
                                                }
                                            }
                                            if (ext)
                                            {
                                                for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                {
                                                    temp_count2 = *Inc[temp_j];
                                                    if (!G_temp4[temp_count2] && G_degree[temp_count2] < temp_v2)
                                                    {
                                                        ext = false;
                                                        break;
                                                    }
                                                }
                                            }
                                            for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                            {
                                                G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                            }
                                            if (ext)
                                            {
                                                if (G_exc[temp_node2] > 0)
                                                {
                                                    yu_exc = false;
                                                    break;
                                                }
                                                new_X.push_back(temp_node2);
                                                ++temp_v2;
                                                G_index[temp_node2] = 1;
                                                for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                {
                                                    G_degree[Graph[temp_node2][temp_j]]++;
                                                }
                                            }
                                        }
                                    }
                                }
                                for (temp_node1 = 0; temp_node1 < Bipartite_index && temp_check2; ++temp_node1)
                                {
                                    if (temp_node1 != povit && G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v1)
                                    {
                                        ext = true;
                                        for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                        {
                                            G_temp4[Graph[temp_node1][temp_yu]] = 1;
                                        }
                                        for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                        {
                                            if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v2)
                                            {
                                                ext = false;
                                                break;
                                            }
                                        }
                                        if (ext)
                                        {
                                            for (temp_i = 0; temp_i < yuj; ++temp_i)
                                            {
                                                temp_node2 = *Inc[temp_i];
                                                if (!G_temp4[temp_node2] && G_degree[temp_node2] < temp_v2)
                                                {
                                                    ext = false;
                                                    break;
                                                }
                                            }
                                        }
                                        for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                        {
                                            G_temp4[Graph[temp_node1][temp_yu]] = 0;
                                        }
                                        if (ext)
                                        {
                                            if (G_exc[temp_node1] > 0)
                                            {
                                                yu_exc = false;
                                                break;
                                            }
                                            new_X.push_back(temp_node1);
                                            ++temp_v2;
                                            G_index[temp_node1] = 1;
                                            for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                            {
                                                G_degree[Graph[temp_node1][temp_j]]++;
                                            }
                                        }
                                    }
                                }
                            }

                            if (yu_exc)
                            {
                                F_Y.clear();
                                X_a.clear();
                                for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    X_a.push_back(*temp_iter);
                                }
                                for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    X_a.push_back(*temp_iter);
                                }
                                for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    X_a.push_back(*temp_iter);
                                }
                                for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    X_a.push_back(*temp_iter);
                                }
                                for (temp_iter = cons_Y.begin(), temp_iter2 = cons_Y.end(); temp_iter != temp_iter2; ++temp_iter)
                                {
                                    F_Y.push_back(*temp_iter);
                                }
                                for (temp_i = 0; temp_i < yuj; ++temp_i)
                                {
                                    F_Y.push_back(*Inc[temp_i]);
                                }

                                string temp_res;
                                TransToString2(X_a, F_Y, temp_res);
                                if (Btree.find(temp_res) == Btree.end())
                                {
                                    if (output % 2 == 0)
                                    {
                                        ++res_count;
                                        if(OutputResults){
                                            cout<<temp_res<<endl;
                                        }
                                        if (res_count >= total_num)
                                        {
                                            return;
                                        }
                                        
                                    }
                                    Btree.insert(temp_res);
                                    new_FIX_X.clear();
                                    new_new_X.clear();

                                    for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                                    {
                                        new_new_X.push_back(*temp_iter);
                                    }
                                    for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                    {
                                        new_new_X.push_back(*temp_iter);
                                    }
                                    for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                    {
                                        new_FIX_X.push_back(*temp_iter);
                                    }
                                    for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                    {
                                        new_FIX_X.push_back(*temp_iter);
                                    }
                                    Enumerate(new_new_X, F_Y, Full_Y, new_FIX_X, depth + 1, output + 1);
                                    if (res_count >= total_num)
                                    {
                                        return;
                                    }
                                    if (output % 2)
                                    {
                                        ++res_count;
                                        if(OutputResults){
                                            cout<<temp_res<<endl;
                                        }
                                        if (res_count >= total_num)
                                        {
                                            return;
                                        }

                                    }
                                }

                            } 
                        }     

                        for (temp_iter = new_X.begin(), temp_iter1 = new_X.end(); temp_iter != temp_iter1; ++temp_iter)
                        {
                            temp_node1 = *temp_iter;
                            if (temp_node1 != povit)
                            {
                                G_index[temp_node1] = 0;
                                for (temp_i = degree[temp_node1] - 1; temp_i >= 0; --temp_i)
                                {
                                    G_degree[Graph[temp_node1][temp_i]]--;
                                }
                            }
                        }
                        for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                        {
                            G_index[*Inc[temp_i]] = 0;
                        }
                    }
                    for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                    {
                        temp_node1 = *Inc[temp_i];
                        for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                        {
                            G_degree[Graph[temp_node1][temp_j]]--;
                        }
                    }
                    for (temp_iter = temp_FIX_X.begin(), temp_iter1 = temp_FIX_X.end(); temp_iter != temp_iter1; ++temp_iter)
                    {
                        G_index[*temp_iter] = 0;
                    }
                    for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                    {
                        G_index[*Inc[temp_i]] = 0;
                    }
                }
                else
                {
                    if ((yu_s1 >= s && yuj < s) || (yu_s1 < s) && (yuj <= yu_s1))
                    {
                        redo = FindNext(opt_Y, Inc, yuj); 
                        continue;
                    }
                    extension = true;
                    temp_FIX_X.clear();
                    for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                    {
                        temp_node1 = *Inc[temp_i];
                        for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                        {
                            G_degree[Graph[temp_node1][temp_j]]++;
                        }
                    }
                    temp_node1 = *Inc[0];
                    temp_v1 = (int)(Y.size() - opt_Y.size()) + yuj;
                    for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                    {
                        temp_node2 = Graph[temp_node1][temp_i];
                        if (G_index[temp_node2] == 0 && G_degree[temp_node2] == temp_v1)
                        {
                            if (G_exc[temp_node2])
                            {
                                extension = false;
                                break;
                            }
                            temp_FIX_X.push_back(temp_node2);
                            G_index[temp_node2] = 2;
                        }
                    }

                    if (extension)
                    {
                        NB.clear();
                        for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                        {
                            temp_node1 = *Inc[temp_i];
                            for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                            {
                                G_temp2[Graph[temp_node1][temp_j]]++;
                            }
                        }
                        
                        for (temp_iter = X.begin(), temp_iter1 = X.end(); temp_iter != temp_iter1; ++temp_iter)
                        {
                            if (G_temp2[*temp_iter] != yuj)
                            {
                                NB.push_back(*temp_iter);
                            }
                            
                        }
                        for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                        {
                            temp_node1 = *Inc[temp_i];
                            for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                            {
                                G_temp2[Graph[temp_node1][temp_j]]++;
                            }
                        }
                        NB_S = ((int)(NB.size()) > s) ? s : NB.size();
                        for (int k = 1; k <= NB_S; ++k)
                        {
                            fix[0] = NB.begin();
                            for (temp_i = 1; temp_i < k; ++temp_i)
                            {
                                fix[temp_i] = fix[temp_i - 1];
                                fix[temp_i]++;
                            }

                            redo2 = true;
                            while (redo2)
                            {
                                for (temp_i = 0; temp_i < k; ++temp_i)
                                {
                                    temp_node1 = *fix[temp_i];
                                    for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                    {
                                        G_degree[Graph[temp_node1][temp_j]]--;
                                    }
                                    G_index[temp_node1] = 0;
                                    for (temp_iter = X.begin(), temp_iter1 = X.end(); temp_iter != temp_iter1; ++temp_iter)
                                    {
                                        if (*temp_iter == temp_node1)
                                        {
                                            X.erase(temp_iter);
                                            break;
                                        }
                                    }
                                }
                                
                                extension = true;
                                temp_v1 = (int)X.size() + 1 - Epsilon;
                                for (temp_i = 0; temp_i < yuj; ++temp_i)
                                {
                                    if (G_degree[*Inc[temp_i]] < temp_v1)
                                    {
                                        extension = false;
                                        break;
                                    }
                                }

                                if (extension)
                                {

                                    for (temp_i = 0; temp_i < yuj; ++temp_i)
                                    {
                                        G_index[*Inc[temp_i]] = 1;
                                    }
                                    new_X.clear();
                                    new_X.push_back(povit);

                                    X.push_back(povit);
                                    temp_check = checkY(X, yuj, cons_Y, Inc, FIX_X);
                                    X.pop_back();
                                    if (temp_check)
                                    {
                                        yu_exc = true;
                                        temp_iter2 = cons_Y.end();
                                        temp_v1 = (int)(Y.size() - opt_Y.size()) - Epsilon + yuj;
                                        temp_v2 = (int)(X.size()) + (int)(new_X.size()) + 1 - Epsilon;
                                        if (cons_Y.size() + yuj >= Epsilon + 1)
                                        {
                                            temp_check2 = true;
                                            for (temp_i = 0; temp_i < yuj; ++temp_i)
                                            {
                                                temp_node1 = *(Inc[temp_i]);
                                                for (int j = 0; j < degree[temp_node1]; ++j)
                                                {
                                                    temp_node2 = Graph[temp_node1][j];
                                                    if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                                    {
                                                        ext = true;
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                                        }
                                                        for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                                        {
                                                            if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v2)
                                                            {
                                                                ext = false;
                                                                break;
                                                            }
                                                        }
                                                        if (ext)
                                                        {
                                                            for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                            {
                                                                temp_count1 = *Inc[temp_j];
                                                                if (!G_temp4[temp_count1] && G_degree[temp_count1] < temp_v2)
                                                                {
                                                                    ext = false;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                                        }
                                                        if (ext)
                                                        {
                                                            if (G_exc[temp_node2] > 0)
                                                            {
                                                                yu_exc = false;
                                                                break;
                                                            }
                                                            new_X.push_back(temp_node2);
                                                            ++temp_v2;
                                                            G_index[temp_node2] = 1;
                                                            for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                            {
                                                                G_degree[Graph[temp_node2][temp_j]]++;
                                                            }
                                                        }
                                                    }
                                                }
                                                if (G_degree[temp_node1] == temp_v2 - 1)
                                                {
                                                    temp_check2 = false;
                                                    break;
                                                }
                                            }
                                            temp_count1 = temp_check2 ? yuj : Epsilon + 1;
                                            for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2 && temp_count1 < Epsilon + 1 && yu_exc; ++temp_iter)
                                            {
                                                ++temp_count1;
                                                temp_node1 = *(temp_iter);
                                                for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                                                {
                                                    temp_node2 = Graph[temp_node1][temp_i];
                                                    if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                                    {
                                                        ext = true;
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                                        }
                                                        for (temp_iter1 = cons_Y.begin(); temp_iter1 != temp_iter2; ++temp_iter1)
                                                        {
                                                            if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v2)
                                                            {
                                                                ext = false;
                                                                break;
                                                            }
                                                        }
                                                        if (ext)
                                                        {
                                                            for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                            {
                                                                temp_count2 = *Inc[temp_j];
                                                                if (!G_temp4[temp_count2] && G_degree[temp_count2] < temp_v2)
                                                                {
                                                                    ext = false;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                                        }
                                                        if (ext)
                                                        {
                                                            if (G_exc[temp_node2] > 0)
                                                            {
                                                                yu_exc = false;
                                                                break;
                                                            }
                                                            ++temp_v2;
                                                            new_X.push_back(temp_node2);
                                                            G_index[temp_node2] = 1;
                                                            for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                            {
                                                                G_degree[Graph[temp_node2][temp_j]]++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            temp_check2 = true;
                                            for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                if (G_degree[*temp_iter] == temp_v2 - 1)
                                                {
                                                    temp_check2 = false;
                                                    temp_node1 = *temp_iter;
                                                    break;
                                                }
                                            }
                                            for (temp_j = 0; temp_j < yuj; ++temp_j)
                                            {
                                                if (G_degree[*Inc[temp_j]] == temp_v2 - 1)
                                                {
                                                    temp_check2 = false;
                                                    temp_node1 = *Inc[temp_j];
                                                    break;
                                                }
                                            }
                                            if (!temp_check2)
                                            {
                                                for (temp_i = 0; temp_i < degree[temp_node1]; ++temp_i)
                                                {
                                                    temp_node2 = Graph[temp_node1][temp_i];
                                                    if (temp_node2 != povit && G_index[temp_node2] == 0 && G_degree[temp_node2] >= temp_v1)
                                                    {
                                                        ext = true;
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 1;
                                                        }
                                                        for (temp_iter1 = cons_Y.begin(); temp_iter1 != temp_iter2; ++temp_iter1)
                                                        {
                                                            if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v2)
                                                            {
                                                                ext = false;
                                                                break;
                                                            }
                                                        }
                                                        if (ext)
                                                        {
                                                            for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                            {
                                                                temp_count2 = *Inc[temp_j];
                                                                if (!G_temp4[temp_count2] && G_degree[temp_count2] < temp_v2)
                                                                {
                                                                    ext = false;
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        for (temp_yu = degree[temp_node2] - 1; temp_yu >= 0; --temp_yu)
                                                        {
                                                            G_temp4[Graph[temp_node2][temp_yu]] = 0;
                                                        }
                                                        if (ext)
                                                        {
                                                            if (G_exc[temp_node2] > 0)
                                                            {
                                                                yu_exc = false;
                                                                break;
                                                            }
                                                            ++temp_v2;
                                                            new_X.push_back(temp_node2);
                                                            G_index[temp_node2] = 1;
                                                            for (temp_j = degree[temp_node2] - 1; temp_j >= 0; --temp_j)
                                                            {
                                                                G_degree[Graph[temp_node2][temp_j]]++;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            for (temp_node1 = 0; temp_node1 < Bipartite_index && temp_check2; ++temp_node1)
                                            {
                                                if (temp_node1 != povit && G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v1)
                                                {
                                                    ext = true;
                                                    for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                                    {
                                                        G_temp4[Graph[temp_node1][temp_yu]] = 1;
                                                    }
                                                    for (temp_iter = cons_Y.begin(); temp_iter != temp_iter2; ++temp_iter)
                                                    {
                                                        if (!G_temp4[*temp_iter] && G_degree[*temp_iter] < temp_v2)
                                                        {
                                                            ext = false;
                                                            break;
                                                        }
                                                    }
                                                    if (ext)
                                                    {
                                                        for (temp_j = 0; temp_j < yuj; ++temp_j)
                                                        {
                                                            temp_count1 = *Inc[temp_j];
                                                            if (!G_temp4[temp_count1] && G_degree[temp_count1] < temp_v2)
                                                            {
                                                                ext = false;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                                                    {
                                                        G_temp4[Graph[temp_node1][temp_yu]] = 0;
                                                    }
                                                    if (ext)
                                                    {
                                                        if (G_exc[temp_node1])
                                                        {
                                                            yu_exc = false;
                                                            break;
                                                        }
                                                        ++temp_v2;
                                                        new_X.push_back(temp_node1);
                                                        G_index[temp_node1] = 1;
                                                        for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                                        {
                                                            G_degree[Graph[temp_node1][temp_j]]++;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if (yu_exc)
                                        {
                                            F_Y.clear();
                                            X_a.clear();
                                            for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                X_a.push_back(*temp_iter);
                                            }
                                            for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                X_a.push_back(*temp_iter);
                                            }
                                            for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                X_a.push_back(*temp_iter);
                                            }
                                            for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                X_a.push_back(*temp_iter);
                                            }

                                            for (temp_iter = cons_Y.begin(), temp_iter2 = cons_Y.end(); temp_iter != temp_iter2; ++temp_iter)
                                            {
                                                F_Y.push_back(*temp_iter);
                                            }
                                            for (temp_i = 0; temp_i < yuj; ++temp_i)
                                            {
                                                F_Y.push_back(*Inc[temp_i]);
                                            }
                                            string temp_res;
                                            TransToString2(X_a, F_Y, temp_res);
                                            if (Btree.find(temp_res) == Btree.end())
                                            {
                                                if (output % 2 == 0)
                                                {
                                                    ++res_count;
                                                    if(OutputResults){
                                                        cout<<temp_res<<endl;
                                                    }
                                                    if (res_count >= total_num)
                                                    {
                                                        return;
                                                    }
                                                }
                                                Btree.insert(temp_res);
                                                new_FIX_X.clear();
                                                new_new_X.clear();

                                                for (temp_iter = X.begin(), temp_iter2 = X.end(); temp_iter != temp_iter2; ++temp_iter)
                                                {
                                                    new_new_X.push_back(*temp_iter);
                                                }
                                                for (temp_iter = new_X.begin(), temp_iter2 = new_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                                {
                                                    new_new_X.push_back(*temp_iter);
                                                }
                                                for (temp_iter = FIX_X.begin(), temp_iter2 = FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                                {
                                                    new_FIX_X.push_back(*temp_iter);
                                                }
                                                for (temp_iter = temp_FIX_X.begin(), temp_iter2 = temp_FIX_X.end(); temp_iter != temp_iter2; ++temp_iter)
                                                {
                                                    new_FIX_X.push_back(*temp_iter);
                                                }

                                                Enumerate(new_new_X, F_Y, Full_Y, new_FIX_X, depth + 1, output + 1);
                                                if (res_count >= total_num)
                                                {
                                                    return;
                                                }
                                                if (output % 2)
                                                {

                                                    ++res_count;
                                                    if(OutputResults) cout<<temp_res<<endl;
                                                    if(res_count>=total_num) return;
                                                }
                                            }
                                        }
                                    }
                                    for (temp_iter = new_X.begin(), temp_iter1 = new_X.end(); temp_iter != temp_iter1; ++temp_iter)
                                    {
                                        temp_node1 = *temp_iter;
                                        if (temp_node1 != povit)
                                        {
                                            G_index[temp_node1] = 0;
                                            for (temp_i = degree[temp_node1] - 1; temp_i >= 0; --temp_i)
                                            {
                                                G_degree[Graph[temp_node1][temp_i]]--;
                                            }
                                        }
                                    }
                                    for (temp_i = 0; temp_i < yuj; ++temp_i)
                                    {
                                        G_index[*Inc[temp_i]] = 0;
                                    }
                                    
                                }
                                for (temp_i = 0; temp_i < k; ++temp_i)
                                {
                                    temp_node1 = *fix[temp_i];
                                    for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                                    {
                                        G_degree[Graph[temp_node1][temp_j]]++;
                                    }
                                    G_index[temp_node1] = 1;
                                    X.push_back(temp_node1);
                                }
                                for (temp_iter = temp_FIX_X.begin(), temp_iter1 = temp_FIX_X.end(); temp_iter != temp_iter1; ++temp_iter)
                                {
                                    G_index[*temp_iter] = 0;
                                }
                                temp_FIX_X.clear();
                                redo2 = FindNext(NB, fix, k);
                            }
                        }
                    }
                    for (temp_i = yuj - 1; temp_i >= 0; --temp_i)
                    {
                        temp_node1 = *Inc[temp_i];
                        for (temp_j = degree[temp_node1] - 1; temp_j >= 0; --temp_j)
                        {
                            G_degree[Graph[temp_node1][temp_j]]--;
                        }
                    }
                    for (temp_iter = temp_FIX_X.begin(), temp_iter1 = temp_FIX_X.end(); temp_iter != temp_iter1; ++temp_iter)
                    {
                        G_index[*temp_iter] = 0;
                    }
                    for (temp_i = 0; temp_i < yuj; ++temp_i)
                    {
                        G_index[*Inc[temp_i]] = 0;
                    }
                }
                redo = FindNext(opt_Y, Inc, yuj);
            }
        }
        //recovery
        G_index[povit] = 0;
        for (temp_i = degree[povit] - 1; temp_i >= 0; --temp_i)
        {
            G_degree[Graph[povit][temp_i]]--;
        }
        for (temp_iter = opt_Y.begin(), temp_iter1 = opt_Y.end(); temp_iter != temp_iter1; ++temp_iter)
        {
            for (temp_i = degree[*temp_iter] - 1; temp_i >= 0; --temp_i)
            {
                G_degree[Graph[*temp_iter][temp_i]]++;
            }
            G_index[*temp_iter] = 1;
        }
        if (G_exc[povit] == 0)
            G_exc[povit] = depth;
    }
    for (temp_i = Bipartite_index - 1; temp_i >= 0; --temp_i)
    {
        if (G_exc[temp_i] == depth)
            G_exc[temp_i] = 0;
    }
}

bool iTraversal::checkY(vector<int> &X, int yuj, vector<int> &cons_Y, vector<int>::iterator Inc[], vector<int> &FIX_X)
{
    temp_check1 = true;
    temp_check2 = true;
    temp_v1 = (int)(X.size());
    temp_v2 = (int)(cons_Y.size());
    temp_iter3 = X.end();
    temp_iter2 = FIX_X.end();

    for (temp_i = 0; temp_i < yuj; ++temp_i)
    {
        G_temp3[*Inc[temp_i]] = 1;
    }

    if (temp_v1 >= Epsilon + 1)
    {
        temp_count1 = 0;
        for (temp_iter = X.begin(); temp_iter != temp_iter3 && temp_count1 < Epsilon + 1; ++temp_iter)
        {
            ++temp_count1;
            for (temp_i = 0; temp_i < degree[*temp_iter]; ++temp_i)
            {
                
                temp_node1 = Graph[*temp_iter][temp_i];
                if (G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v1 - Epsilon && !G_temp3[temp_node1])
                {
                    temp_check1 = false;
                    for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                    {
                        G_temp4[Graph[temp_node1][temp_yu]] = 1;
                    }
                    for (temp_iter1 = X.begin(); temp_iter1 != temp_iter3; ++temp_iter1)
                    {
                        if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v2 + yuj - Epsilon + 1)
                        {
                            temp_check1 = true;
                            break;
                        }
                    }
                    if (!temp_check1 && FIX_X.size() != 0)
                    {
                        temp_count2 = 0;
                        for (temp_iter1 = FIX_X.begin(); temp_iter1 != temp_iter2; ++temp_iter1)
                        {
                            if (!G_temp4[*temp_iter1])
                            {
                                temp_count2++;
                            }
                        }
                        if (G_degree[temp_node1] < temp_v1 - Epsilon + temp_count2)
                        {
                            temp_check1 = true;
                        }
                    }
                    for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                    {
                        G_temp4[Graph[temp_node1][temp_yu]] = 0;
                    }
                    if (!temp_check1)
                    {
                        break;
                    }
                }
            }
            if (!temp_check1)
                break;
        }
    }
    else
    {
        for (temp_node1 = Bipartite_index; temp_node1 < Graph_size; ++temp_node1)
        {
            if (G_index[temp_node1] == 0 && G_degree[temp_node1] >= temp_v1 - Epsilon && !G_temp3[temp_node1])
            {
                temp_check1 = false;
                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                {
                    G_temp4[Graph[temp_node1][temp_yu]] = 1;
                }
                for (temp_iter1 = X.begin(); temp_iter1 != temp_iter3; ++temp_iter1)
                {
                    if (!G_temp4[*temp_iter1] && G_degree[*temp_iter1] < temp_v2 + yuj - Epsilon + 1)
                    {
                        temp_check1 = true;
                        break;
                    }
                }
                if (!temp_check1 && FIX_X.size() != 0)
                {
                    temp_count2 = 0;
                    for (temp_iter1 = FIX_X.begin(); temp_iter1 != temp_iter2; ++temp_iter1)
                    {
                        if (!G_temp4[*temp_iter1])
                        {
                            ++temp_count2;
                        }
                    }
                    if (G_degree[temp_node1] < temp_v1 - Epsilon + temp_count2)
                    {
                        temp_check1 = true;
                    }
                }
                for (temp_yu = degree[temp_node1] - 1; temp_yu >= 0; --temp_yu)
                {
                    G_temp4[Graph[temp_node1][temp_yu]] = 0;
                }
                if (!temp_check1)
                {
                    break;
                }
            }
        }
    }

    for (temp_i = 0; temp_i < yuj; ++temp_i)
    {
        G_temp3[*Inc[temp_i]] = 0;
    }
    return temp_check1;
}
