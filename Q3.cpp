#include <iostream>
using namespace std;

template <class kt,class vt>
struct mapnode {
public:
    kt key;
    vt val;
    mapnode<kt,vt> *left;
    mapnode<kt,vt> *right;
    int ht;
};

template <typename kt>
class MyComp {
public:
    int operator()(kt &a, kt &b) {
        if(a > b)
            return 1;
        if(a < b)
            return -1;
        return 0;
    }
};

template <class kt,class vt,class ct=MyComp<kt>>
class OrdMap {
public:
    mapnode<kt,vt> *main_node = NULL;
    ct compare;
    int mapsize=0;

    int kcmp(kt a,kt b) {
        return compare(a,b);
    }

    int height(mapnode<kt,vt> *tnode) {
        int lh, rh;
        if (tnode == NULL)
            return 0;
        if (tnode->right == NULL)
            rh = 0;
        else
            rh = 1 + tnode->right->ht;
        if (tnode->left == NULL)
            lh = 0;
        else
            lh = 1 + tnode->left->ht;
        if (lh > rh)
            return (lh);
        return (rh);
    }

    mapnode<kt,vt> *rotateleft(mapnode<kt,vt> *x) {
        mapnode<kt,vt> *y;
        y = x->right;
        x->right = y->left;
        y->left = x;
        x->ht = height(x);
        y->ht = height(y);
        return (y);
    }

    mapnode<kt,vt> *rotateright(mapnode<kt,vt> *x) {
        mapnode<kt,vt> *y;
        y = x->left;
        x->left = y->right;
        y->right = x;
        x->ht = height(x);
        y->ht = height(y);
        return (y);
    }

    mapnode<kt,vt> *RL(mapnode<kt,vt> *tnode) {
        tnode->right = rotateright(tnode->right);
        tnode = rotateleft(tnode);
        return (tnode);
    }

    mapnode<kt,vt> *RR(mapnode<kt,vt> *tnode) {
        tnode = rotateleft(tnode);
        return (tnode);
    }

    mapnode<kt,vt> *LR(mapnode<kt,vt> *tnode) {
        tnode->left = rotateleft(tnode->left);
        tnode = rotateright(tnode);
        return (tnode);
    }

    mapnode<kt,vt> *LL(mapnode<kt,vt> *tnode) {
        tnode = rotateright(tnode);
        return (tnode);
    }

    int BF(mapnode<kt,vt> *tnode) {
        int lh, rh;
        if (tnode == NULL)
            return (0);
        if (tnode->right == NULL)
            rh = 0;
        else
            rh = 1 + tnode->right->ht;
        if (tnode->left == NULL)
            lh = 0;
        else
            lh = 1 + tnode->left->ht;
        return (lh - rh);
    }

    mapnode<kt,vt> *m_insert(mapnode<kt,vt> *tnode, kt k, vt v) {
        if (tnode == NULL) {
            tnode = new mapnode<kt,vt>;
            tnode->key = k;
            tnode->left = NULL;
            tnode->right = NULL;
            tnode->val = v;
        }
//        else if (k < tnode->key) {
        else if(kcmp(k,tnode->key)==-1) {
            tnode->left = m_insert(tnode->left, k, v);
            if (BF(tnode) == 2)
//                if (k < tnode->left->key)
                if(kcmp(k,tnode->left->key)==-1)
                    tnode = LL(tnode);
                else
                    tnode = LR(tnode);
        }
//        else if(k > tnode->key) {
        else if(kcmp(k,tnode->key)==1) {
            tnode->right = m_insert(tnode->right, k, v);
            if (BF(tnode) == -2)
//                if (k > tnode->right->key)
                if(kcmp(k,tnode->right->key)==1)
                    tnode = RR(tnode);
                else
                    tnode = RL(tnode);
        }
        tnode->ht = height(tnode);
        return (tnode);
    }

    void insert(kt k, vt v) {
        mapnode<kt,vt> *mnode = main_node;
        main_node = m_insert(mnode,k,v);
        mapsize++;
    }

    void inorder(mapnode<kt,vt> *root,void(*vfp)(kt,vt)) {
        if(root == NULL)
            return;
        inorder(root->left,vfp);
        vfp(root->key,root->val);
        inorder(root->right,vfp);
    }
    void display(void(*vfp)(kt,vt)) {
        mapnode<kt,vt> *mnode = main_node;
        inorder(mnode,vfp);
    }

