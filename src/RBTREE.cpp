#ifndef RBTREECPP
#define RBTREECPP

#include "RBTREE.h"

#include <assert.h>
#include <queue>
using namespace std;


template <typename Type>
rbtree<Type>::rbtree()
{
    //构造函数 
    //初始化Nil为空  root=Nil  且root和Nil的颜色都为黑色
    Nil=new rbnode<Type>;
    assert(Nil != nullptr);
    Nil->color=BLACK;
    Nil->left=Nil->right=Nil->parent=nullptr;
    
    root=Nil;
}

template <typename Type>
rbtree<Type>::~rbtree()
{
    destroy(root);
    delete Nil;
    Nil = nullptr;
}

template <typename Type>
bool rbtree<Type>::Insert(const Type &value)
{   
    //插入value 插入成功返回1 如果已经有该数据则插入失败返回0
    
    /*
    插入分两步
    1.寻找该结点应该插入的地方 创建新节点 
    2.该结点默认设置为红色  根据调整策略调整
    */

    rbnode<Type>* prev=Nil;
    rbnode<Type>* local=root;

    //找到该结点在二叉排序树中的位置
    while(local != Nil){

        if(local->data == value){
            //已经存在值为value的结点 插入失败
            cout<<"该值已存在！插入失败！"<<endl;
            return false;
        }

        prev = local;  //记录当前位置

        if(local->data > value){
            local = local->left;
        }
        else if(local->data < value){
            local = local->right;
        }
        
    }

    //此时已经找到 loc=Nil prev=父节点
    //创建结点备用
    rbnode<Type>* insert_node=CreateNode(value);
    if(prev == Nil){
        //prev == Nil 说明树是空的 要插入的是root结点
        root=insert_node;
        root->color = BLACK;
    } else {
        //prev != Nil 不是插入root结点
        if(value < prev->data){
            //插左边
            prev->left=insert_node;
            insert_node->parent=prev;
        } else {
            //插右边
            prev->right=insert_node;
            insert_node->parent=prev;
        }
    }
    //第一步 结点插入 结束！！

    //第二步 调整
    Insert_Fixup(insert_node);
    
    return true;
}

template <typename Type>
void rbtree<Type>::Remove(Type key)
{
    //删除结点并维护平衡
    /*
        第一步：找到真正要删除的结点
    */
    rbnode<Type>* snode=SearchNode(root,key); //找到逻辑上要删除的结点
    rbnode<Type>* y=Nil;
    rbnode<Type>* py=Nil;
    int deletecolor=BLACK;
    int direction=RIGHT;
    //判断
    if(snode == Nil){
        cout<<"要删除的结点不存在！"<<endl;
        return;
    }
    if(snode->left == Nil && snode->right == Nil){
        //如果是叶子结点 要删除的就是他自己
        y=Nil;
        py=snode->parent;
        if(snode == root){
            root=Nil;
        }else if(snode == snode->parent->left){
                snode->parent->left = snode->right;
                direction=LEFT;
            } else {
                snode->parent->right = snode->right;
                direction=RIGHT;
            }
        deletecolor=snode->color;
        delete snode;
    } else if(snode->left == Nil){
        //只有右子树
        int precolor=snode->color;
        if(snode == root)
        {
            py=Nil;

            root=snode->right;
            snode->right->parent=Nil;
        } else {
            py=snode->parent;

            if(snode == snode->parent->left){
                snode->parent->left = snode->right;
                direction = LEFT;
            } else {
                snode->parent->right = snode->right;
                direction = RIGHT;
            }
            snode->right->parent = snode->parent;
        }
        y = snode->right;
        deletecolor=snode->color;
        delete snode;
    } else if(snode->right == Nil){
        //只有左子树
        int precolor=snode->color;
        if(snode == root)
        {
            py=Nil;
            root=snode->left;
            snode->left->parent=Nil;
        } else {
            py=snode->parent;

            if(snode == snode->parent->left){
                snode->parent->left = snode->left;
                direction=LEFT;
            } else {
                snode->parent->right = snode->left;
                direction=RIGHT;
            }
            snode->left->parent = snode->parent;
        }
        y = snode->left;
        deletecolor=snode->color;
        delete snode;
    } else {
        //有两个子树
        rbnode<Type>* mini=Minimun(snode);
        
        snode->data = mini->data; //数据上移动
        if(mini->right == Nil){
            y=Nil;
            py=mini->parent;
            if(mini == mini->parent->left){
                mini->parent->left = mini->right;
                direction = LEFT;
            } else {
                mini->parent->right = mini->right;
                direction = RIGHT;
            }
            deletecolor=mini->color;
            delete mini;
        } else {
            //只有右子树
            if(mini == root)
            {
                py=Nil;
                root=mini->right;
                root->color=BLACK;
                mini->right->parent=Nil;
            } else {
                py=mini->parent;
                if(mini == mini->parent->left){
                    mini->parent->left = mini->right;
                    direction = LEFT;
                } else {
                    mini->parent->right = mini->right;
                    direction = RIGHT;
                }
                mini->right->parent = mini->parent;
            }
            y = mini->right;
            deletecolor=mini->color;

            delete mini;
        }
    }
    //删除结束 开始调整
    Remove_Fixup(y,py,deletecolor,direction);
    return ;
}

