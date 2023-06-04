#ifndef __AIRLIST__H__
#define __AIRLIST__H__

#include <airkit/Struct/airIList.h>
#include <airkit/Core/airAlloc.h>
namespace air
{
    // 通用双向链表
    template <typename Type>
    class List
    {
    private:
        // 链表节点
        struct Node : public IListNode<Node>
        {
            Type mData; // 数据域
            inline Node() : IListNode<Node>() {}
            inline Node(const Type &ele) : IListNode<Node>() { constructor<Type>(&mData, ele); }
        };
        IList<Node> mList;        // 链表管理
        Alloctor<Node> mAlloctor; // 内存分配器

        // 获取内存
        template <typename... Args>
        inline Node *getNode(Args... args)
        {
            auto obj = mAlloctor.alloc(
#ifdef _check_memory_free
                this_file(), this_line(),
#endif
                args...);
            return obj;
        }
        // 释放内存
        inline void freeNode(Node *block)
        {
            mAlloctor.dealloc(block);
        }

    public:
        class ConstIterator; // 只读迭代器
        class Iterator
        {
            Node *mCurrent; // 当前节点
            // 友元类
            friend class List;
            friend class ConstIterator;
            explicit Iterator(Node *begin) : mCurrent(begin) {}

        public:
            inline Iterator() : mCurrent(nullptr) {}
            inline ~Iterator() { mCurrent = nullptr; }

            // 重载操作符
            // 前置 ++、--
            inline Iterator &operator++()
            {
                mCurrent = mCurrent->getNext();
                return *this;
            }
            inline Iterator &operator--()
            {
                mCurrent = mCurrent->getPrev();
                return *this;
            }
            // 后置 ++、--
            inline Iterator operator++(int32)
            {
                Iterator ret = *this;
                mCurrent = mCurrent->getNext();
                return ret;
            }
            inline Iterator operator--(int32)
            {
                Iterator ret = *this;
                mCurrent = mCurrent->getPrev();
                return ret;
            }
            inline Iterator &operator=(const Iterator &rhs)
            {
                mCurrent = rhs.mCurrent;
                return *this;
            }
            inline Iterator &operator=(const ConstIterator &rhs)
            {
                mCurrent = rhs.mCurrent;
                return *this;
            }

            inline bool operator==(const Iterator &rhs) { return mCurrent == rhs.mCurrent; }
            inline bool operator!=(const Iterator &rhs) { return mCurrent != rhs.mCurrent; }
            inline bool operator==(const ConstIterator &rhs) { return mCurrent == rhs.mCurrent; }
            inline bool operator!=(const ConstIterator &rhs) { return mCurrent != rhs.mCurrent; }

            inline Type &operator*() { return mCurrent->mData; }
            inline Type *operator->() { return &mCurrent->mData; }
        };
        class ConstIterator
        {
            Node *mCurrent; // 当前节点
            // 友元类
            friend class List;
            friend class Iterator;
            explicit ConstIterator(Node *begin) : mCurrent(begin) {}

        public:
            inline ConstIterator() : mCurrent(nullptr) {}
            inline ~ConstIterator() { mCurrent = nullptr; }

            // 重载操作符
            // 前置 ++、--
            inline const ConstIterator &operator++()
            {
                mCurrent = mCurrent->getNext();
                return *this;
            }
            inline const ConstIterator &operator--()
            {
                mCurrent = mCurrent->getPrev();
                return *this;
            }
            // 后置 ++、--
            inline const ConstIterator operator++(int32)
            {
                ConstIterator ret = *this;
                mCurrent = mCurrent->getNext();
                return ret;
            }
            inline const ConstIterator operator--(int32)
            {
                ConstIterator ret = *this;
                mCurrent = mCurrent->getPrev();
                return ret;
            }
            inline const ConstIterator &operator=(const Iterator &rhs)
            {
                mCurrent = rhs.mCurrent;
                return *this;
            }
            inline const ConstIterator &operator=(const ConstIterator &rhs)
            {
                mCurrent = rhs.mCurrent;
                return *this;
            }

            inline bool operator==(const Iterator &rhs) { return mCurrent == rhs.mCurrent; }
            inline bool operator!=(const Iterator &rhs) { return mCurrent != rhs.mCurrent; }
            inline bool operator==(const ConstIterator &rhs) { return mCurrent == rhs.mCurrent; }
            inline bool operator!=(const ConstIterator &rhs) { return mCurrent != rhs.mCurrent; }

            inline const Type &operator*() { return mCurrent->mData; }
            inline const Type *operator->() { return &mCurrent->mData; }
        };

    public:
        List() { mList.initList(); }
        ~List() { clear(); }

        // 获取节点数量
        uint getCount() const { return mList.getCount(); }
        uint getSize() const { return mList.getCount(); }

        // 是否为空
        Bool isEmpty() const { return mList.getCount() == 0 ? Bool::True : Bool::False; }

        // 获取第一个节点
        Type &entry() { return mList.getEntry().mData; }
        Type &front() { return mList.getEntry().mData; }
        // 获取最后一个节点
        Type &tail() { return mList.getTail().mData; }
        Type &back() { return mList.getTail().mData; }

        // 获取迭代器
        inline Iterator begin() { return Iterator(mList.getRoot()->getNext()); }
        inline ConstIterator cbegin() { return ConstIterator(mList.getRoot()->getNext()); }
        inline Iterator end() { return Iterator(mList.getRoot()); }
        inline ConstIterator cend() { return ConstIterator(mList.getRoot()); }

        // 清空
        void clear()
        {
            auto entry = mList.front();
            auto root = mList.getRoot();
            while (entry != root)
            {
                mList.removeEntry();
                freeNode(entry);
                entry = mList.front();
            }
        }
        // 插入
        template <typename... Args>
        Iterator insertEntry(const Args... ele)
        {
            auto node = getNode(ele...);
            mList.insertEntry(node);
            return Iterator(node);
        }
        // 插入
        template <typename... Args>
        Iterator insertTail(const Args... ele)
        {
            auto node = getNode(ele...);
            mList.insertTail(node);
            return Iterator(node);
        }
        template <typename... Args>
        Iterator insert(Iterator &iter, const Args... ele)
        {
            auto node = getNode(ele...);
            mList.insert(&(*iter), node);
            return Iterator(node);
        }

        // 删除
        void removeEntry()
        {
            auto node = mList.removeEntry();
            if (node != nullptr)
                freeNode(node);
        } // 删除
        void removeTail()
        {
            auto node = mList.removeTail();
            if (node != nullptr)
                freeNode(node);
        } // 删除
        void remove(Iterator &iter)
        {
            auto node = mList.remove(iter.mCurrent);
            if (node != nullptr)
                freeNode(node);
        }
    };
}

#endif //!__AIRLIST__H__