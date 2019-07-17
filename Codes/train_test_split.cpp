// Partition the original data set into ten train(90%)/test(10%) sets 
// input data format：user_id, object_id

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> //inlude sort()
#include <iterator>  //include back_inserter()
#include <time.h>    //inckude time(0), which is used in srand(time(0));
using namespace std;

const string filename = "Source\\rym";
const int record_num = 610398; 

/*

// movielens
const string filename = "mov";
const int record_num = 82520; 

*/

struct edge
{
	int source_id;
	int target_id;
};

void data_divide()
{
		//将原始数据读入，并存储起来
		ifstream rdfile(filename);
		if (!rdfile){cout<<"error read file!"; system("pause");}

		vector<edge>edges(0);
		edges.resize(record_num);

		int uid=0, oid=0, i=0;
		while(rdfile>>uid>>oid)
		{
				edges[i].source_id = uid;
				edges[i].target_id = oid;
				++i;
		}

		//一次次遍历存储起来的原始数据，将数据分成训练集和测试集
		for (int step=0; step<10; step++)//10次遍历
		{
				vector<edge>edge_train(0);
				vector<edge>edge_probe(0);
				edge_train.resize(record_num*9/10 + 10);
				edge_probe.resize(record_num/10 + 10);

				stringstream ss;
				string train_name="", probe_name=""; 

				int i_t=0, i_p=0;
				for (int i=0; i<int(edges.size()); i++)//每次遍历，分一次训练集和测试集
				{
						if (i%10 != step)
						{
						edge_train[i_t].source_id = edges[i].source_id;
						edge_train[i_t].target_id = edges[i].target_id;
						i_t++;
						}
						else{
						edge_probe[i_p].source_id = edges[i].source_id;
						edge_probe[i_p].target_id = edges[i].target_id;
						i_p++;
						}
				}
				cout<<"step :"<<step<<"\ttraining records: "<<i_t<<"\tprobs records: "<<i_p<<endl; //test

				//输出到文件
				ss.clear();
				probe_name = ""; train_name = "";
				ss<<filename<<"_train_part_"<<step;
				ss>>train_name;
				ss.clear();

				ss<<filename<<"_probe_part_"<<step;
				ss>>probe_name;
				ss.clear();

				ofstream wt_t(train_name);
				ofstream wt_p(probe_name);
				if (!wt_t || !wt_p) {cout<<"error 8898"; system("pause");}

				for (int i=0; i<i_t; i++)
				{
						wt_t<<edge_train[i].source_id<<"    "<<edge_train[i].target_id<<"\n";
				}
				for (int j=0; j<i_p; j++)
				{
						wt_p<<edge_probe[j].source_id<<"    "<<edge_probe[j].target_id<<"\n";
				}
				wt_t.close();
				wt_p.close();
		}

}

int _tmain(int argc, _TCHAR* argv[])
{
	data_divide();
	system("pause");
	return 0;
}

