#include <iostream>
#include <sstream>
#include <string>
using namespace std;
#define SIZE 10

unsigned int prime[3] = {7,5,3}; // 2^7-1, 2^5-1, 2^3-1

template <class Tk,class Tv>
struct mapnode {
    Tk key;
    Tv val;
    mapnode<Tk,Tv> *next = NULL;
};

template <class Tk,class Tv>
class UnordMap {
public:
    mapnode<Tk,Tv> *hash_tab[SIZE] = {};

    int hashIndex(Tk k) {
        string ks;
        ostringstream oss;
        oss << k;
        ks = oss.str();
        unsigned long long int h = 0;
        for(int i=0,j=0;i<ks.length();i++,j++) {
            if(j>=3)
                j=0;
            h += ks[i] << prime[j];
            h -= ks[i];
        }
        return h%SIZE;
    }

    void insert(Tk k, Tv v) {
        int indx = hashIndex(k);
        mapnode<Tk,Tv> *mnode = new mapnode<Tk,Tv>;
        mnode->key = k;
        mnode->val = v;
        mnode->next = NULL;
        mapnode<Tk,Tv> *p;
        if(!hash_tab[indx]) {
            hash_tab[indx] = mnode;
        }
        else {
            p = hash_tab[indx];
            while(p->next!=NULL) {
                if(p->key==k)
                    return;
                p=p->next;
            }
            if(p->key!=k) {
                p->next = mnode;
            }
        }
    }

    void erase(Tk k) {
        int indx = hashIndex(k);
        mapnode<Tk,Tv> *p,*q;
        if(hash_tab[indx]!=NULL) {
            if(hash_tab[indx]->key==k) {
                hash_tab[indx] = hash_tab[indx]->next;
                return;
            }
            p = hash_tab[indx]->next;
            q = hash_tab[indx];
            while(p!=NULL) {
                if(p->key==k) {
                    q->next = p->next;
                    delete p;
                    return;
                }
                q = p;
                p=p->next;
            }
        }
    }

    mapnode<Tk,Tv> *search(Tk k) {
        int indx = hashIndex(k);
        mapnode<Tk,Tv> *p;
        if(hash_tab[indx]!=NULL) {
            p = hash_tab[indx];
            while(p!=NULL) {
                if(p->key==k) {
                    return p;
                }
                p = p->next;
            }
        }
        return NULL;
    }

    bool find(Tk k) {
        mapnode<Tk,Tv> *p = search(k);
        if(p!=NULL)
            return true;
        return false;
    }

    void display() {
        mapnode<Tk,Tv> *p;
        for(int i=0;i<SIZE;i++) {
            p = hash_tab[i];
            while(p!=NULL) {
                cout << "[" << i << "]" << p->key << "->" << p->val << " ";
                p = p->next;
            }
        }
    }

    Tv& operator[](Tk k) {
        mapnode<Tk,Tv> *p = search(k);
        if(p==NULL) {
            Tv df{};
            insert(k,df);
            p = search(k);
        }
        return p->val;
    }


};

int main() {
    UnordMap<int,int> map;
    int n,a;
    cin >> n;
    int k,v;
    while(n--) {
        cin >> a;
        // cout << "(" << a << ") ";
        switch(a) {
            case 1: //insert
                cin >> k >> v;
//                for(int i=1;i<=200;i++)
                map.insert(k,v);
                map.display();
                cout << endl;
                break;
            case 2: //erase
                cin >> k;
                map.erase(k);
                map.display();
                cout << endl;
                break;
            case 3: //find
                cin >> k;
                cout << map.find(k) << endl;
                break;
            case 4: //[i]
                cin >> k;
                cout << map[k] << endl;
                break;
            case 5: //[i]=v
                cin >> k >> v;
                map[k] = v;
                map.display();
                cout << endl;
                break;
            case 6: //display
                map.display();
                cout << endl;
                break;
        }
    }
    return 0;
}