template <typename Type>
rbnode<Type> *rbtree<Type>::CreateNode(const Type &addkey)
{
    //创建一个新结点 data值为addkey 默认创建红色结点 
    rbnode<Type>* newnode=new rbnode<Type>;
    assert(newnode != nullptr);

    newnode->color=RED;
    newnode->left=newnode->right=newnode->parent=Nil;
    newnode->data=addkey;
    return newnode;
}

template <typename Type>
void rbtree<Type>::InOrder(rbnode<Type> *root)
{
    if(root->left !=  Nil)InOrder(root->left);
    cout<<root->data<<" ";
    if(root->right != Nil)InOrder(root->right);
}

template <typename Type>
void rbtree<Type>::InOrderColor(rbnode<Type> *root)
{
    if(root->left !=  Nil)InOrder(root->left);
    cout<<root->color<<" ";
    if(root->right != Nil)InOrder(root->right);
}

template <typename Type>
void rbtree<Type>::LevelOrder(rbnode<Type> *root)
{
    //层序遍历
    queue<pair<rbnode<Type>*,int>> que;

    int level_index=0;

    if(root == Nil) 
    {
        cout<<"黑Nil"<<endl;
        return;
    }
    que.push({root,level_index});

    while(!que.empty()){
        auto t=que.front();
        que.pop();

        auto node = t.first;
        auto level = t.second;
        
        if(level > level_index){
            cout<<endl;
            level_index = level;
        }
        

        if(node->color == RED)cout<<"红";
        else cout<<"黑";

        if(node == Nil)cout<<"Nil"<<" ";
        else cout<<node->data<<" ";


        if(node != Nil)
        {
            que.push({node->left,level+1});
            que.push({node->right,level+1});
        }
    }

    cout<<endl;
    return ;
}

template <typename Type>
void rbtree<Type>::LeftRotate(rbnode<Type> *y)
{
    if(!y || y == Nil || y->parent == Nil)return;
    
    rbnode<Type>* py=y->parent;
    rbnode<Type>* gy=py->parent;


    py->right=y->left;
    if(y->left != Nil)y->left->parent=py;

    if(gy != Nil){
        if(py == gy->left)gy->left=y;
        else gy->right=y;
    } else {
        root = y;
        y->parent = Nil;
        y->color = BLACK;
    }
    y->parent=gy;

    py->parent=y;
    y->left=py;
}

template <typename Type>
void rbtree<Type>::RightRotate(rbnode<Type> *y)
{
    if(!y || y == Nil || y->parent == Nil)return;
    
    rbnode<Type>* py=y->parent;
    rbnode<Type>* gy=py->parent;
    
    py->left=y->right;
    if(y->right != Nil)y->right->parent=py;

    if(gy != Nil){
        if(py == gy->left)gy->left=y;
        else gy->right=y;
    } else {
        root = y;
        y->parent = Nil;
        y->color = BLACK;
    }
    y->parent=gy;

    py->parent=y;
    y->right=py;
}

template <typename Type>
void rbtree<Type>::Insert_Fixup(rbnode<Type> *y)
{
    if(!y || y == Nil)return ;
    
    /*
        插入调整思路（插入结点默认为红）：
        1.检查父节点
            父节点为黑：没有冲突 调整完毕
            父节点为红：有冲突 进入2
        2.拿取爷爷结点和uncle结点 检查uncle结点颜色
            uncle结点为红：固定策略调整颜色 检查爷爷结点 递归调整爷爷结点
            uncle结点为黑：旋转 改色 调整结束
    */

    rbnode<Type> *py=y->parent;

    //1.检查父结点
    if(py->color == BLACK) return;

    //2.
    rbnode<Type> *gy=py->parent;
    rbnode<Type> *u=(py == gy->left)?gy->right:gy->left;

    //检查uncle
    if(u->color == RED) //XYr型  修改颜色后矛盾上移 递归调整
    {
        if(gy != root)gy->color = RED;
        py->color = BLACK;
        u->color = BLACK;
        y->color = RED;

        if(gy == root || gy->parent->color == BLACK)return;
        else Insert_Fixup(gy);
    } 
    else //uncle为黑色 旋转 旋转后必结束
    {
        //第一步 判断具体类型
        if(py == gy->left){
            if(y == py->left){
                //LLb 右旋一次
                RightRotate(py);
                py->color=BLACK;
                gy->color=RED;
            } else if (y == py->right){
                //LRb 对y左旋一次 右旋一次
                LeftRotate(y);
                RightRotate(y);
                y->color=BLACK;
                gy->color=RED;
            }
        } else {
            if(y == py->right){
                //RRb 左旋一次
                LeftRotate(py);
                py->color=BLACK;
                gy->color=RED;
            } else if (y == py->left){
                //RLb 对y右旋一次 左旋一次
                RightRotate(y);
                LeftRotate(y);
                y->color=BLACK;
                gy->color=RED;
            }
        }


    }
}

