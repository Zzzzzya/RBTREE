#ifndef RBTREE
#define RBTREE
#include <iostream>

//颜色枚举
#define RED 0
#define BLACK 1

#define RIGHT 0
#define LEFT 1

//红黑树结点定义
template <typename Type>
struct rbnode{
    int color;
    Type data;
    rbnode* left;
    rbnode* right;
    rbnode* parent;
};

template <typename Type>
class rbtree{
private:
    rbnode<Type>* root;
    rbnode<Type>* Nil;

public:
    rbtree();
    ~rbtree();
    void InOrder(){
        std::cout<<"中序遍历为:"<<std::endl;
        InOrder(root);
        std::cout<<std::endl;
    }           
    void InOrderColor(){
        std::cout<<"中序遍历颜色为："<<std::endl;
        InOrderColor(root);
        std::cout<<std::endl;
    } 
    void LevelOrder(){
        std::cout<<"层序遍历为:"<<std::endl;
        LevelOrder(root);
    }
    void LevelOrder(rbnode<Type>* root);//层序遍历
    bool Insert(const Type &value);         //插入新结点
    void Remove(Type key);                  //删除结点
protected:
    rbnode<Type>* CreateNode(const Type &addkey=Type());//创建一个新结点
    void InOrder(rbnode<Type>* root);//中序遍历
    void InOrderColor(rbnode<Type>* root);
    void LeftRotate(rbnode<Type>* y);//左旋
    void RightRotate(rbnode<Type>* y);//右旋
    void Insert_Fixup(rbnode<Type>* y);//插入后调整
    rbnode<Type>* SearchNode(rbnode<Type> *root,Type key);
    void Tranplant(rbnode<Type>* u,rbnode<Type>* v);//将v树顶替u
    rbnode<Type>* Minimun(rbnode<Type>* x);//找到x为根结点的子树下的最小结点（最左结点）
    void Remove(rbnode<Type>* z);
    void Remove_Fixup(rbnode<Type>*,rbnode<Type>*,int,int);
    void destroy(rbnode<Type>* root);//销毁红黑树
};

#endif

