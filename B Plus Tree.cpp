#include <bits/stdc++.h>

#define all(x)      x->keys.begin(), x->keys.begin()+x->len

using namespace std;

class B_plus_tree{
    private:

    int order;
    struct Node{
        bool leaf_hai_kya=false;
        vector<int> keys;
        vector<Node*> child;
        Node* next=nullptr;
        Node* prt=nullptr;
        int len=0;
        Node(int order){
            keys.assign(order,-1);
            child.assign(order+1,nullptr);
        }
    };

    Node* root;
    
    public:

    B_plus_tree(int ord){
        order=ord;
        root= new Node(ord);
        root->leaf_hai_kya=true;
    }

    void print(Node* col){
        cout<<"["<<flush;
        for(int i=0;i<col->len;i++) cout<<col->keys[i]<<","<<flush;
        cout<<"]\n"<<flush;
    }

    void display(){
        Node* row=root;
        int i=0;
        while (row!=nullptr){
            cout<<"Level "<<i<<"\n"<<flush;
            Node* col=row;
            while (col!=nullptr){
                cout<<"["<<flush;
                for(int i=0;i<col->len;i++) cout<<col->keys[i]<<","<<flush;
                cout<<"]"<<flush;
                col=col->next;
            }
            cout<<"\n"<<flush;
            row=row->child[0];
            i++;
        }
        cout<<"\n"<<flush;
    }

    Node* present(Node* curr,int n){
        auto itr=lower_bound(curr->keys.begin(),curr->keys.begin()+curr->len,n);
        if (curr->leaf_hai_kya){
            if(itr!=curr->keys.end() && *itr==n) return curr;
            else return nullptr;
        }
        else{
            int idx=itr-(curr->keys.begin());
            return present(curr->child[idx],n);
        }
    }

    Node* search(int n){
        return present(root,n);
    }

    void internalInsertion(Node* curr,int key,Node* child){
        for(int i=0;i<curr->len;i++){
            if (curr->keys[i]>key){
                swap(curr->keys[i],key);
                swap(curr->child[i+1],child);
            }
        }
        curr->keys[curr->len]=key;
        curr->child[curr->len+1]=child;
        curr->len++;
        if (curr->len>=order){
            if (curr==root){
                Node* child2=new Node(order);
                root=new Node(order);
                child2->prt=root;
                curr->prt=root;
                int spt=ceil((curr->len-1)/2.0);
                root->child[0]=curr;
                root->child[1]=child2;
                root->keys[0]=curr->keys[spt];
                root->len++;
                curr->next=child2;
                child2->child[0]=curr->child[spt+1];
                for(int i=spt+1;i<curr->len;i++){
                    child2->keys[child2->len]=curr->keys[i];
                    child2->child[child2->len+1]=curr->child[i+1];
                    child2->len++;
                }
                for(int i=0;i<child2->len+1;i++) child2->child[i]->prt=child2;
                
                curr->len=spt;
            }
            else{
                Node* node=new Node(order);
                node->prt=curr->prt;
                int spt=ceil((curr->len-1)/2.0);
                node->child[0]=curr->child[spt+1];
                for(int i=spt+1;i<curr->len;i++){
                    node->keys[node->len]=curr->keys[i];
                    node->child[node->len+1]=curr->child[i+1];
                    node->len++;
                }
                for(int i=0;i<node->len+1;i++){
                    node->child[i]->prt=node;
                }
                curr->len=spt;
                curr->next=node;
                internalInsertion(curr->prt,curr->keys[spt],node);

            }
        }
    }

