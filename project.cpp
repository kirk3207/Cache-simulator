#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

int address[32]={0};
int mem[1024][3]={0}; //0:Valid 1:TAG 2:Data
int allocation,tag;
int missCount=0;


int main()
{
    ifstream Cache;
    ifstream Reference;
    ofstream fout;
    stringstream cacheLocation;
    stringstream referenceLocation;
    stringstream outputLocation;
    string s;
    int addr_Bit,num_of_set,associat,block_size;
    int block_Bit,word_Bit,tag_Bit;
    cacheLocation<<"cacheA.org";
    referenceLocation<<"DataReference.lst";
    outputLocation<<"index.rpt";
    Cache.open(cacheLocation.str());
    Cache>>s>>addr_Bit;
    Cache>>s>>num_of_set;
    Cache>>s>>associat;
    Cache>>s>>block_size;
    Cache.close();

    word_Bit = log2(block_size);
    block_Bit = log2(num_of_set);
    tag_Bit = addr_Bit - word_Bit - block_Bit;

    fout.open(outputLocation.str());
    fout<<"Address bits: "<<addr_Bit<<endl;
    fout<<"Number of sets: "<<num_of_set<<endl;
    fout<<"Associaticity: "<<associat<<endl;
    fout<<"Block size: "<<block_size<<endl<<endl;
    fout<<"Indexing bit count: "<<block_Bit<<endl;
    fout<<"Indexing bits: ";
    for(int i=0;i<block_Bit;i++)
    {
        fout<<31-tag_Bit-i<<" ";
    }
    fout<<endl;
    fout<<"Offset bit count: "<<word_Bit<<endl<<endl;
    fout<<".benchmark testcase1"<<endl;
    Reference.open(referenceLocation.str());

    while(getline(Reference,s))
    {
        if(s==".benchmark datarealup");
        else if(s==".end")break;
        else
        {
            allocation=0;
            tag=0;
            for(int i=31;i>=0;i--)
            {
                address[31-i]=s[i]-48;
                fout<<address[i];
            }
            fout<<" ";
            for(int i=0;i<block_Bit;i++)
            {
                allocation+=address[i+word_Bit]*pow(2,i);
            }
            for(int i=0;i<tag_Bit;i++)
            {
                tag+=address[i+word_Bit+block_Bit]*pow(2,i);
            }
            //cout<<allocation<<" "<<tag<<" ";

            if(mem[allocation][0]==0)   //if not valid
            {
                mem[allocation][0]=1;
                mem[allocation][1]=tag;
                missCount++;
                fout<<"miss"<<endl;
            }
            else
            {
                if(tag==mem[allocation][1])
                {
                    fout<<"hit"<<endl;
                }
                else
                {
                    mem[allocation][1]=tag;
                    fout<<"miss"<<endl;
                }
            }
            mem[allocation][0]=1;
            mem[allocation][1]=tag;


        }
    }
    fout<<".end"<<endl<<endl;
    fout<<"Total cache miss count: "<<missCount<<endl;
    return 0;
}
