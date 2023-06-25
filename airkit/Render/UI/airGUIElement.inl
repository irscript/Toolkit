
// 改变位置
inline void IGUIElement::setPos(Vec2f &pos)
{
    Vec2f offset(mArea.width(), mArea.height());
    mArea.mLT = pos;
    mArea.mRB += offset;
}
// 改变尺寸
inline void IGUIElement::setSize(Vec2f &pos)
{
    mArea.mRB = mArea.mLT + pos;
    onSize(mArea.width(), mArea.height());
}

// 移动控件
inline void IGUIElement::moveTo(const Rectf &rect)
{
    // 判断是否大小是否改变
    bool update = rect.width() != mArea.width() || rect.height() != mArea.height();
    mArea = rect;
    if (update == true)
        onSize(mArea.width(), mArea.height());
}

// 设置父控件
void IGUIElement::setParent(IGUIElement *parent)
{
    if (mParent != nullptr)
        removeChild(this);
    mParent = parent;
    if (mParent != nullptr)
        addChild(this);
}
// 移除子控件
void IGUIElement::removeChild(IGUIElement *child)
{
    for (auto it = mChildUI.front(); it != mChildUI.getRoot(); it = it->getNext())
        if (it == child)
        {
            mChildUI.remove(it);
            it->drop();
            return;
        }
}
// 添加子控件
void IGUIElement::addChild(IGUIElement *child)
{
    // 确保控件不为空且不能是窗口
    make_ensure(child != nullptr && child->isWindow() == false);
    // 验证子控件不能已经存在
    for (auto it = mChildUI.front(); it != mChildUI.getRoot(); it = it->getNext())
        if (it == child)
            return;

    // 添加控件
    mChildUI.insertEntry(child);
    // 引用一下
    child->grab();
}
// 子控件置顶
void IGUIElement::topChild(IGUIElement *child)
{ // 验证子控件是否在该列表
    for (auto it = mChildUI.front(); it != mChildUI.getRoot(); it = it->getNext())
        if (it == child)
        {
            mChildUI.remove(it);
            mChildUI.insertEntry(child);
            return;
        }
}

// 从屏幕坐标转窗口坐标
void IGUIElement::screenToWindow(Vec2f &pos) const
{
    const auto win = getWindow();
    if (win != nullptr)
        pos -= win->mArea.mLT;
}
// 从窗口坐标转屏幕坐标
void IGUIElement::windowToScreen(Vec2f &pos) const
{
    const auto win = getWindow();
    if (win != nullptr)
        pos += win->mArea.mLT;
}
// 从窗口坐标转到客户坐标
void IGUIElement::windowToClient(Vec2f &pos) const
{
    const IGUIElement *win = this;
    while (win->mParent != nullptr)
    {
        pos -= win->mArea.mLT;
        win = win->mParent;
    }
}
// 从客户坐标转到窗口坐标
void IGUIElement::clientToWindow(Vec2f &pos) const
{
    const IGUIElement *win = this;
    while (win->mParent != nullptr)
    {
        pos += win->mArea.mLT;
        win = win->mParent;
    }
}

// 从屏幕坐标转客户坐标
void IGUIElement::screenToClient(Vec2f &pos) const
{
    const IGUIElement *win = this;
    while (win != nullptr)
    {
        pos += win->mArea.mLT;
        win = win->mParent;
    }
}
// 从客户坐标转屏幕坐标
void IGUIElement::clientToScreen(Vec2f &pos) const
{
    const IGUIElement *win = this;
    while (win = nullptr)
    {
        pos += win->mArea.mLT;
        win = win->mParent;
    }
}

// 获取关联的窗口
IWindow *IGUIElement::getWindow() const
{
    const IGUIElement *win = this;
    while (win != nullptr)
    {
        if (win->isWindow())
            break;
        win = win->mParent;
    }
    return (IWindow *)win;
}