    bool insert(int n){
        Node* pres=search(n);
        if (pres!=nullptr){
            return false;
        }
        else{
            Node* trg=root;
            while (!trg->leaf_hai_kya){
                auto itr=lower_bound(trg->keys.begin(),trg->keys.begin()+trg->len,n);
                int idx=itr-(trg->keys.begin());
                trg=trg->child[idx];
            }
            
            for (int i=0;i<trg->len;i++){
                if (trg->keys[i]>n){
                    swap(trg->keys[i],n);
                }
            }

            trg->keys[trg->len]=n;
            trg->len++;
            
            if (trg->len>=order){
                if(trg==root){
                    Node* child2=new Node(order);
                    child2->leaf_hai_kya=true;
                    root=new Node(order);
                    int spt=ceil((trg->len)/2.0);
                    child2->prt=root;
                    trg->prt=root;
                    root->child[0]=trg;
                    root->child[1]=child2;
                    trg->next=child2;

                    for(int i=spt;i<trg->len;i++){
                        child2->keys[child2->len]=trg->keys[i];
                        child2->len++;
                    }
                    trg->len=spt;
                    root->keys[0]=trg->keys[spt-1];
                    root->len++;

                }
                else{
                    Node* node=new Node(order);
                    node->leaf_hai_kya=true;
                    node->prt=trg->prt;
                    node->next=trg->next;
                    trg->next=node;

                    int spt=ceil((trg->len)/2.0);
                    for(int i=spt;i<trg->len;i++){
                        node->keys[node->len]=trg->keys[i];
                        node->len++;
                    }
                    trg->len=spt;
                    internalInsertion(trg->prt,trg->keys[spt-1],node);
                }
            }
            return true;
        }
    }

    void internaldel(int idx,Node* curr,Node* rightnode){
        if (curr==root && curr->len==1){
            root=curr->child[0];
            root->prt=nullptr;
            root->next=nullptr;
            delete curr;
            delete rightnode;
            return;
        }
        int mxv=curr->keys[curr->len-1];
        for (int i=idx;i<curr->len-1;i++){
            curr->keys[i]=curr->keys[i+1];
            curr->child[i+1]=curr->child[i+2];
        }
        curr->len--;
        delete rightnode;
        if (curr->len>=(order-1)/2 || curr==root) return;
        Node* prnt=curr->prt;
        auto itr=lower_bound(all(prnt),curr->keys[curr->len]);

        if(itr!=prnt->keys.begin()){
            int leftindex=itr-prnt->keys.begin()-1;
            Node* leftnode = prnt->child[leftindex];
            if (leftnode->len>=ceil((order-1)/2)+1){
                idx=leftnode->keys[leftnode->len-1];
                int key1=prnt->keys[leftindex];
                Node* chld=leftnode->child[leftnode->len];
                chld->prt=curr;
                for (int i=0;i<=prnt->len;i++){
                    swap(key1,curr->keys[i]);
                    swap(chld,curr->child[i]);
                }
                curr->len++;
                curr->child[curr->len]=chld;
                prnt->keys[leftindex]=idx;
                leftnode->len--;
                return;
            }
        }
        if(itr!=prnt->keys.begin()+prnt->len){
            int rightindex=itr-prnt->keys.begin();
            Node* rightnode=prnt->child[rightindex+1];
            if(rightnode->len>=ceil((order-1)/2)+1){
                curr->keys[curr->len]=prnt->keys[rightindex];
                Node* chld=rightnode->child[0];
                chld->prt=curr;
                curr->child[curr->len+1]=chld;
                curr->len++;
                prnt->keys[rightindex]=rightnode->keys[0];
                for(int i=0;i<rightnode->len;i++){
                    rightnode->keys[i]=rightnode->keys[i+1];
                    rightnode->child[i]=rightnode->child[i+1];
                }
                rightnode->len--;
                return;
            }
        }
        if(itr!=prnt->keys.begin()){
            int leftindex=itr-prnt->keys.begin()-1;
            Node* leftnode = prnt->child[leftindex];
            
            leftnode->len++;
            leftnode->keys[leftnode->len-1]=prnt->keys[leftindex];
            prnt->keys[leftindex]=mxv;
            leftnode->child[leftnode->len]=curr->child[0];
            curr->child[0]->prt=leftnode;
            for (int i=0;i<curr->len;i++){
                leftnode->keys[leftnode->len]=curr->keys[i];
                leftnode->child[leftnode->len+1]=curr->child[i+1];
                leftnode->len++;
                curr->child[i+1]->prt=leftnode;
            }
            leftnode->next=curr->next;
            internaldel(leftindex,prnt,curr);
        }
        else if(itr!=prnt->keys.begin()+prnt->len){
            int rightindex=itr-prnt->keys.begin();
            Node* rightnode=prnt->child[rightindex+1];
            curr->len++;
            curr->keys[curr->len-1]=prnt->keys[rightindex];
            curr->child[curr->len]=rightnode->child[0];
            rightnode->child[0]->prt=curr;
            for(int i=0;i<rightnode->len;i++){
                curr->keys[curr->len]=rightnode->keys[i];
                curr->child[curr->len+1]=rightnode->child[i+1];
                curr->len++;
                rightnode->child[i+1]->prt=curr;
            }
            curr->next=rightnode->next;
            internaldel(rightindex,prnt,rightnode);
        }

    }   

