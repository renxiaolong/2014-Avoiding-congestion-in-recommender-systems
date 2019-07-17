// DWC.cpp
// step3_4SemiConductivityHeatS.cpp : �������̨Ӧ�ó������ڵ㡣
//1.��һ��ȷ����ѵ�����Ͳ��Լ��ϣ���HeatS������ÿ���û�����Ӧ������object���
//2.�������ÿ���û����ԣ�����object�õ��ķ�����ѵ�������Ѿ�ѡ���object������-1
//3.�������ÿ���û��ڲ��Լ��еı�
//��������ݸ�ʽ��userid itemid
//���1��ÿ��user��Ӧ������item�Ĵ������, 
//       ����-1��ʾ��ѵ�������Ѿ�ѡ�����item, һ��ֻ��һ��-2���ʾû����Ը�user����
//���2�����Լ���ÿ��user��ѡ���item��id
//       һ��ֻ��һ��-2���ʾû����Ը�user����

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

const string TRAIN_FILE = "S\\mov_train_part_0";
const string PROBE_FILE = "S\\mov_probe_part_0";
const int MAX_UID = 943;
const int MAX_IID = 1682;
const int EXISTED_ITEM_NUM = 1682; 
const string SCORE_FILE = "R\\mov_SCHeatS_rlist200_part_0";
const string SECLC_FILE = "R\\mov_select_part_0";
const int TOPL = 200;
/*
//netflix
const string TRAIN_FILE = "Data_source\\netflix_train_part_0";
const string PROBE_FILE = "Data_source\\netflix_probe_part_0";
const int MAX_UID = 10000;
const int MAX_IID = 6000;
const string SCORE_FILE = "Data_result\\netflix_score_part_0";
const string SECLC_FILE = "Data_result\\netflix_select_part_0";
*/


struct node
{
		double value;
		vector<int>ids; //id vector of connected objects
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
		while (rdfile_t>>uid>>oid) //��ѵ����
		{
				items[oid-1].ids.push_back(uid);
				users[uid-1].ids.push_back(oid);
		}
		while (rdfile_p>>uid>>oid) //�����Լ�
		{
				items_probe[oid-1].ids.push_back(uid);
				users_probe[uid-1].ids.push_back(oid);
		}
		cout<<"good after read data:"<<TRAIN_FILE<<endl;
}

void SCHeatS(int target_uid, vector<node>items, vector<node>users, vector<itemValue> &score, double exp_a)
{
		//allocation: items -> users
		bool items_flag[MAX_IID]={0};//Ϊ0��ʾû�б�target_uidѡ��
		for (int i=0; i<int(users[target_uid-1].ids.size()); i++) //������б�target_id��ѡ���items
		{
				items_flag[users[target_uid-1].ids[i]-1] = 1;
		}
		for (int u_sub=0; u_sub<MAX_UID; u_sub++) //����ÿһ��user, item -> user
		{
				bool find_source = 0;//�������ڵ��Ƿ������Դ��item����
				for (int i_sub=0; i_sub<int(users[u_sub].ids.size()); i_sub++)
				{
						if (items_flag[users[u_sub].ids[i_sub]-1])
						{
								find_source = 1;
								break;
						}
				}
				if (find_source)
				{
						//�����ĸ����
						double denominator = 0.0, numerator = 0.0;
						for (int i_sub=0; i_sub<int(users[u_sub].ids.size()); i_sub++)
						{
								denominator += pow(double(users[u_sub].ids.size())*double(items[ users[u_sub].ids[i_sub]-1 ].ids.size()) , exp_a);
								if (items_flag[users[u_sub].ids[i_sub]-1])
								{
										//����
										numerator += pow(double(users[u_sub].ids.size())*double(items[ users[u_sub].ids[i_sub]-1 ].ids.size()) , exp_a);
								}
						}
						users[u_sub].value = numerator / denominator; //����+=
				}
		}

		if (exp_a==0)
		{
				ofstream wt("R\\test");
				if (!wt) cout<<"error creat out put file! 3329";
				for (int temp_p=0; temp_p<int(users.size()); temp_p++)
				{
						wt<<users[temp_p].ids.size()<<","<<users[temp_p].value<<endl;
				}
		}
		else
		{
				ofstream wt("R\\test_1.35");
				if (!wt) cout<<"error creat out put file! 3329";
				for (int temp_p=0; temp_p<int(users.size()); temp_p++)
				{
						wt<<users[temp_p].ids.size()<<","<<users[temp_p].value<<endl;
				}
		}
		
		//allocation: users -> items
		for (int i=0; i<MAX_UID; i++)
		{
				if (users[i].value != 0)
				{
						for (int j=0; j<int(users[i].ids.size()); j++)
						{
								items[users[i].ids[j]-1].value += users[i].value / double(items[users[i].ids[j]-1].ids.size());
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
				//score[it_sub] = -1;//ȥ��target_uid�Ѿ�ѡ���items�Ĵ��ֵ
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
								allRankScore.push_back( (double(p+q)/2) / (double(EXISTED_ITEM_NUM - double(users[target_uid-1].ids.size()) ) ) );
								break;
						}
				}
		}
}

int _tmain(int argc, _TCHAR* argv[])
{
		vector<node> items(0), users(0); //ѵ����
		vector<node> items_probe(0), users_probe(0); //���Լ�

		rdata(items, users, items_probe,users_probe);

		ofstream wt_select(SECLC_FILE);
		if (!wt_select){cout<<"error create out put file"<<endl; system("pause");}

		for (int i=0; i<MAX_UID; i++) //��������Լ���ÿ��user��ѡ���item��id
		{
				if (int(users_probe[i].ids.size())==0) wt_select<<-2;
				else for (int j=0; j<int(users_probe[i].ids.size()); j++)
				{
						wt_select<<users_probe[i].ids[j]<<"    ";
				}

				wt_select<<"\n";
		}
		wt_select.close();

		for (double exp_a=0; exp_a>=-2.001; exp_a-=1.35)
		{
				//modify the output-file name
				string add_temp="";
				stringstream add_name_ss;
				add_name_ss<<exp_a;
				add_name_ss>>add_temp;
				string score_file = SCORE_FILE + "_expa_" + add_temp;

				vector<double> allRankScore(0);
				ofstream wt_score(score_file);
				if (!wt_score) cout<<"error create out-put file! \n";
				for (int target_uid=1; target_uid<=MAX_UID; target_uid++)
				{
						if (int(users[target_uid-1].ids.size())==0 || int(users_probe[target_uid-1].ids.size())==0)
						{
								wt_score<<-2<<"\n";	continue;
						}

						vector<itemValue> score(0);
						SCHeatS(target_uid, items, users, score, exp_a);
						rankScore( target_uid, score, users, users_probe, allRankScore);

						// ���target_uid�Ƽ���TOPL����Ʒ
						for (int itemSub=0; itemSub<TOPL; itemSub++)
								wt_score<<score[itemSub].itemID<<"    ";
						wt_score<<"\n";
				}
				wt_score.close();

				// ����average rankScore
				//cout<<"the size of allRankScore is "<<allRankScore.size()<<"\n";
				double sum = 0;
				for (int i=0; i<int(allRankScore.size()); i++)
				{
						sum += allRankScore[i];
				}
				cout<<"average rank score is : "<< sum/double(allRankScore.size()) << "  \t";

				cout<<"exp_a:"<<exp_a<<": ok!\n";
		}

		system("pause");
		return 0;
}


