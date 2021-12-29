# SIGMOD22-MBPE
Our code, data and additional materials are avaliable here.
## Index  
```shell
.
|- README.md
|- Code
|  | - ...
|
|- Supplemental_Materials
|  |- Additional_Experimental_Results.pdf
|  |- Additional_proofs.pdf
```


# iTraversal
An efficient algorithm for enumerating large maximal k-biplexes

## Setup
```shell
g++ main.cpp -O3 -o iTraversal
```

## Usage
  ./iTraversal {OPTIONS}

    iTraversal, an efficient algorithm for enumerating all Maximal k-BiPlexes (MBPs)

  OPTIONS:

      -h, --help                          Display this help menu
      -f[dataset], --file=[dataset]       Path to dataset
      -k[para k], --k=[para k]            The parameter k
      -r[Num of MBPs] --r=[Num of MBPs]   The upper bound of the number of returning MBPs 
      -q[quiete], --q=[quiete]            quiete or not

## Input Graph Format
The input graph  should follow the following format.

 Example.graph

    3 1 2
    0 1 2
    1 0
    2 0
    (File ends with an empty line)

(1) Given an input graph G=(L,R), vertices are represented by non-negtive integers from 0 to |V(G)|. By default, {0,1,...,|L|-1} denotes the left side and {|L|,|L|+1,...,|L|+|R|-1} denotes the right side. 

(2) The first line includes 3 non-neigtive integers, e.g., 3 1 2, that denote the number of vertices, the id of the first vertex in the right side and the number of edges, respectively. To illustrate, consider the first line 3 2 1. The input graph has three vertices {0, 1, 2}, and two edges (0, 1) and (0, 2). {0} denotes the left side and {1,2} denotes the right sie.

(3) The following lines represent an adjacent list of the input graph. To illustrate, consider the second line 0 1 2. The vertex with id 0 is adjacent with two vertices 1 and 2.

## Output Format
By default, iTraversal only returns the number of found MBPs in the input graph and the corresponding running time.

    Running Time: 1ms
    Total Num: 1000

To see the details, you can choose option "-q 1", then it would output all results to the console as the following format.  

    0&1&2&3
    0&1&2&4
    . . .
    . . .
    Running Time: 1ms
    Total Num: 1000
Note that each line denotes a MBP. To illusrate, consider the first line "0&1&2&3". It denotes a large MB G[{0,1,2,3}].


## Running Example

```shell
> g++ main.cpp -O3 -o iTraversal
>
> ./iTraversal -f "./Example.graph" -k 1 -q 1
0&1&2&4&5&6&7&
0&1&2&3&4&6&7&
0&1&2&3&5&6&7&
Running Time: 0ms
Total Num: 3
>
> cat Example.graph
8 4 13
0 4 5 6 7
1 4 5 6
2 4 5 6 7
3 6 7
4 0 1 2
5 0 1 2
6 0 1 2 3
7 0 2 3

>
>
```