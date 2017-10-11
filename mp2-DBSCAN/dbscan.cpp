#include<iostream>
#include<map>
#include<fstream> 
#include<sstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<cmath>
using namespace std;

typedef struct point_str{
	double x;
	double y;
	int id;
	int core_id;
}point;

typedef struct core_str{
	int i;
	int id;
	vector<int> neighbour;
}core;


double distance(int i, int j, vector<point> p_db){
	double a=pow(p_db[i].x-p_db[j].x,2);
	double b=pow(p_db[i].y-p_db[j].y,2);
	double c=sqrt(a+b);
	return c;

}


void Density_Connected(vector<point> & point_db, vector<core> & core_list,  core cpoint, int k){
	vector<int> neighbour=cpoint.neighbour;
	for (int i = 0; i < neighbour.size(); ++i)
	{
		int t=neighbour[i];
		if(t==cpoint.i) continue;
		if(point_db[t].id==k) continue;
		point_db[t].id=k;
		point p=point_db[t];
		if(p.core_id!=-1){
			core_list[p.core_id].id=k;
			core nextcore=core_list[p.core_id];
			Density_Connected(point_db,core_list,nextcore, k);
		}
	}
}


int DBSCAN(vector<point> & point_db, int minsup,double e){
	vector<core> core_list;
	int k=0;
	int i,j;
	int core_num=0;
	for (i = 0; i < point_db.size(); i++){
		vector<int> neighbour_tp;
		for (j = 0; j< point_db.size(); j++)
		{	
				if(distance(i,j,point_db)<=e){
					neighbour_tp.push_back(j);
				}
		}
		if(neighbour_tp.size()>=minsup){
			core coretp;
			coretp.i=i;
			coretp.id=-1;
			coretp.neighbour.assign(neighbour_tp.begin(),neighbour_tp.end());
			core_list.push_back(coretp);
			point_db[i].core_id=core_num;
			core_num++;
		}

	}

	for (int i = 0; i < core_list.size(); i++){
		if(core_list[i].id!=-1) continue;
		k++;
		core_list[i].id=k;
		point_db[core_list[i].i].id=k;
		Density_Connected( point_db , core_list, core_list[i] , k);
	}
	return k;
}


int main(int argc, char *argv[])
{
	
/* read file */
	ifstream inputfile;
	char* file="t7-4k.txt";
	inputfile.open(file);
	string line;
	int minsup=atoi(argv[1]);
	double e=strtod(argv[2],NULL);
	vector<point> point_db;
	int j=0;
	int num=0;
/*get point database*/
	while(getline(inputfile,line)){
		stringstream tmpstring;
		tmpstring<<line;
		point tmpoint;
		char c;
		tmpstring>>tmpoint.x>>c>>tmpoint.y;
		tmpoint.id=-1;
		tmpoint.core_id=-1;
		point_db.push_back(tmpoint);
		num++;
	}
	inputfile.close();
	clock_t t=clock();
 	int cluster_num=DBSCAN(point_db, minsup,e);
 	t=clock()-t;
 	//cout<<"total dbscan time: "<<(float)t/CLOCKS_PER_SEC<<" s"<<endl;
 	for (int i = 1; i <=cluster_num; i++)
 	{
 		cout<<"cluster "<<i<<endl;
 		int m=0;
 		for (j=0; j<point_db.size(); j++)
 		{
 			if(point_db[j].id==i){
 				m++;
 			}
 				
 		}
 		cout<<"total: "<<m<<" points in cluster "<<i<<endl;
 		cout<<endl;
 	}
 	cout<<"Noise"<<endl;
 	int noise_num=0;
 	for (j=0; j<point_db.size(); j++)
 	{
 		if(point_db[j].id==-1){
 			cout<<"{"<<point_db[j].x<<", "<<point_db[j].y<<"},"<<endl;
 			noise_num++;
 		}			
 	}
 	cout<<"total: "<<noise_num<<" noise points"<<endl;

}
