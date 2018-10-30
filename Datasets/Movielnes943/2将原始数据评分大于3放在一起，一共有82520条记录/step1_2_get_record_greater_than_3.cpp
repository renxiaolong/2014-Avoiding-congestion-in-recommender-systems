// step1_2_get_record_greater_than_3.cpp : 定义控制台应用程序的入口点。
//对最原始数据进行处理，挑选出其中评分大于3的记录

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

const string filename = "mov.dat";
const string outname = "mov_score345.dat";
//const int record_num = ;

void data_divide()
{
		ifstream rdfile(filename);
		if (!rdfile){cout<<"error read file!"; system("pause"); }

		ofstream wtfile(outname);
		if (!wtfile){cout<<"error create output file!"; system("pause"); }
		
		int uid=0, oid=0, score=0, timestamps=0;
		int record_num = 0;
		while(rdfile>>uid>>oid>>score>>timestamps)
		{
				if (score>2)
				{
						record_num ++;
						wtfile<<uid<<","<<oid<<"\n";
				}
		}
		cout<<"record_num is : "<<record_num<<endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
		data_divide();
		system("pause");
		return 0;
}