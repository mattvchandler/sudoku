#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
using namespace std;
struct Board{ int n; bool orig;};

Board **B;
int num, numx, numy;

int * factor(int n, int &len)
{
    int * sieve, * primes, * factors, k=n, j=0, l=0,m=0;
    sieve=new int[n];
    for(int i=0; i<n; i++)
        sieve[i]=i+1;
    sieve[0]=0;
    for(int j=1; j< sqrt(float(n)); j++)
    {
        if(sieve[j]!=0)
            for(int i=j+sieve[j]; i<n; i+=sieve[j])
            {
                if(sieve[i]!=0) k--;
                sieve[i]=0;
            }
    }
    k--;
    primes=new int[k];
    memset(primes,0,k);
    for(int i=0; i<n; i++)
        if(sieve[i]!=0)
        {
            int x=n;
            while(x%sieve[i]==0){l++; x/=sieve[i];}
            primes[j++]=sieve[i];
        }
    factors=new int[l];
    for(int i=0; i<j; i++)
    {
        int x=n;
        while(x%primes[i]==0)
        {
            factors[m++]=primes[i];
            x/=primes[i];
        }
    }
    len=l;
    delete [] sieve;
    delete [] primes;
    return factors;
}

void poss(int x, int y, int * possible)
{
    for(int i=0;i<num;i++)
        possible[i]=i+1;
    for(int i=0; i<num; i++)
    {
        if(B[x][i].n!=0) possible[B[x][i].n-1]=0;
        if(B[i][y].n!=0) possible[B[i][y].n-1]=0;
    }
    for(int i=(x/numx)*numx; i<(x/numx)*numx+numx; i++)
        for(int j=(y/numy)*numy; j<(y/numy)*numy+numy; j++)
            if(B[i][j].n!=0) possible[B[i][j].n-1]=0;
}

bool solve(int n)
{
    if(n>num*num-1) return true;
    int x=n%num, y=n/num;
    if(B[x][y].orig) return solve(n+1);
    int *possible;
    possible=new int[num];
    poss(x,y,possible);
    for(int i=0; i<num; i++)
    {
        if(possible[i]==0) continue;
        B[x][y].n=i+1;
        if(solve(n+1)) return true;
    }
    delete [] possible;
    B[x][y].n=0;
    return false;

}

bool load(char *filename)
{
    fstream file;
    char *line=new char[256];
    int * factors, flength;
    file.open(filename);
    if(file.is_open())
    {
        int k=0,n=0;
        file.getline(line,256);
        while(line[k]!='\0'&& line[k]!='\r')
        {
            n++;k++;
        }
        file.seekg(0);
        num=n;
        factors=factor(n,flength);
        if(flength<=1)
        {
            cout<<"\nBoard dimension error"<<endl;
            file.close();
            return false;
        }
        numx=factors[flength-1];
        numy=factors[flength-2];
        B=new Board*[num];
        for(int i=0; i<num; i++)
            B[i]=new Board[num];
        int j=0;
        while (!file.eof())
        {
            int i=0;
            file.getline(line,256);

            while(line[i]!=0 && line[i]!='\r')
            {
                B[i][j].n=line[i]-48;
                B[i][j].orig=(B[i][j].n!=0);
                i++;
            }
            j++;
        }
        delete [] factors;
    }
    else
    {
        file.close();
        cout<<"\nCould not open file"<<endl;
        delete [] line;
        return false;
    }
    file.close();
    delete [] line;
    return true;
}
void disp(void)
{
    cout<<endl;
    for(int i=0; i<num; i++)
    {
        for(int j=0; j<num; j++)
        {
            cout<<B[j][i].n<<" ";
            if(j!=num-1 && (j+1)%numx==0) cout<<"| ";
        }
        cout<<endl;
        if(i!=num-1 && (i+1)%numy==0)
        {
            for(int k=0; k<num; k++) (k!=num-1 && (k+1)%numx==0)?cout<<"--+-":cout<<"--";
            cout<<endl;
        }
    }
}


int main(int argc, char* argv[])
{
    char * filename=new char[256];
    if(argc!=2)
    {
        cout<<"Enter Filename: ";
        cin.getline(filename,256);
    }
    else
        strcpy(filename,argv[1]);
    if(load(filename))
    {
        disp();
        solve(0);
        disp();
        for(int i=0; i<num; i++)
            delete [] B[i];
        delete [] B;
    }
    else
        cout<<"could not load filename"<<endl;
    // cin.ignore();
    delete [] filename;
    return 0;
}
