#ifndef __AIRRBTREE__H__
#define __AIRRBTREE__H__

#include <airkit/Core/airTypes.h>

namespace air
{
    // 红黑树节点,使用时请派生该模板
    template <typename Node>
    class RBTreeNode
    {
    public:
        // 颜色枚举
        enum class RBTClr : uint
        {
            Red,
            Black,
        };

    protected:
        Node *mParent; // 父节点
        Node *mLeft;   // 左子结点
        Node *mRight;  // 右子节点

        RBTClr mRBClr : 1;               // 红黑标记
        uint mFlag : (sizeof(uint) - 1); // 其他标记：用于继承节点扩展使用
    public:
        RBTreeNode()
            : mParent(nullptr), mLeft(nullptr),
              mRight(nullptr), mRBClr(RBTClr::Red), mFlag(0) {}

        inline void setParent(Node *node = nullptr) { mParent = node; }
        inline void setLeft(Node *node = nullptr) { mLeft = node; }
        inline void setRight(Node *node = nullptr) { mRight = node; }

        inline void setRed() { mRBClr = RBTClr::Red; }
        inline void setBlack() { mRBClr = RBTClr::Black; }
        inline void setColor(RBTClr clr) { mRBClr = clr; }

        inline Node *getParent() { return mParent; }
        inline Node *getLeft() { return mLeft; }
        inline Node *getRight() { return mRight; }
        inline RBTClr getColor() const { return mRBClr; }

        inline bool isRed() { return mRBClr == RBTClr::Red; }
        inline bool isBlack() { return mRBClr == RBTClr::Black; }

        inline bool isRoot() { return mParent == nullptr; }
        inline bool isLeft() { return mParent != nullptr && this == mParent->mLeft; }
        inline bool isRight() { return mParent != nullptr && this == mParent->mRight; }
        inline bool isLeaf() { return nullptr == mLeft && nullptr == mRight; }
        // 获取最左侧节点
        inline Node *getLeftmost()
        {
            auto ret = (Node *)this;
            while (ret != nullptr && ret->mLeft != nullptr)
                ret = ret->mLeft;
            return ret;
        }
        // 获取最右侧节点
        inline Node *getRightmost()
        {
            auto ret = (Node *)this;
            while (ret != nullptr && ret->mRight != nullptr)
                ret = ret->mRight;
            return ret;
        }
    };
    // 红黑数容器,Node派生自RBTreeNode
    template <typename Node>
    class RBTree
    {
    public:
        Node *mRoot; // 根节点
        uint mCount; // 节点计数
    public:
        inline RBTree() : mRoot(nullptr), mCount(0) {}
        inline void init() { mRoot = nullptr, mCount = 0; }

        inline Node *getLeftmost() { return mRoot != nullptr ? mRoot->getLeftmost() : nullptr; }
        inline Node *getRightmost() { return mRoot != nullptr ? mRoot->getRightmost() : nullptr; }
        // 旋转操作原理
        /*      |                                             |
         *      y       right-rotate(tree, y) ->              x
         *     / \                                           / \
         *    x   c     <- left-rotate(tree, x)             a   y
         *   / \                                               / \
         *  a   b                                             b   c
         */
        // 左旋
        inline void rotateLeft(Node *nodeX)
        {
            Node *rightY = nodeX->getRight();
            if (rightY != nullptr)
            {
                // 调整 x 的右节点和 y 的左节点
                nodeX->setRight(rightY->getLeft());
                if (rightY->getLeft() != nullptr)
                    rightY->getLeft()->setParent(nodeX);
                // 设置 x y 的父节点
                Node *parent = nodeX->getParent();
                rightY->setParent(parent);
                if (parent == nullptr) // x 为根节点
                    mRoot = rightY;
                else if (nodeX->isLeft() == true) // x 为左节点
                    parent->setLeft(rightY);
                else
                    parent->setRight(rightY); // x 为右节点
                // 放置 x 在 y 的左节点
                rightY->setLeft(nodeX);
                nodeX->setParent(rightY);
            }
        }
        // 右旋
        inline void rotateRight(Node *nodeY)
        {
            Node *leftX = nodeY->getLeft();
            if (leftX != nullptr)
            {
                // 调整 x  y 的子节点
                nodeY->setLeft(leftX->getRight());
                if (leftX->getRight() != nullptr)
                    leftX->getRight()->setParent(nodeY);
                // 设置 x y 的父节点
                Node *parent = nodeY->getParent();
                leftX->setParent(parent);
                if (parent == nullptr) // y 为根节点
                    mRoot = leftX;
                else if (nodeY->isLeft() == true) // x 为左节点
                    parent->setLeft(leftX);
                else
                    parent->setRight(leftX); // y 是右节点
                // 放置 y 在 x 的右节点
                nodeY->setParent(leftX);
                leftX->setRight(nodeY);
            }
        }

