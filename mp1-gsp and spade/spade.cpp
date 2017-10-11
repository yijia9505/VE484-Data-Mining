#include<iostream>
#include<map>
#include<fstream> 
#include<sstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<time.h>
using namespace std;



typedef struct seq{
	int support;
	string item;
	map<int, vector<string> > pos;
} sequence;


bool FindPos(string str, char symbol, vector<string> & pos)
{
	for(int i=0;i<str.length();i++){
		if (str[i]==symbol){
			stringstream ss;
			ss<<i;
			string s=ss.str();
			pos.push_back(s);
		}	
	}	
	if(pos.empty()) return false;
	else return true;
}

void ComputeSymbols(sequence & C,vector<string> a)
{
	//cout<<C.item<<"~~~~~"<<endl;
	for(int i=0;i<a.size();i++){
		string line=a[i];
		vector<string> pos;
		char ch=C.item[0];
		if(FindPos(line,ch,pos)){
			 C.pos[i]=pos;
			 C.support=C.support+1;
			 //cout<<i<<": ";
			 //for(int j=0;j<pos.size();j++){
			 	//cout<<pos[j]<<" ";
			 //}
			 //cout<<endl;
		}	
	}
}



void ComputeSpade(sequence a, sequence b, sequence & ab){
		map<int, vector<string> > ::iterator itb;
		map<int, vector<string> > ::iterator ita;
		//cout<<ab.item<<"----------"<<endl;
		for(itb=b.pos.begin(); itb != b.pos.end(); itb++){
			int seqNum=itb->first;
			vector<string> tmpb=itb->second;
			vector<string> tmpab;
			ita=a.pos.find(seqNum);
			if(ita==a.pos.end()) continue;
			string l=(ita->second).at(0);
			long least_a=strtol(l.c_str(),NULL,10);
			vector<string>:: iterator itline;
			//cout<<b.item<<" "<<a.item<<endl;
			for(itline=tmpb.begin(); itline!=tmpb.end();itline++){
				string t=*itline;
				long position=strtol(t.c_str(),NULL,10);
				if(position>least_a){
					tmpab.insert(tmpab.begin(),itline,tmpb.end());
					break;
				}
			}
			//if(>least_a) cout<<seqNum<<": 1"<<endl;
			if(!tmpab.empty()){
				ab.pos[seqNum]=tmpab; 
				ab.support=ab.support+1;
			}
		}
}



void spade(vector<sequence> P, int minsup,vector<sequence> & f, int k)
{
	for(int i=0;i<P.size();i++){
		f.push_back(P[i]);
		vector<sequence> Pa;
		for(int j=0;j<P.size();j++){
			sequence new_seq;
			string a=P[i].item;
			string b=P[j].item;
			char bk=b[k];
			a.append(1,bk);
			new_seq.item=a;
			new_seq.support=0;
			ComputeSpade(P[i], P[j], new_seq);
			//cout<<new_seq.item<<" "<<new_seq.support<<endl;
			if(new_seq.support>=minsup) Pa.push_back(new_seq);
		}
		if(!Pa.empty()){
			for(int t=0; t<Pa.size();t++){
			}
			spade(Pa,minsup,f,k+1);
		} 
	}
}




int main(int argc, char *argv[])
{
	
/* read file */
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

/* compute spade for ACGT */
	clock_t t=clock();
	string symbols="ACGT";
	vector<sequence> P;
	for(int i=0;i<symbols.length();i++){
		 sequence tmp;
		 tmp.item=symbols[i];
		 tmp.support=0;
		 ComputeSymbols(tmp,itemset);
		 if (tmp.support>=minsup) P.push_back(tmp);
	}

	vector<sequence> frequent_seq;
	int k=0;
/*spade*/
	
    spade(P,minsup,frequent_seq,k);
    t=clock()-t;
    ofstream outfile;
    outfile.open("output.txt");
/*output*/
	for(int i=0;i<frequent_seq.size();i++)
	{
		outfile<<frequent_seq[i].item<<" - "<<frequent_seq[i].support<<endl;
	}
	outfile.close();
	//cout<<(float)t/CLOCKS_PER_SEC<<"s"<<endl;
	return 0;
}




