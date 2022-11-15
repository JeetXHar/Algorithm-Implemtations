#include <bits/stdc++.h>

#define fo(i,l,r)       for(int i=l; i<r; i+=1)
#define all(x)          x.begin(),x.end()
#define pb              push_back
#define vi              vector<int>
using namespace std;



class extendible_hashing{
    private:

    struct Bucket{
        int localdepth=0;
        vi values;
    };
    map<int,Bucket *>buckets;
    int globaldepth=0,bucketsize;

    public:

    extendible_hashing(int bs){
        bucketsize=bs;
        buckets[0]=new Bucket;
    }

    void display(){
        cout<<"Global Depth is : "<<globaldepth<<endl;
        fo(i,0,1<<globaldepth){
            cout<<"Depth of Bucket pointed by hash value "<<i<<" :"<<buckets[i]->localdepth<<"\n";
            for(auto x: buckets[i]->values) cout<<x<<" ";
            cout<<"\n";
        }
        cout<<endl;
    }

    int present(int value){
        int h=value&((1<<globaldepth)-1);
        return (find(all(buckets[h]->values),value))!=(buckets[h]->values).end();
    }

    void split(int ind,Bucket * bkt){
        
        Bucket * newbucket=new Bucket;
        vi temp=bkt->values;
        bkt->values.clear();

        if (bkt->localdepth == globaldepth){
            buckets[ind^(1<<globaldepth)]=newbucket;
            if(globaldepth!=0){
                fo(i,0,1<<globaldepth) 
                    if(i!=ind) 
                        buckets[i^(1<<globaldepth)]=buckets[i];
            }
            globaldepth++;
            newbucket->localdepth=++bkt->localdepth;
            for(auto i: temp){
                int h= i&((1<<globaldepth)-1);
                buckets[h]->values.pb(i);
            }
        }
        else{
            int k=ind & (1<<(bkt->localdepth)-1);
            vi indices;

            fo(i,0,1<<globaldepth){
                int last= i& (1<<(bkt->localdepth)-1);
                if (last==k) indices.pb(i);
            }
            
            newbucket->localdepth=++bkt->localdepth;

            fo(i,indices.size()/2,indices.size()) buckets[indices[i]]=newbucket;

            for(auto i : temp){
                int h=i&((1<<globaldepth)-1);
                buckets[h]->values.pb(i);
            }
        }
    }

    bool insert(int n){
        if (present(n)){
            cout<<"Value already Exist!!\n\n";
            return false;
        }
        int h=n&((1<<globaldepth)-1);
        if(buckets[h]->values.size() < bucketsize){
            buckets[h]->values.pb(n);
            return true;
        }
        else{
            split(h,buckets[h]);
            return insert(n);

        }

    }

    void merge(){
        fo(i,0,(1<<(globaldepth-1))){

            int h=(i + (1<<(globaldepth-1)));
            Bucket *curr=buckets[i];
            Bucket *matched=buckets[h];

            if (curr!=matched){
                int sz=curr->values.size()+matched->values.size();
                if (sz<=bucketsize){

                    for(auto x: matched->values) 
                        curr->values.push_back(x);

                    vi inds;

                    for(auto bkt : buckets)
                        if (bkt.second==matched)
                            inds.pb(bkt.first);

                    delete matched;
                    
                    for (auto j : inds) buckets[j]=buckets[i];

                    buckets[i]->localdepth--;
                }
            }
        }
    }

    void shrink(){
        set<Bucket *>bkt;
        for(auto i : buckets) bkt.insert(i.second);
        int count =0;
        int sz = bkt.size();
        for(auto i : bkt) 
            if((globaldepth-(i->localdepth))>=1) count++;
        if(count==sz){
            globaldepth--;
            merge();
            shrink();
        }
    }

    void del(int n){
        if(present(n)){

            int h=n&((1<<globaldepth)-1);
            auto it=find(all(buckets[h]->values),n);
            buckets[h]->values.erase(it);

            merge();
            shrink();
            cout<<"Number deleted.\n\n";
        }
        else cout<<"Error !!! Element not found.\n";
    }

};

void start(int bucketsize){

    extendible_hashing data=extendible_hashing(bucketsize);
    int choice,n;

    cout<<"Enter corresponding choice:\n";
    cout<<"1. Display\n";
    cout<<"2. Insertion\n";
    cout<<"3. Deletion\n";
    cout<<"4. Check for present\n";
    cout<<"5. Exit\n";
    while (true){
        cout<<"Enter a choice-> ";
        cin>>choice;
        cout<<"\n";
        switch(choice){
            case 1:
                data.display();
                break;
            case 2:
                cout<<"Enter the value to be inserted: ";
                cin>>n;
                if (data.insert(n)) cout<<"Insertion Successful\n\n";
                break;
            case 3:
                cout<<"Enter the value to be deleted: ";
                cin>> n;
                data.del(n);
                break;
            case 4:
                cout<<"Enter the value to check if present: ";
                cin>>n;
                if(data.present(n)) cout<<"Value is present\n\n";
                else cout<<"Value not found\n\n";
                break;
            case 5:
                exit(false);
            default:
                cout<<"Invalid choice !!\nTry again\n\n";
                break;
        }
    }
}

int main(){
    int bucketsize;
    cout<<"Enter size of bucket : ";
    cin>>bucketsize;
    start(bucketsize);
    return 0;
}