#ifndef FLOATING_EYE_EMPTYWIDGET_H
#define FLOATING_EYE_EMPTYWIDGET_H

#include "IWidget.h"

#include <string>

class EmptyWidget :
    public IWidget
{
public:
    EmptyWidget(const std::string& name);

    virtual void draw() override;

private:
    std::string name_;
};

#endif // FLOATING_EYE_EMPTYWIDGET_H
