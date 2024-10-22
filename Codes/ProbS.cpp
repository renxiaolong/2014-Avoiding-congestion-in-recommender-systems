// step3_4_ProbS.cpp : 定义控制台应用程序的入口点。
//1.在一对确定的训练集和测试集上，用ProbS方法给每个用户所对应的所有object打分
//2.输出对于每个用户而言，所有object得到的分数，训练集中已经选择的object被赋予-1
//3.输出对于每个用户在测试集中的边

//读入的数据格式：userid itemid
//输出：每个user对应的所有item的打分数据, 
//      其中-1表示在训练集中已经选择过得item, 一行只有一个-2则表示没有针对该user计算
//输出：测试集中每个user所选择的item的id
//      一行只有一个-2则表示没有针对该user计算

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

const string TRAIN_FILE = "S\\train.csv";
const string PROBE_FILE = "S\\test.csv";
const int MAX_UID = 943;
const int MAX_IID = 1682;
const int EXISTED_ITEM_NUM = 1682; // item
const int TOPL     = 200;
const string SCORE_FILE = "R\\mov_score_part_0";  //最终可能推荐的商品中，Top L 个 item id
const string SECLC_FILE = "R\\mov_select_part_0"; //测试集中每个user所选择的item的id

/*
// netflix
const string TRAIN_FILE = "Data_source\\netflix_train_part_8";
const string PROBE_FILE = "Data_source\\netflix_probe_part_8";
const int MAX_UID = 10000;
const int MAX_IID = 6000;
const string SCORE_FILE = "Data_result\\netflix_score_part_8";
const string SECLC_FILE = "Data_result\\netflix_select_part_8";

// rym
const string TRAIN_FILE = "Data_source\\rym_train_part_4";
const string PROBE_FILE = "Data_source\\rym_probe_part_4";
const int MAX_UID = 33221;
const int MAX_IID = 5234;
const string SCORE_FILE = "Data_result\\rym_score_part_4";
const string SECLC_FILE = "Data_result\\rym_select_part_4";

*/

struct node
{
		double value;
		vector<int>ids; //id vector of connected objects or users 
};

struct edge
{
		int source_id;
		int target_id;
};

struct itemValue
{
		int itemID;
		double value;

		bool operator== (const itemValue& objstruct)const
		{
				return value  == objstruct.value;
		}
		bool operator>(const itemValue& objstruct)const
		{
				return value > objstruct.value;
		}
		bool operator<(const itemValue& objstruct)const
		{
				return value  < objstruct.value;
		}
};

void rdata(vector<node>&items, vector<node>&users, vector<node>&items_probe, vector<node>&users_probe)
{
		node i = {0};
		items.resize(MAX_IID, i);
		users.resize(MAX_UID, i);
		items_probe.resize(MAX_IID, i);
		users_probe.resize(MAX_UID, i);

		ifstream rdfile_t(TRAIN_FILE);
		ifstream rdfile_p(PROBE_FILE);
		if (!rdfile_t || !rdfile_p){cout<<"error open file!"; system("pause");}

		int uid, oid;
		while (rdfile_t>>uid>>oid) //读训练集
		{
				items[oid-1].ids.push_back(uid);
				users[uid-1].ids.push_back(oid);
		}
		while (rdfile_p>>uid>>oid) //读测试集
		{
				items_probe[oid-1].ids.push_back(uid);
				users_probe[uid-1].ids.push_back(oid);
				//user_select_p[(uid-1)*MAX_IID+oid-1] = true;
		}
		cout<<"good after read! \n";
}

