#ifndef __AIRSET__H__
#define __AIRSET__H__

#include <airkit/Struct/airRBTree.h>
namespace air
{
    template <typename Type>
    class Set
    {
    private:
        // 集合节点
        struct Node : RBTreeNode<Node>
        {
            Type mData;

            Node() : RBTreeNode<Node>() {}
            Node(const Type &ele) : RBTreeNode<Node>() { constructor<Type>(&mData, ele); }
        };

        // 红黑树
        RBTree<Node> mTree;
        // 获取内存
        template <typename... Args>
        inline static Node *getNode(Args... args)
        {
            auto obj = (Node *)alloc(sizeof(Node));
            constructor<Node>(obj, args...);
            return obj;
        }
        // 释放内存
        inline static void freeNode(Node *block)
        {
            destructor<Node>(block);
            dealloc(block);
        }

        // 清空节点
        void _clear(Node *node)
        {
            if (node->getLeft() != nullptr)
                _clear(node->getLeft());
            if (node->getRight() != nullptr)
                _clear(node->getRight());
            freeNode(node);
        }

    public:
        // 前序迭代器,只读
        class Iterator
        {
            Node *mCurrent; // 当前节点
            // 友元类
            friend class Set;

            explicit Iterator(Node *begin) : mCurrent(begin) {}

        public:
            inline Iterator() : mCurrent(nullptr) {}
            inline ~Iterator() { mCurrent = nullptr; }

            // 操作符重载
            inline const Type &operator*() { return mCurrent->mData; }
            inline const Type *operator->() { return &mCurrent->mData; }

            // 前置 ++ 、 --
            inline Iterator &operator++()
            {
                if (mCurrent == nullptr)
                    return *this;
                if (mCurrent->getRight() != nullptr)
                {
                    mCurrent = mCurrent->getRight();
                    while (mCurrent && mCurrent->getLeft() != nullptr)
                        mCurrent = mCurrent->getLeft();
                }
                else if (mCurrent->isLeft() == true)
                    mCurrent = mCurrent->getParent();
                else
                {
                    while (mCurrent->isRight() == true)
                        mCurrent = mCurrent->getParent();

                    mCurrent = mCurrent->getParent();
                }
                return *this;
            }
            inline Iterator &operator--()
            {
                if (mCurrent == nullptr)
                    return *this;
                if (mCurrent->getLeft() != nullptr)
                {
                    mCurrent = mCurrent->getLeft();
                    while (mCurrent && mCurrent->getRight() != nullptr)
                        mCurrent = mCurrent->getRight();
                }
                else if (mCurrent->isRight() == true)
                    mCurrent = mCurrent->getParent();
                else
                {
                    while (mCurrent->isLeft() == true)
                        mCurrent = mCurrent->getParent();

                    mCurrent = mCurrent->getParent();
                }
                return *this;
            }
            // 后置 ++ 、 --
            inline Iterator operator++(int32)
            {
                Iterator ret = *this;
                ++*this;
                return ret;
            }
            inline Iterator operator--(int32)
            {
                Iterator ret = *this;
                --*this;
                return ret;
            }

            inline Iterator &operator=(const Iterator &rhs)
            {
                mCurrent = rhs.mCurrent;
                return *this;
            }
            inline bool operator==(const Iterator &rhs) { return mCurrent == rhs.mCurrent; }
            inline bool operator!=(const Iterator &rhs) { return mCurrent != rhs.mCurrent; }
        };

    public:
        // 节点初始化
        Set() { mTree.init(); }
        ~Set() { clear(); }

        // 获取节点数量
        inline uint getCount() { return mTree.mCount; }
        inline uint getSize() { return mTree.mCount; }

        inline Iterator begin() { return Iterator(mTree.getLeftmost()); }
        inline Iterator end() { return Iterator(nullptr); }
        inline Iterator getMax() { return Iterator(mTree.getRightmost()); }
        inline Iterator getRoot() { return Iterator(mTree.mRoot); }
        inline Iterator getMin() { return Iterator(mTree.getLeftmost()); }
        // 清空
        void clear()
        {
            // 释放内存
            if (mTree.mRoot != nullptr)
                _clear(mTree.mRoot);

            // 再次初始化树
            mTree.init();
        }

        // 是否为空
        inline bool isEmpty() { return mTree.mCount == 0; }
        // 插入
        bool insert(const Type &ele)
        {
            // 找到插入位置
            Node *iter = nullptr, *pos = mTree.mRoot;
            while (pos != nullptr)
            {
                iter = pos;
                if (pos->mData == ele) // 存在相同元素
                    return false;
                else if (pos->mData > ele)
                    pos = pos->getLeft();
                else
                    pos = pos->getRight();
            }
            // 分配节点
            Node *node = getNode(ele);
            // 插入数据
            node->setParent(iter);
            if (iter == nullptr) // 空树
                mTree.mRoot = node;
            else if (iter->mData > ele)
                iter->setLeft(node);
            else
                iter->setRight(node);
            // 初始化插入节点
            node->setLeft(nullptr);
            node->setRight(nullptr);
            node->setRed();
            // 维护红黑树性质
            mTree.fixupInsert(node);
            return true;
        }
        // 删除
        void remove(Iterator iter)
        {
            if (iter.mCurrent == nullptr)
                return;
            // 移除节点
            mTree.remove(iter.mCurrent);
            // 释放内存
            freeNode(iter.mCurrent);
        }
        // 查找,
        Iterator find(const Type &ele)
        {
            Node *node = mTree.mRoot;
            while (node != nullptr)
            {
                const Type &data = node->mData;
                if (data == ele)
                    return Iterator(node);
                else if (ele < data)
                    node = node->getLeft();
                else
                    node = node->getRight();
            }
            return Iterator(nullptr);
        }
    };

}
#endif //!__AIRSET__H__