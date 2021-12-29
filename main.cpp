#include<time.h>
#include<cstring>
#include "iTraversal.h"
#include "args.hxx"
#include"Util.h"

#define FILELEN 1024


int main(int argc, char** argv) {

	char filepath[1024] = ".........";

	int k = 1;
	int num=1000;
	int isquiete = 0;

	args::ArgumentParser parser(
        "iTraversal, an algorithm for enumerating all maximal biplexes\n");

    args::HelpFlag help(parser, "help", "Display this help menu",
                        {'h', "help"});
    args::Group required(parser, "", args::Group::Validators::All);

    args::ValueFlag<std::string> benchmark_file(
        parser, "benchmark", "Path to benchmark", {'f', "file"},
        "");

	args::ValueFlag<int> K(parser, "para k", "The parameter k", {'k', "k"}, 1);


	args::ValueFlag<int> Results(parser, "Num of results", "Num of results", {'r', "r"}, 1000);

	args::ValueFlag<int> Quiete(parser, "quiete", "quiete or not", {'q', "q"}, 0);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 0;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 0;
    }

	strncpy(filepath, args::get(benchmark_file).c_str(), FILELEN);
	k = args::get(K);
	isquiete = args::get(Quiete);
	num = args::get(Results);

	if (k<0||num<0) {
		fprintf(stderr, "k, theta and num should be at least 0\n");
		exit(-1);
	}

	if(isquiete>0){
		OutputResults=1;
	}

	Util util;
	int bi=0;
    int *degree=NULL;
    int **Graph=NULL;
    int graph_size=util.ReadGraph(filepath,Graph,degree,bi);
	


    iTraversal iminer(Graph,degree,graph_size,bi,k,num);
    time_t s1 = clock();
    iminer.miner();
    time_t s2 = clock();
    cout<<"Running Time: "<<(double)(s2-s1)<<"ms"<<endl;
	cout<<"Total Num: "<<iminer.res_count<<endl;

	return 0;
}