    bool del(int n){
        Node* pres=search(n);
        if (pres==nullptr){
            return false;
        }
        int key=pres->keys[pres->len-1];

        auto itr=lower_bound(all(pres),n);
        int idx=itr-pres->keys.begin();
        for (int i=idx;i<pres->len-1;i++){
            pres->keys[i]=pres->keys[i+1];
        }
        pres->len--;

        if (pres==root) return true;
        else if (pres->len>=ceil((order-1)/2))return true;
        else{
            Node* prnt=pres->prt;
            auto itr=lower_bound(all(prnt),key);
            if (itr!=prnt->keys.begin()){
                Node* leftnode=prnt->child[itr-prnt->keys.begin()-1];
                if (leftnode->len>=ceil((order-1)/2)+1){
                    int key=leftnode->keys[leftnode->len-1];
                    for(int i=0;i<pres->len+1;i++){
                        swap(key,pres->keys[i]);
                    }
                    pres->len++;
                    leftnode->len--;
                    prnt->keys[itr-prnt->keys.begin()-1]=leftnode->keys[leftnode->len-1];
                    return true;
                }
            }
            if (itr!=prnt->keys.begin()+prnt->len){
                Node* rightnode=prnt->child[itr-prnt->keys.begin()+1];
                if (rightnode->len>=ceil((order-1)/2)+1){
                    int key=rightnode->keys[0];
                    pres->keys[pres->len]=key;
                    pres->len++;
                    for (int i=0;i<rightnode->len-1;i++){
                        rightnode->keys[i]=rightnode->keys[i+1];
                    }
                    rightnode->len--;
                    prnt->keys[itr-prnt->keys.begin()]=pres->keys[pres->len-1];
                    return true;
                }
            }
            if (itr!=prnt->keys.begin()){
                Node* leftnode=prnt->child[itr-prnt->keys.begin()-1];

                for (int i=0;i<pres->len;i++){
                    leftnode->keys[leftnode->len]=pres->keys[i];
                    leftnode->len++;
                }
                leftnode->next=pres->next;
                internaldel(int(itr-prnt->keys.begin()-1),prnt,pres);
            }
            else if(itr!=prnt->keys.begin()+prnt->len){
                Node* rightnode=prnt->child[itr-prnt->keys.begin()+1];
                
                for (int i=0;i<rightnode->len;i++){
                    pres->keys[pres->len]=rightnode->keys[i];
                    pres->len++;
                }
                pres->next=rightnode->next;
                internaldel(int(itr-prnt->keys.begin()),prnt,rightnode);
            }
            return true;
        }
    }
};

void start(int order){
    B_plus_tree data=B_plus_tree(order);
    int choice,n;

    cout<<"Enter corresponding choice:\n";
    cout<<"1. Display\n";
    cout<<"2. Insertion\n";
    cout<<"3. Deletion\n";
    cout<<"4. Check for present\n";
    cout<<"5. Exit\n";

    while(true){
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
                else cout<<"Value already exist\n\n";
                break;
            case 3:
                cout<<"Enter the value to be deleted: ";
                cin>> n;
                if(data.del(n)) cout<<"Deleted successfully\n\n";
                else cout<<"Value not found\n\n";
                break;
            case 4:
                cout<<"Enter the value to check if present: ";
                cin>>n;
                if(data.search(n)) cout<<"Value is present\n\n";
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
    int order;
    cout<<"Enter Order for the B+ Tree: ";
    cin>>order;
    start(order);
    return 0;
}
