/* Copyright (C) 2016 Teng K. J.
   All rights reserved.
*/
#ifndef _0T_BUTTON_H
#define _0T_BUTTON_H

#include "BaseWindow.h"

namespace tl {

class Button: public BaseWindow {
public:
    /** Default constructor */
    Button();
    /** Default destructor */
    ~Button();

    Button(BaseWindow *parent);
    Button(BaseWindow *parent, LPCTSTR name, SizeAndPos sp, bool visible = true);

    int Create(BaseWindow *parent, LPCTSTR name, SizeAndPos sp, bool visible = true);
    void Destroy();

    BaseWindow *GetParent() {return parent;}
    const BaseWindow *GetParent() const {return parent;}

    virtual void OnClick() {}
protected:
private:
    BaseWindow *parent;
};

}

#endif // _0T_BUTTON_H
