#include<iostream>
#include<map>
#include<fstream> 
#include<string>
#include<vector>
#include<cstdlib>
#include<time.h>
using namespace std;


typedef struct seq{
	int support;
	string item;
	vector<string> child;
	struct seq* parent;
} sequence;


vector<sequence> extendPTree(vector<sequence> & orilevel){
	vector<sequence> nextlevel;
	for(int i=0;i<orilevel.size();i++){
		vector<string> sibling=orilevel[i].parent->child;
		for(int j=0;j<sibling.size();j++){
				string newstr=orilevel[i].item;
				string s=sibling[j];
				int lastpos=s.length()-1;
				char ss=s[lastpos];
				newstr.append(1, ss);
				orilevel[i].child.push_back(newstr);
				sequence tmp;
				tmp.item=newstr;
				tmp.support=0;
				tmp.parent=& orilevel[i];
			    nextlevel.push_back(tmp);
		}
	}
	return nextlevel;
}



bool belongto(string str, string substr){
	int size2=substr.length();
	int pos=0;
	for(int i=0;i<size2;i++){
		char b=substr[i];
		if(pos>=str.length()) return false;
		int iffind=str.find(b,pos);
		if(iffind==string::npos) return false;
		else pos=iffind+1;
	}
	if(pos<=str.length()) return true;
	else return false;
}


void ComputeSupport(vector<sequence> & C,vector<string> a)
{
	for(int i=0;i<a.size();i++){
		string line=a[i];
		for(int j=0;j<C.size();j++){
			if(belongto(line,C[j].item)){
				C[j].support=C[j].support+1;
			}
		}
	}
}



vector<sequence> GSP(vector<string> a, string symbols,int minsup)
{
	vector<sequence> f;
	map<int, vector<sequence> > pstree;
	vector<sequence> c1;
	sequence root;
	root.item="0";
	root.support=a.size();
	for(int i=0;i<symbols.length();i++){
		 sequence tmp;
		 tmp.item=symbols[i];
		 tmp.support=0;
		 tmp.parent=&root;
		 root.child.push_back(tmp.item);
		 c1.push_back(tmp);
	}
	int k=1;
	pstree[1]=c1;
	int j;
	map<int,vector<sequence> >::iterator it=pstree.find(k);
	while(!(it->second).empty()){
		ComputeSupport ((it->second), a);
		string pre_parent=(it->second).at(0).parent->item;
		int left=0;
		for(j=0;j<(it->second).size(); j++){
			 sequence & tmpseq=(it->second).at(j);
			 if(pre_parent!=tmpseq.parent->item){
			 	left=j;
			 	pre_parent=tmpseq.parent->item;
			 }
			 if(tmpseq.support>=minsup){
			 	f.push_back(tmpseq);
			 } 
			 else{
				vector<sequence> ::iterator  k=(it->second).begin()+j;
				vector<string> ::iterator    t=(it->second).at(j).parent->child.begin()+j-left;
				t=(it->second).at(j).parent->child.erase(t);
			  	k=(it->second).erase(k);
			  	j--;

			 }
		}
		if((it->second).empty()) break;
	    vector<sequence> new_subtree=extendPTree(it->second);
		k++;
		pstree[k]=new_subtree;
		it=pstree.find(k);
	}
	return f;
}


int main(int argc, char *argv[])
{
	
	ifstream inputfile;
	char* file="sequencedb.txt";
	inputfile.open(file);
	string line;
	int minsup=atoi(argv[1]);
	vector<string> itemset;
	int j=0;
	while(getline(inputfile,line)){
		if(line[0]!='>'){
			itemset.push_back(line);
			j++;
		}
	}
	inputfile.close();
	string symbols="ACGT";


	clock_t t=clock();
    vector<sequence> frequent_seq=GSP(itemset,symbols,minsup);
    t=clock()-t;
    ofstream outfile;
    outfile.open("output.txt");

	for(int i=0;i<frequent_seq.size();i++)
	{
		outfile<<frequent_seq[i].item<<" - "<<frequent_seq[i].support<<endl;
	}
	//cout<<(float)t/CLOCKS_PER_SEC<<"s"<<endl;
	outfile.close();
	return 0;
}




