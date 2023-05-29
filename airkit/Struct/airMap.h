#ifndef __AIRMAP__H__
#define __AIRMAP__H__

#include <airkit/Struct/airRBTree.h>

namespace air
{
    template <typename Key, typename Value>
    class Map
    {
    private:
        // 映射节点
        struct Node : RBTreeNode<Node>
        {
            Key mKey;
            Value mVal;

            Node() : RBTreeNode<Node>() {}
            Node(const Key &key, const Value &val) : RBTreeNode<Node>()
            {
                constructor<Key>(&mKey, key);
                constructor<Value>(&mVal, val);
            }
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
            friend class Map;

            explicit Iterator(Node *begin) : mCurrent(begin) {}

        public:
            inline Iterator() : mCurrent(nullptr) {}
            inline ~Iterator() { mCurrent = nullptr; }

            inline const Key &key() { return mCurrent->mKey; }
            inline const Value &value() { return mCurrent->mVal; }

            inline const Key &getKey() { return mCurrent->mKey; }
            inline const Value &getValue() { return mCurrent->mVal; }
            // 操作符重载
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

            inline Node &operator*() { return *mCurrent; }
        };

    public:
        inline Map() { mTree.init(); }
        inline ~Map() { clear(); }

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
        bool insert(const Key &key, const Value &val)
        {
            // 找到插入位置
            Node *iter = nullptr, *pos = mTree.mRoot;
            while (pos != nullptr)
            {
                iter = pos;
                if (pos->mKey == key) // 存在相同元素
                    return false;
                else if (pos->mKey > key)
                    pos = pos->getLeft();
                else
                    pos = pos->getRight();
            }
            // 分配节点
            Node *node = getNode(key, val);
            // 插入数据
            node->setParent(iter);
            if (iter == nullptr) // 空树
                mTree.mRoot = node;
            else if (iter->mKey > key)
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
        // 查找
        Iterator find(const Key &key)
        {
            Node *node = mTree.mRoot;
            while (node != nullptr)
            {
                const Key &keys = node->mKey;
                if (keys == key)
                    return Iterator(node);
                else if (key < keys)
                    node = node->getLeft();
                else
                    node = node->getRight();
            }
            return Iterator(nullptr);
        }
        // 更新,没有就插入数据
        bool update(const Key &key, const Value &val)
        {
            auto it = Find(key);
            // 不存在，就插入
            if (it.mCurrent == nullptr)
                return Insert(key, val);
            // 更新值域
            it.mCurrent->mVal = val;
            return true;
        }
        // 移除数据
        void remove(Iterator iter)
        {
            if (iter.mCurrent == nullptr)
                return;
            // 移除节点
            mTree.remove(iter.mCurrent);
            // 释放内存
            freeNode(iter.mCurrent);
        }
        // 移除数据
        inline void remove(const Key &key)
        {
            auto it = find(key);
            // 不存在，就插入
            if (it.mCurrent != nullptr)
                remove(it);
        }
    };

}
#endif //!__AIRMAP__H__