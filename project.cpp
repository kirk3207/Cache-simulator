#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

int address[32]={0};
int mem[1024][10]={0}; //0:Valid 1:TAG 2:TAG2 3:TAG3 4:TAG4
int allocation,tag;    //5:TAG use times 6:TAG2 use times 7:TAG3 ... 9:all miss flag
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
    int least_used=1;
    cacheLocation<<"cacheF.org";
    referenceLocation<<"InstReference_matrix.lst";
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
    fout<<"Associativity: "<<associat<<endl;
    fout<<"Block size: "<<block_size<<endl<<endl;
    fout<<"Indexing bit count: "<<block_Bit<<endl;
    fout<<"Indexing bits: ";
    for(int i=0;i<block_Bit;i++)
    {
        fout<<31-tag_Bit-i<<" ";
    }
    fout<<endl;
    fout<<"Offset bit count: "<<word_Bit<<endl<<endl;
    fout<<".benchmark datarealup"<<endl;
    Reference.open(referenceLocation.str());

    while(getline(Reference,s))
    {
        allocation=0;
        tag=0;
        if(s==".benchmark datarealup");
        else if(s==".end")break;
        else
        {
            for(int i=31;i>=0;i--)
            {
                address[31-i]=s[i]-48;
                //fout<<address[i];
            }
            for(int i=31;i>=0;i--)
            {
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
                //cout<<tag<<" "<<allocation<<" "<<"miss"<<endl;
                fout<<"miss"<<endl;
            }
            else
            {
                if(associat==1)
                {
                    if(tag==mem[allocation][1])
                    {
                        fout<<"hit"<<endl;
                        //cout<<tag<<" "<<allocation<<" "<<"hit"<<endl;
                    }
                    else
                    {
                        mem[allocation][1]=tag;
                        missCount++;
                        //cout<<tag<<" "<<allocation<<" "<<"miss"<<endl;
                        fout<<"miss"<<endl;
                    }

                }
                else if(associat==2)
                {
                    for(int i=1;i<=2;i++)
                    {
                        if(tag==mem[allocation][i])
                        {
                            mem[allocation][9]=0;
                            mem[allocation][i+4]+=1;
                            fout<<"hit"<<endl;
                            break;
                        }
                        mem[allocation][9]=1;
                    }

                    if(mem[allocation][9]==1)
                    {
                        if(mem[allocation][5]<=mem[allocation][6])
                        {
                            mem[allocation][1]=tag;
                            mem[allocation][5]=0;
                        }
                        else
                        {
                            mem[allocation][2]=tag;
                            mem[allocation][6]=0;
                        }
                        missCount++;
                        fout<<"miss"<<endl;
                    }
                }
                else if(associat==4)
                {
                    for(int i=1;i<=4;i++)
                    {
                        if(tag==mem[allocation][i])
                        {
                            mem[allocation][9]=0;
                            mem[allocation][i+4]+=1;
                            fout<<"hit"<<endl;
                            break;
                        }
                        mem[allocation][9]=1;
                    }

                    if(mem[allocation][9]==1)
                    {
                        for(int i=5;i<=8;i++)
                        {
                            if(least_used>mem[allocation][i])
                            {
                                least_used=i;
                            }
                        }
                        mem[allocation][least_used-4]=tag;
                        mem[allocation][least_used]=0;
                        missCount++;
                        fout<<"miss"<<endl;
                    }
                }
            }
            //mem[allocation][0]=1;
            //mem[allocation][1]=tag;
        }
    }
    fout<<".end"<<endl<<endl;
    fout<<"Total cache miss count: "<<missCount<<endl;
    Reference.close();
    fout.close();
    return 0;
}