void ProbS(int target_uid, vector<node>items, vector<node>users, vector<itemValue>& score)
{
		bool users_flag[MAX_UID]={0}, items_flag[MAX_IID]={0};//0表示value=0,1表示value!=0

		//allocation: items -> users
		for (int it_sub=0; it_sub<int(users[target_uid-1].ids.size()); it_sub++)
		{
				for (int u_sub=0; u_sub<int(items[users[target_uid-1].ids[it_sub]-1].ids.size()); u_sub++)
				{
						users[items[users[target_uid-1].ids[it_sub]-1].ids[u_sub]-1].value += 
								1.0 / double(items[users[target_uid-1].ids[it_sub]-1].ids.size());
				}
		}
		//allocation: users -> items
		for (int i=0; i<MAX_UID; i++)
		{
				if (users[i].value != 0)
				{
						for (int j=0; j<int(users[i].ids.size()); j++)
						{
								items[users[i].ids[j]-1].value += users[i].value / double(users[i].ids.size());
						}
				}
		}
		//store rank score
		score.resize( MAX_IID );
		for (int i=0; i<MAX_IID; i++)
		{
				score[i].itemID = i + 1;
				score[i].value = items[i].value;
		}
		for (int it_sub=0; it_sub<int(users[target_uid-1].ids.size()); it_sub++)
		{
				//score[it_sub] = -1;//去除target_uid已经选择的items的打分值
				score[users[target_uid-1].ids[it_sub]-1].value = -1;
		}
		sort ( score.begin(), score.end() );
		reverse( score.begin(), score.end() );
}
void rankScore ( int target_uid, vector<itemValue> score, vector<node> users , vector<node> users_probe, vector<double>& allRankScore)
{
		double rs = 0;
		for (int i=0; i<int(users_probe[target_uid-1].ids.size()); i++)
		{
				bool findfirst = false;
				int p=0, q=0; // start location and end location
				for (int j=0; j<int(score.size()); j++)
				{
						if ( score[j].itemID == users_probe[target_uid-1].ids[i] )
						{
								if (!findfirst) { findfirst = true; p= j+1; }
								q = j+1; 
						}
						if (findfirst && score[j].itemID != users_probe[target_uid-1].ids[i])
						{
								allRankScore.push_back( (double(p+q)/2.0) / (double(EXISTED_ITEM_NUM - double(users[target_uid-1].ids.size()) ) ) );
								break;
						}
				}
		}
}
int _tmain(int argc, _TCHAR* argv[])
{
		cout<<SCORE_FILE<<endl;
		vector<node> items(0), users(0); //训练集
		vector<node> items_probe(0), users_probe(0); //测试集

		rdata(items, users, items_probe,users_probe);

		ofstream wt_score(SCORE_FILE);
		ofstream wt_select(SECLC_FILE);
		if (!wt_score || !wt_select){cout<<"error create out put file"<<endl; system("pause");}

		for (int i=0; i<MAX_UID; i++) //输出：测试集中每个user所选择的item的id
		{
				if (int(users_probe[i].ids.size())==0) wt_select<<-2;
				else for (int j=0; j<int(users_probe[i].ids.size()); j++)
						wt_select<<users_probe[i].ids[j]<<"    ";

				wt_select<<"\n";
		}
		wt_select.close();

		vector<double> allRankScore(0);
		for (int target_uid=1; target_uid<=MAX_UID; target_uid++)
		{
				if (int(users[target_uid-1].ids.size())==0 || int(users_probe[target_uid-1].ids.size())==0)
				{
						wt_score<<-2<<"\n";	continue;
				}

				vector<itemValue> score(0);
				ProbS(target_uid, items, users, score);
				rankScore( target_uid, score, users, users_probe, allRankScore);

				//输出：每个user对应的TOPL个item的id
				for (int itemSub=0; itemSub<TOPL; itemSub++)
						wt_score<<score[itemSub].itemID<<"    ";
				wt_score<<"\n";

		}
		wt_score.close();

		// 计算average rankScore
		cout<<"the size of allRankScore is "<<allRankScore.size()<<"\n";
		double sum = 0;
		for (int i=0; i<int(allRankScore.size()); i++)
		{
				sum += allRankScore[i];
		}
		cout<<"average rank score is : "<< sum/double(allRankScore.size()) << "\n";

		system("pause");
		return 0;
}

