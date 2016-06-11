/********************************************************
*以下为c++自实现的kmeans算法,随机初始化k个中心,支持余弦和
*欧式距离
*********************************************************/
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<string>
#include<assert.h>
#include<stdlib.h>
#include<time.h> 
int const N = 1000;
int const eps = 1e-3;
using namespace std;
typedef double dtype;
typedef vector<int>::size_type vt;
double disfun(const vector<dtype> &a,const vector<dtype> &b,string distype)
{
	double dis_sum=0;
	if(distype=="sqeuclidean")
	{
		for(vt i=0;i<a.size();++i)	
			dis_sum+=pow(a[i]-b[i],2);
	}
	if(distype=="cosine")
	{
		double len_a=0,len_b=0,in_product=0;
		for(vt i=0;i<a.size();++i)
		{
			len_a+=pow(a[i],2);
			len_b+=pow(b[i],2);
			in_product+=a[i]*b[i];
		}	
		dis_sum = 1-in_product*1.0/(sqrt(len_a)*sqrt(len_b));		
	}
	if(distype=="hamming")
	{
		int count=0;
		for(vt i=0;i<a.size();++i)
		{
			if(a[i]!=b[i])count++;	
		}
		dis_sum = count*1.0/a.size();
	}
	return dis_sum;
}
void ReservoirSampling()
{
	
}
vector<int> random_chooseK(const int n,int k)
{
	map<int,int> mvis;
	vector<int> vec;
	int i=0;
	cout<<"n:"<<n<<"k:"<<k<<endl;
	cout<<"random choose K ok"<<endl;
	srand((unsigned)time(NULL));
	while(i<k)
	{
		int tmp=rand()%(n);
		if(!mvis.count(tmp)){
			mvis[tmp]=1;
			vec.push_back(tmp);
			i++;
		}		
	}
	for(int &i:vec)cout<<i<<"->";
	cout<<"random choose K over"<<endl;
	return vec;		
} 
bool nochange(const vector<vector<dtype> > &kcenter,const vector<vector<dtype> > &prekcenter)
{
	for(int i=0;i<kcenter.size();++i)
	{
		string st("sqeuclidean");
		if(disfun(kcenter[i],prekcenter[i],st)>eps)return false;
	}	
	return true;
}
vector<dtype> sum_vector(const vector<dtype> &a,const vector<dtype> &b)
{
	vector<dtype> avec;
	assert(a.size()==b.size());
	for(int i=0;i<a.size();++i)
	{
		avec.push_back(a[i]+b[i]);
	}
    return avec;
}
void setZero(vector<dtype> &vec)
{
	for(int i=0;i<vec.size();++i)
	{
		vec[i]=0;
	}	
}
void AlterKcenter(const vector<vector<dtype> > &d,vector<vector<dtype> > &kcenter,const vector<vector<int> > &kset,string distype)
{
	for(int i=0;i<kcenter.size();++i)
	{
		setZero(kcenter[i]);
		cout<<i<<":"<<kset[i].size()<<endl;
		for(int j=0;j<kset[i].size();++j)
		{
			kcenter[i] = sum_vector(d[kset[i][j]],kcenter[i]);	
		}
		for(int p=0;p<kcenter[i].size();++p)
		{
			if(distype=="hamming")
				kcenter[i][p]=int(kcenter[i][p]*1.0/kset[i].size()+0.5);
			else kcenter[i][p]=kcenter[i][p]*1.0/kset[i].size();
		}
	}
}
void Kmeans(const vector<vector<dtype> > &d,int k,string distype,vector<vector<int> > &kset,map<int,int> &category,vector<vector<dtype> > &kcenter)
{
	vector<int> kgram=random_chooseK(d.size(),k);
	vector<vector<dtype> > prekcenter;
	//vector<vector<int> > kset; 
	for(int i=0;i<k;++i){
		vector<int> vitmp;
		kset.push_back(vitmp);
	}
	cout<<"kset"<<endl;
	for(vt i=0;i<kgram.size();++i)kcenter.push_back(d[kgram[i]]);
	cout<<"kset over"<<endl;
	bool flag =false;
	int count=0;
	while(!flag)
	{
		for(int x=0;x<k;++x)kset[x].clear();
		for(int j=0;j<d.size();++j)
		{
			double min_dis= 1e308;
			int index = -1;
			for(int m =0;m <k;++m)
			{
				double dis=disfun(d[j],kcenter[m],distype);
				if(dis<min_dis)
				{
					min_dis=dis;
					index = m;
				}
			}
			kset[index].push_back(j);
			category[j]=index;	
		}
		prekcenter = kcenter;
		count++;
		cout<<"alterkcenter begin"<<endl; 
		AlterKcenter(d,kcenter,kset,distype);
		cout<<"alterkcenter finished"<<endl; 
		flag = nochange(kcenter,prekcenter);
		cout<<"µÚ"<<count<<"´Îµü´ú"<<endl; 
	}						
}
void printmatrix(const vector<vector<dtype> > &matrix)
{
	for(int i=0;i<matrix.size();++i)
	{
		for(int j=0;j<matrix[i].size();++j)
			cout<<matrix[i][j]<<"  ";
		cout<<endl; 	
	}
}
double SSE(const vector<vector<dtype> > &matrix,const vector<vector<dtype> > &kcenter,map<int,int> &category,string distype,map<int,double> &EverySSE)
{
	double TotalSSE=0;
	//map<int,double> EverySSE;
	for(int i=0;i<matrix.size();++i)
	{
		double tmpsse=0;
		tmpsse=disfun(matrix[i],kcenter[category[i]],distype);
		if(EverySSE.count(category[i]))
			EverySSE[category[i]]+=tmpsse;
		else EverySSE[category[i]]=tmpsse;
		TotalSSE+=tmpsse;
	}	
	return TotalSSE;			
}
void saveresult(const vector<vector<dtype> > &matrix,vector<vector<int> > &kset)
{
	fstream f("result.txt",ios::out);
	map<int,int> category;
	cout<<"kset size:"<<kset.size()<<endl;
	f<<"total:"<<endl;
	for(int i=0;i<kset.size();++i){
		f<<"µÚ"<<i<<"Àà:"<<kset[i].size()<<endl; 
	}
	for(int i=0;i<kset.size();++i)
	{
		cout<<kset[i].size()<<endl;
		for(int j=0;j<kset[i].size();++j)
			category[kset[i][j]]=i;
	}
	for(int i=0;i<matrix.size();++i)
	{
		f<<i<<": "<<category[i]<<endl; 
	} 
	for(int i=0;i<matrix.size();++i)
	{
		f<<"----------------------------------"<<category[i]<<"---------------------------------"<<endl;
		for(int j=0;j<matrix[i].size();++j)
			f<<matrix[i][j]<<"  ";
		f<<endl; 	
	}
	f.close();
	
}
int main()
{
	vector<vector<dtype> > matrix;
	int all=0;
	srand((unsigned)time(0));
	while(all<600)
	{
		vector<dtype> row;
		for(int n=1;n<=600;++n)
		{
			int tmp=rand()%(2);
			if(rand()/double(RAND_MAX)>0.6)tmp=0;
			//cout<<tmp<<endl;
			row.push_back(tmp);	
		}
		matrix.push_back(row);
		all++;
	}
	//printmatrix(matrix);
	vector<vector<int> > kset;
	vector<vector<dtype> > kcenter;
	map<int,int> category;
	string st("hamming");
	Kmeans(matrix,30,st,kset,category,kcenter);
	saveresult(matrix,kset);
	map<int,double> EverySSE;
	cout<<SSE(matrix,kcenter,category,st,EverySSE)<<endl;
	map<int,double>:: const_iterator it=EverySSE.begin();
	while(it!=EverySSE.end()){
		cout<<it->first<<":"<<it->second<<endl; 
		it++;
	}
	system("pause");
	return 0;
}
