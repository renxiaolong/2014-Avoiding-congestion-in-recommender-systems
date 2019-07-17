// Gini.cpp :
// calculate Gini Coeffcient
// input: a column of data
// output: the Gini Coefficient

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <iterator> 
#include <time.h>  
#include <Windows.h>
using namespace std;

const string SOURCE_FILE_PATH = "S\\";
const string FILE_NAME_PATH = "S\\_000";

vector<string> getFileNames()
{
		char szCommand[MAX_PATH] = {0};
		string command = "dir /a-d /b %s > " + FILE_NAME_PATH;
		wsprintfA(szCommand, command.c_str(), SOURCE_FILE_PATH.c_str());
		system(szCommand);

		ifstream rd(FILE_NAME_PATH);
		if (!rd) cout<<"error open file 3382! \n";

		vector<string> filepathes;
		string str = "";
		while( getline(rd, str) )
				if (str!="_000") filepathes.push_back(str);
		rd.close();

		return filepathes;
}

void getGini (string rdFile)
{
		vector<double> salarys (0);
		ifstream rd(rdFile);
		if (!rd) cout<<"error open source file 883! \n";

		double s_temp=0;
		while (rd>>s_temp)
				salarys.push_back(s_temp);
		rd.close();

		// step 1
		sort ( salarys.begin(), salarys.end() ); 

		//step2
		vector<double> populationgCumulate (salarys.size(),0); 
		for (int i=0; i<int(salarys.size()); i++)
				populationgCumulate[i] = i+1;

		// step 3
		vector<double> salaryCumulate ( salarys.size(),0 );
		salaryCumulate[0] = salarys[0];
		for (int i=1; i<int(salarys.size()); i++)
				salaryCumulate[i] = salaryCumulate[i-1] + salarys[i];

		// step 4 & 5
		double totalPopulation = populationgCumulate[ int(populationgCumulate.size())-1];
		double totalSalary     = salaryCumulate [ int(salaryCumulate.size())-1 ];
		for (int i=0; i<int(salarys.size()); i++)
		{
				populationgCumulate[i] /= totalPopulation;
				salaryCumulate[i] /= totalSalary;
		}

		// step 6
		vector<double> trapezoidArea ( int(salarys.size()), 0 );
		for (int i=1; i<int(salarys.size()); i++)
				trapezoidArea[i] = ( populationgCumulate[i] - populationgCumulate[i-1] ) 
						         * ( salaryCumulate[i] + salaryCumulate[i-1] ) / 2;
		// end step
		double totalArea = 0;
		for (int i=0; i<int(salarys.size()); i++)
				totalArea += trapezoidArea[i];

		cout<<rdFile<<" "<< ( 0.5 - totalArea ) / 0.5 <<"\n";

}
int _tmain(int argc, _TCHAR* argv[])
{
		vector<string> sourceFileNames = getFileNames(); // store file path
		for (int nameSub=0; nameSub<int(sourceFileNames.size()); nameSub++ )
		{
				string rdFile = SOURCE_FILE_PATH + sourceFileNames[nameSub];
				getGini(rdFile);
		}

		system("pause");
		return 0;
}