template <typename Type>
rbnode<Type> *rbtree<Type>::SearchNode(rbnode<Type> *root, Type key)
{
    rbnode<Type>* q=root;
    while(q!=Nil){
        if(q->data == key)return q;
        if(key < q->data)q=q->left;
        else q=q->right;
    }
    return Nil;
}

template <typename Type>
void rbtree<Type>::Tranplant(rbnode<Type> *u, rbnode<Type> *v)
{
}

template <typename Type>
rbnode<Type> *rbtree<Type>::Minimun(rbnode<Type> *x)
{
    //找到右子树中最小的结点
    rbnode<Type> *y=x->right;
    while(y->left != Nil){
        y = y->left;
    }
    return y;
}

template <typename Type>
void rbtree<Type>::Remove(rbnode<Type> *z)
{
    
}

template <typename Type>
void rbtree<Type>::Remove_Fixup(rbnode<Type> *y,rbnode<Type> *py,int deletecolor,int direction)
{
    if(deletecolor == RED)return ; //删除结点为红色 不用调整

    //删除结点为黑色
    /*
        1.确定类型 --> 找到兄弟结点！
    */
    rbnode<Type>* bro=(direction == RIGHT)?py->left:py->right;

    if(py == Nil)
    {
        y->color=BLACK;
        return ;
    }
    //兄弟结点为黑色
    if(bro->color == BLACK){
        int num_redchild_of_bro=0;
        if(bro->left->color == RED)num_redchild_of_bro++;
        if(bro->right->color == RIGHT)num_redchild_of_bro++;
        if(!num_redchild_of_bro){
            //Rb0 or Lb0
            //父黑兄红
            int pycolor=py->color;
            cout<<"here"<<endl;
            py->color = BLACK;
            bro->color = RED;
           
            if(pycolor == BLACK){
                if(py == root)return ;
                else {
                    int dir=(py == py->parent->left)?LEFT:RIGHT;
                    Remove_Fixup(py,py->parent,py->color,dir);
                }
            }
        } 
        else if(num_redchild_of_bro == 1){
            //Rb1 or Lb1
            //要确定是内红还是外红
            int reddir=(bro->left->color == RED)?LEFT:RIGHT;
            rbnode<Type>* w=(bro->left->color == RED)?bro->left:bro->right;

      

            if(reddir == direction){
                //内红
           
                w->color = py->color;
                py->color = BLACK;
                if(reddir == RIGHT){
                    LeftRotate(w);
                    RightRotate(w);
                } else {
                    RightRotate(w);
                    LeftRotate(w);
                }
            } else {
                //外红
               
                w->color=BLACK;
                bro->color=py->color;
                py->color=BLACK;    

                if(reddir == LEFT){
                    RightRotate(bro);
                } else {
                    LeftRotate(bro);
                }
            }
            
        }
        else if(num_redchild_of_bro == 2){
            //双红
            rbnode<Type>* inside=(direction == RIGHT)?bro->right:bro->left;
            inside->color=py->color;
            py->color=BLACK;
            if(direction == RIGHT){
                LeftRotate(inside);
                RightRotate(inside);
            } else {
                RightRotate(inside);
                LeftRotate(inside);
            }
        }
    }
    else 
    {
        //兄弟结点为红 只需旋转一次 把问题转为兄弟结点为黑
        bro->color=BLACK;
        py->color=RED;
        if(direction == RIGHT)RightRotate(bro);
        else LeftRotate(bro);
        Remove_Fixup(y,py,BLACK,direction);
    }

    return ;
}

template <typename Type>
void rbtree<Type>::destroy(rbnode<Type> *root)
{
    if(root == Nil)return;
    if(root->left != Nil)destroy(root->left);
    if(root->right != Nil)destroy(root->right);
    delete root;
    root = nullptr;
    return ;
}


#endif
