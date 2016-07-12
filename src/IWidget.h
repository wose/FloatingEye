#ifndef FLOATING_EYE_IWIDGET_H
#define FLOATING_EYE_IWIDGET_H

class IWidget
{
public:
    IWidget() = default;
    virtual ~IWidget() = default;

    virtual void draw() = 0;
};

#endif // FLOATING_EYE_IWIDGET_H
