#include <iostream>
//#include <bits/stdc++.h>
using namespace std;

//class coords {
//public:
//    int x;
//    int y;
//};
//
//class cordcmp {
//public:
//    int operator()(coords &c1 ,coords &c2) {
//        if (c1.x>c2.x || (c1.x==c2.x && c1.y>c2.y))
//            return 1;
//        if(c1.x<c2.x || (c1.x==c2.x && c1.y<c2.y))
//            return -1;
//        return 0;
//    }
//};

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

template <class gt>
struct avlnode {
public:
    gt val;
    avlnode<gt> *left;
    avlnode<gt> *right;
    int ht;
    int count;
    int nsbtr;
    gt min;
    gt max;
};

template <class gt, class ct=MyComp<gt>>
class AVLtree {
public:
    avlnode<gt> *main_root = NULL;
    ct compare;

    int dcmp(gt a,gt b) {
        return compare(a,b);
    }

    int height(avlnode<gt> *tnode) {
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

    int children(avlnode<gt> *tnode) {
        if(tnode == NULL)
            return 0;
        return tnode->count + children(tnode->left) + children(tnode->right);
    }

    gt sbtr_min(avlnode<gt> *tnode) {
        if(tnode->left==NULL) {
            return tnode->val;
        }
        return sbtr_min(tnode->left);
    }

    gt sbtr_max(avlnode<gt> *tnode) {
        if(tnode->right==NULL) {
            return tnode->val;
        }
        return sbtr_max(tnode->right);
    }

    avlnode<gt> *rotateleft(avlnode<gt> *x) {
        avlnode<gt> *y;
        y = x->right;
        x->right = y->left;
        y->left = x;
        x->ht = height(x);
        x->nsbtr = children(x);
        x->min = sbtr_min(x);
        x->max = sbtr_max(x);
        y->ht = height(y);
        y->nsbtr = children(y);
        y->min = sbtr_min(y);
        y->max = sbtr_max(y);
        return y;
    }

    avlnode<gt> *rotateright(avlnode<gt> *x) {
        avlnode<gt> *y;
        y = x->left;
        x->left = y->right;
        y->right = x;
        x->ht = height(x);
        x->nsbtr = children(x);
        x->min = sbtr_min(x);
        x->max = sbtr_max(x);
        y->ht = height(y);
        y->nsbtr = children(y);
        y->min = sbtr_min(y);
        y->max = sbtr_max(y);
        return y;
    }

    avlnode<gt> *RL(avlnode<gt> *tnode) {
        tnode->right = rotateright(tnode->right);
        tnode = rotateleft(tnode);
        return tnode;
    }

    avlnode<gt> *RR(avlnode<gt> *tnode) {
        tnode = rotateleft(tnode);
        return tnode;
    }

    avlnode<gt> *LR(avlnode<gt> *tnode) {
        tnode->left = rotateleft(tnode->left);
        tnode = rotateright(tnode);
        return tnode;
    }

    avlnode<gt> *LL(avlnode<gt> *tnode) {
        tnode = rotateright(tnode);
        return tnode;
    }

    int BF(avlnode<gt> *tnode) {
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

    avlnode<gt> *insert_node(avlnode<gt> *tnode, gt x) {
        if (tnode == NULL) {
            tnode = new avlnode<gt>;
            tnode->val = x;
            tnode->left = NULL;
            tnode->right = NULL;
            tnode->count = 1;
            tnode->nsbtr = 1;
            tnode->min = x;
            tnode->max = x;
        }
//        else if (tnode->val == x) {
        else if(dcmp(tnode->val,x)==0) {
            tnode->count += 1;
        }
//        else if (x < tnode->val) {
        else if(dcmp(x,tnode->val)==-1) {
            tnode->left = insert_node(tnode->left, x);
            if (BF(tnode) == 2)
//                if (x < tnode->left->val)
                if(dcmp(x,tnode->left->val)==-1)
                    tnode = LL(tnode);
                else
                    tnode = LR(tnode);
        }
        else {
            tnode->right = insert_node(tnode->right, x);
            if (BF(tnode) == -2)
//                if (x > tnode->right->val)
                if(dcmp(x,tnode->right->val)==1)
                    tnode = RR(tnode);
                else
                    tnode = RL(tnode);
        }
        tnode->ht = height(tnode);
        tnode->nsbtr = children(tnode);
        tnode->min = sbtr_min(tnode);
        tnode->max = sbtr_max(tnode);
        return tnode;
    }

    void insert(gt k) {
        avlnode<gt> *tnode = main_root;
        main_root = insert_node(tnode,k);
    }
    
    void inorder(avlnode<gt> *root,void(*vfp)(gt)) {
        if(root == NULL)
            return;
        inorder(root->left,vfp);
//        cout << root->val << "," << root->count << ",[" << root->ht << "](" << root->nsbtr << ") ";
        for(int i=0;i<root->count;i++)
            vfp(root->val);
        inorder(root->right,vfp);
    }

    void display(void(*vfp)(gt)) {
        avlnode<gt> *tnode = main_root;
        inorder(tnode,vfp);
    }

    avlnode<gt> *delete_node(avlnode<gt> *tnode, gt v) {
        avlnode<gt> *q;
        if(tnode==NULL)
            return NULL;
        else {
//            if(tnode->val == v) {
            if(dcmp(tnode->val,v)==0) {
                tnode->count -= 1;
                if(tnode->count == 0) {
                    if(tnode->right != NULL) {
                        q = tnode->right;
                        while(q->left != NULL) {
                            q = q->left;
                        }
                        tnode->val = q->val;
                        tnode->count = q->count;
                        tnode->right = delete_node(tnode->right,tnode->val);
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
            }
//            else if(v < tnode->val) {
            else if(dcmp(v,tnode->val)==-1) {
                tnode->left = delete_node(tnode->left,v);
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
                tnode->right = delete_node(tnode->right,v);
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
        tnode->nsbtr = children(tnode);
        tnode->min = sbtr_min(tnode);
        tnode->max = sbtr_max(tnode);
        return tnode;
    }

    void remove(gt k) {
        avlnode<gt> *tnode = main_root;
        main_root = delete_node(tnode,k);
    }

    bool search_key(avlnode<gt> *tnode, gt k) {
        if(tnode==NULL){
            return false;
        }
//        else if(k == tnode->val) {
        else if(dcmp(k,tnode->val)==0) {
            return true;
        }
//        else if(k < tnode->val) {
        else if(dcmp(k,tnode->val)==-1) {
            return search_key(tnode->left,k);
        }
        else {
            return search_key(tnode->right,k);
        }
    }

    bool search(gt k) {
        avlnode<gt> *tnode = main_root;
        return search_key(tnode,k);
    }

    int count_occ(avlnode<gt> *tnode, gt k) {
        if(tnode==NULL){
            return 0;
        }
//        else if(k == tnode->val) {
        else if(dcmp(k,tnode->val)==0) {
            return tnode->count;
        }
//        else if(k < tnode->val) {
        else if(dcmp(k,tnode->val)==-1) {
            return count_occ(tnode->left,k);
        }
        else {
            return count_occ(tnode->right,k);
        }
    }

    int count(gt k) {
        avlnode<gt> *tnode = main_root;
        count_occ(tnode,k);
    }

    gt upper_bound(gt k) {
        avlnode<gt> * tn = main_root;
        gt lbk{};
        gt df{};
        while(tn!=NULL) {
//            if(k < tn->val) {
            if(dcmp(k,tn->val)==-1) {
//                if(lbk==df || lbk > tn->val) {
                if(dcmp(lbk,df)==0 || dcmp(lbk,tn->val)==1) {
                    lbk = tn->val;
                }
                tn = tn->left;
            }
            else {
                tn = tn->right;
            }
        }
        return lbk;
    }

    gt lower_bound(gt k) {
        avlnode<gt> * tn = main_root;
        gt lbk{};
        gt df{};
        while(tn!=NULL) {
//            if(tn->val == k) {
            if(dcmp(tn->val,k)==0) {
                return k;
            }
//            if(k < tn->val) {
            if(dcmp(k,tn->val)==-1) {
//                if(lbk==df || lbk > tn->val) {
                if(dcmp(lbk,df)==0 || dcmp(lbk,tn->val)==1) {
                    lbk = tn->val;
                }
                tn = tn->left;
            }
            else {
                tn = tn->right;
            }
        }
        return lbk;
    }

    gt klargest(int k) {
//        if(k>=root->nsbtr) {
//            return root->max;
//        }
        int cnt = k;
        int rsbtz = 0;
        avlnode<gt> *tnode = main_root;
        while(tnode!=NULL) {
            if(tnode->right==NULL)
                rsbtz = 0;
            else
                rsbtz = tnode->right->nsbtr;
            if(rsbtz+1<=cnt && cnt<=rsbtz+tnode->count) {
                return tnode->val;
            }
            else if(cnt > rsbtz+tnode->count) {
                cnt -= rsbtz+tnode->count;
                tnode = tnode->left;
            }
            else {
                tnode = tnode->right;
            }
        }
        gt df{};
        return df;
    }
    int count_range(avlnode<gt> *tnode, gt start, gt end, gt minlim, gt maxlim) {
        if(tnode==NULL)
            return 0;
//        if(minlim>=start && maxlim<=end) {
        if(dcmp(minlim,start)>=0 && dcmp(maxlim,end)<=0) {
            return tnode->nsbtr;
        }
//        if(tnode->val > end) {
        if(dcmp(tnode->val,end)==1) {
            return count_range(tnode->left,start,end,minlim,tnode->val);
        }
//        else if(tnode->val < start) {
        else if(dcmp(tnode->val,start)==-1) {
            return count_range(tnode->right,start,end,tnode->val,maxlim);
        }
        else {
            return tnode->count + count_range(tnode->left,start,end,minlim,tnode->val) + count_range(tnode->right,start,end,tnode->val,maxlim);
        }
    }
    int range(gt start, gt end) {
        avlnode<gt> *tnode = main_root;
        return count_range(tnode,start,end,tnode->min,tnode->max);
    }

    gt lesser_bound(gt k) {
        avlnode<gt> * tn = main_root;
        gt lbk{};
        gt df{};
        while(tn!=NULL) {
//            if(tn->val == k) {
            if(dcmp(tn->val,k)==0) {
                return k;
            }
//            if(k > tn->val) {
            if(dcmp(k,tn->val)==1) {
//                if(lbk==df || lbk < tn->val) {
                if(dcmp(lbk,df)==0 || dcmp(lbk,tn->val)==-1) {
                    lbk = tn->val;
                }
                tn = tn->right;
            }
            else {
                tn = tn->left;
            }
        }
        return lbk;
    }

    gt closest(gt k,gt(*cfp)(gt,gt,gt)=nullptr) {
        if(dcmp(k,main_root->min)<=0)
            return main_root->min;
        else if(dcmp(k,main_root->max)>=0)
            return main_root->max;
        gt x = lesser_bound(k);
        gt y = lower_bound(k);
        gt df{};
        if(dcmp(x,k)==0 || dcmp(y,k)==0)
            return k;
        else if(dcmp(x,df)==0 && dcmp(y,df)==0)
            return df;
        else if(dcmp(x,df)==0)
            return y;
        else if(dcmp(y,df)==0)
            return x;
        else {
            if (cfp == nullptr)
                return x;
            return cfp(x, y, k);
        }
    }

};

template <class gt>
gt fn_close_num(gt a, gt b, gt k) {
    if(abs(k-a)<=abs(b-k))
        return a;
    else
        return b;
}

template <class gt>
void fn_avl_disp(gt a) {
    cout << a << " ";
}


int main(void){
    int n,a;
    int k,v;
    cin >> n;
    AVLtree<int> avl;
    void (*dfp)(int) = fn_avl_disp;
    int (*clfp)(int,int,int) = fn_close_num;
    while(n--) {
        cin >> a;
        // cout << "(" << a << ") ";
        switch(a) {
            case 1: //insert
                cin >> k;
                avl.insert(k);
                avl.display(dfp);
                cout << endl;
                break;
            case 2: //erase
                cin >> k;
                avl.remove(k);
                avl.display(dfp);
                cout << endl;
                break;
            case 3: //search
                cin >> k;
                cout << avl.search(k) << endl;
                break;
            case 4: //count
                cin >> k;
                cout << avl.count(k) << endl;
                break;
            case 5: //lower_bound
                cin >> k;
                cout << avl.lower_bound(k) << endl;
                break;
            case 6: //upper_bound
                cin >> k;
                cout << avl.upper_bound(k) << endl;
                break;
            case 7: //closest
                cin >> k;
                dfp(avl.closest(k,clfp));
                cout << endl;
                break;
            case 8: //klargest
                cin >> k;
                dfp(avl.klargest(k));
                cout << endl;
                break;
            case 9: //range
                cin >> k >> v;
                cout << avl.range(k,v);
                break;
            case 10: //display
                avl.display(dfp);
                cout << endl;
                break;
        }
    }
    return 0;
}