    mapnode<kt,vt> *delete_node(mapnode<kt,vt> *tnode, kt k) {
        mapnode<kt,vt> *q;
        if(tnode==NULL)
            return NULL;
        else {
//            if(tnode->key == k) {
            if(kcmp(tnode->key,k)==0) {
                if(tnode->right != NULL) {
                    q = tnode->right;
                    while(q->left != NULL) {
                        q = q->left;
                    }
                    tnode->key = q->key;
                    tnode->val = q->val;
                    tnode->right = delete_node(tnode->right,tnode->key);
                    if(BF(tnode)==2) {
                        if(BF(tnode->left)<0) {
                            tnode = LR(tnode);
                        }
                        else {
                            tnode = LL(tnode);
                        }
                    }
                }
                else {
                    return tnode->left;
                }
            }
//            else if(k < tnode->key) {
            else if(kcmp(k,tnode->key)==-1) {
                tnode->left = delete_node(tnode->left,k);
                if(BF(tnode) == -2) {
                    if(BF(tnode->right)>0) {
                        tnode = RL(tnode);
                    }
                    else {
                        tnode = RR(tnode);
                    }
                }
            }
            else {
                tnode->right = delete_node(tnode->right,k);
                if(BF(tnode) == 2) {
                    if(BF(tnode->left)<0) {
                        tnode = LR(tnode);
                    }
                    else {
                        tnode = LL(tnode);
                    }
                }
            }
        }
        tnode->ht = height(tnode);
        return tnode;
    }

    void erase(kt k) {
        mapnode<kt,vt> *mnode = main_node;
        if(find(k)) {
            main_node = delete_node(mnode, k);
            mapsize--;
        }
    }

    mapnode<kt,vt>* search_key(mapnode<kt,vt> *tnode, kt k) {
        if(tnode==NULL){
            return NULL;
        }
//        else if(k == tnode->key) {
        else if(kcmp(k,tnode->key)==0) {
            return tnode;
        }
//        else if(k < tnode->key) {
        else if(kcmp(k,tnode->key)==-1) {
            return search_key(tnode->left,k);
        }
        else {
            return search_key(tnode->right,k);
        }
    }

    bool find(kt k) {
        mapnode<kt,vt> *mnode = main_node;
        mnode = search_key(mnode,k);
        if(mnode==NULL)
            return false;
        return true;
    }

    void clearnode(mapnode<kt,vt> *mnode) {
        if(mnode!=NULL) {
            clearnode(mnode->left);
            clearnode(mnode->right);
            delete mnode;
        }
    }
    void clear() {
        clearnode(main_node);
        main_node = NULL;
        mapsize = 0;
    }

    int size() {
        return mapsize;
    }

    vt& operator[](kt k) {
        mapnode<kt,vt> *mnode = main_node;
        mnode = search_key(mnode,k);
        if(mnode==NULL) {
            vt r{};
            insert(k,r);
            mnode = main_node;
            mnode = search_key(mnode,k);
        }
        return mnode->val;
    }
};

//class CoOrds {
//public:
//    int x,y;
//};

template <class kt,class vt>
void fn_map_disp_kv(kt k,vt v) {
    cout << k << "->" << v << " ";
}

template <class kt,class vt>
void fn_map_disp_v(vt v) {
    cout << v << endl;
}


int main(void){
    OrdMap<int,int> map;
    void (*kvdfp)(int,int) = fn_map_disp_kv;
    int n,a;
    int k,v;
    cin >> n;
    while(n--) {
        cin >> a;
        // cout << "(" << a << ") ";
        switch(a) {
            case 1: //insert
                cin >> k >> v;
                map.insert(k,v);
                map.display(kvdfp);
                cout << endl;
                break;
            case 2: //erase
                cin >> k;
                map.erase(k);
                map.display(kvdfp);
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
                map.display(kvdfp);
                cout << endl;
                break;
            case 6: //size
                cout << map.size() << endl;
                break;
            case 7: //clear
                map.clear();
                map.display(kvdfp);
                cout << endl;
                break;
            case 8: //display
                map.display(kvdfp);
                cout << endl;
                break;
        }
    }
    return 0;
}