        // 插入时修护红黑树
        inline void fixupInsert(Node *node)
        {
            // 更新计数
            ++mCount;
            Node *parent = node->getParent();
            // 父亲一定存在的情况，叔叔存在/不存在 父亲结点颜色为红色
            while (parent != nullptr &&
                   parent->isRed() == true)
            {
                // 如果旋转节点的父节点是左节点
                if (parent->isLeft() == true)
                {
                    // 旋转节点的 右叔叔 存在且颜色为红
                    Node *runcle = parent->getParent()->getRight();
                    if (runcle != nullptr &&
                        runcle->isRed() == true)
                    {
                        // 情况 1：改变颜色
                        parent->setBlack();
                        runcle->setBlack();
                        parent->getParent()->setRed();
                        // 处理下一个节点
                        node = parent->getParent();
                        parent = node->getParent();
                    }
                    else
                    {
                        // 右节点
                        if (node->isRight() == true)
                        {
                            // 情况 2:左旋节点
                            node = parent;
                            parent = node->getParent();
                            rotateLeft(node);
                        }
                        // 情况 3
                        parent->setBlack();
                        if (parent->getParent() != nullptr)
                        {
                            parent->getParent()->setRed();
                            // 右旋节点
                            rotateRight(parent->getParent());
                        }
                    }
                }
                else // 如果旋转节点的父节点是右节点
                {
                    // 旋转节点的 右叔叔 存在且颜色为红
                    Node *luncle = parent->getParent()->getLeft();
                    if (luncle != nullptr &&
                        luncle->isRed() == true)
                    {
                        // 情况 1：改变颜色
                        parent->setBlack();
                        luncle->setBlack();
                        parent->getParent()->setRed();
                        // 处理下一个节点
                        node = parent->getParent();
                        parent = node->getParent();
                    }
                    else
                    {
                        // 左节点
                        if (node->isLeft() == true)
                        {
                            // 情况 2:右旋节点
                            node = parent;
                            parent = node->getParent();
                            rotateRight(node);
                        }
                        // 情况 3
                        parent->setBlack();
                        if (parent->getParent() != nullptr)
                        {
                            parent->getParent()->setRed();
                            // 右旋节点
                            rotateLeft(parent->getParent());
                        }
                    }
                }
            }
            // 根节点置黑
            mRoot->setBlack();
        }
        // 移除时时修护红黑树
        inline void fixupRemove(Node *node)
        {
            while (node != nullptr &&
                   node != mRoot &&
                   node->isBlack() == true)
            {
                Node *parent = node->getParent();
                // node 是左节点
                if (node->isLeft() == true)
                {
                    Node *rbros = parent->getRight();
                    if (rbros == nullptr)
                        return;

                    // node的兄弟节点 runcle 是红色
                    if (rbros->isRed() == true) // A: case 1
                    {
                        rbros->setBlack();              // 置黑
                        rbros->getParent()->setRed();   // 置红
                        rotateLeft(rbros->getParent()); // 父节点左旋
                        rbros = parent->getRight();
                        if (rbros == nullptr)
                            return;
                    }
                    // node的兄弟节点 rbros 是黑色，且兄弟节点的两个孩子节点为黑色
                    if ((rbros->getLeft() == nullptr || rbros->getLeft()->isBlack() == true) &&
                        (rbros->getRight() == nullptr || rbros->getRight()->isBlack() == true)) // A:case 2
                    {
                        // 兄弟节点置红
                        rbros->setRed();
                        node = parent;
                        parent = parent->getParent();
                    }
                    else
                    {
                        // node的兄弟节点 rbros是黑色，但是 rbros 的右节点是黑色
                        if (rbros->getRight() == nullptr ||
                            rbros->getRight()->isBlack() == true) // A:case 3
                        {
                            if (rbros->getLeft() != nullptr)
                                rbros->getLeft()->setBlack();
                            rbros->setRed();
                            rotateRight(rbros);
                            rbros = parent->getRight();
                        }
                        rbros->setColor(parent->getColor()); // A:case 4
                        parent->setBlack();
                        if (rbros->getRight() != nullptr)
                            rbros->getRight()->setBlack();
                        rotateLeft(parent);
                        node = mRoot;
                    }
                }
                // node 是右节点
                else
                {
                    Node *lbros = parent->getLeft();
                    if (lbros == nullptr)
                        return;
                    // node的兄弟节点 lbros 是红色
                    if (lbros->isRed() == true) // B: case 1
                    {
                        lbros->setBlack();              // 置黑
                        lbros->getParent()->setRed();   // 置红
                        rotateRight(lbros->getParent()); // 父节点左旋
                        lbros = parent->getLeft();
                        if (lbros == nullptr)
                            return;
                    }
                    // node的兄弟节点 lbros 是黑色，且兄弟节点的两个孩子节点为黑色
                    if ((lbros->getLeft() == nullptr || lbros->getLeft()->isBlack() == true) &&
                        (lbros->getRight() != nullptr && lbros->getRight()->isBlack() == true)) // B: case 2
                    {
                        // 兄弟节点置红
                        lbros->setRed();
                        node = parent;
                    }
                    else
                    {
                        // node的兄弟节点 lbros是黑色，但是 lbros 的右节点是红色
                        if (lbros->getLeft() == nullptr || lbros->getLeft()->isBlack() == true) // B: case 3
                        {
                            if (lbros->getRight() != nullptr)
                                lbros->getRight()->setBlack();
                            lbros->setRed();
                            rotateLeft(lbros);
                            lbros = parent->getLeft();
                        }
                        // B: case 4
                        lbros->setColor(parent->getColor());
                        parent->setBlack();
                        if (lbros->getLeft() != nullptr)
                            lbros->getLeft()->setBlack();
                        rotateRight(parent);
                        node = mRoot;
                    }
                }
            }
            node->setBlack();
        }
        // 移除节点
        inline void remove(Node *node)
        {
            Node *x = nullptr;             // 后继节点
            auto color = node->getColor(); // 移除节点颜色

            // 待删节点，左子树为空
            if (node->getLeft() == nullptr)
            {
                x = node->getRight();
                // 删除节点操作
                if (node->isRoot() == true) // 删除节点node是根节点
                    mRoot = x;
                else if (node->isLeft() == true) // node是左节点
                    node->getParent()->setLeft(x);
                else // node是右节点
                    node->getParent()->setRight(x);
                // 设置后继节点的父节点
                if (x != nullptr)
                    x->setParent(node->getParent());

                node->setParent(nullptr);
                node->setRight(nullptr);
            } // 待删节点，右子树为空
            else if (node->getRight() == nullptr)
            {
                x = node->getLeft();
                // 删除节点操作
                if (node->isRoot() == true) // 删除节点node是根节点
                    mRoot = x;
                else if (node->isLeft() == true) // node是左节点
                    node->getParent()->setLeft(x);
                else // node是右节点
                    node->getParent()->setRight(x);
                // 设置后继节点的父节点
                if (x != nullptr)
                    x->setParent(node->getParent());

                node->setParent(nullptr);
                node->setLeft(nullptr);
            } // 左右子树都不为空
            else
            {
                // 寻找node的右子树的最左节点
                Node *y = node->getRight();
                x = y;
                while (x != nullptr &&
                       x->getLeft() != nullptr)
                    x = x->getLeft();

                color = x->getColor();
                // 后继节点 为 node->mRight

                // 删除node节点操作
                //  x->SetParent(node->GetParent());
                if (node->isRoot() == true) // node是根节点
                    mRoot = y;
                else if (node->isLeft() == true) // node是左节点
                    node->getParent()->setLeft(y);
                else // node是右节点
                    node->getParent()->setRight(y);
                // 设置后继节点的父节点
                y->setParent(node->getParent());

                // 链接node的左子树
                x->setLeft(node->getLeft());
                node->getLeft()->setParent(x);
                x->setColor(node->getColor());

                node->setParent(nullptr);
                node->setLeft(nullptr);
                node->setRight(nullptr);
            }
            // 更新计数
            --mCount;
            // 颜色为黑，调整红黑树性质
            if (x != nullptr && color == Node::RBTClr::Black)
                fixupRemove(x);
        }
    };
}
#endif //!__AIRRBTREE__H__