//**************************************************************************/
// Copyright (c) 1998-2018 Autodesk, Inc.
// All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "mcipxdev.h"

#include "ui_plugin_form.h"
#include "qmessagebox.h"
#include "qobject.h"

#define mcipxdev_CLASS_ID Class_ID(0x475e5012, 0xa20a1115)

class mcipxdev : public UtilityObj, public QObject
{
public:
    // Constructor/Destructor
    mcipxdev();
    virtual ~mcipxdev();

    virtual void DeleteThis() override {}

    virtual void BeginEditParams(Interface *ip, IUtil *iu) override;
    virtual void EndEditParams(Interface *ip, IUtil *iu) override;

    virtual void Init(HWND hWnd);
    virtual void Destroy(HWND hWnd);

    // Singleton access
    static mcipxdev* GetInstance()
    {
        static mcipxdev themcipxdev;
        return &themcipxdev;
    }

private:
    void DoSomething();
    QWidget *widget;
    Ui::PluginRollup ui;
    IUtil* iu;
};


class mcipxdevClassDesc : public ClassDesc2
{
public:
    virtual int           IsPublic() override { return TRUE; }
    virtual void*         Create(BOOL /*loading = FALSE*/) override { return mcipxdev::GetInstance(); }
    virtual const TCHAR * ClassName() override { return GetString(IDS_CLASS_NAME); }
    virtual SClass_ID     SuperClassID() override { return UTILITY_CLASS_ID; }
    virtual Class_ID      ClassID() override { return mcipxdev_CLASS_ID; }
    virtual const TCHAR*  Category() override { return GetString(IDS_CATEGORY); }

    virtual const TCHAR*  InternalName() override { return _T("mcipxdev"); } // Returns fixed parsable name (scripter-visible name)
    virtual HINSTANCE     HInstance() override { return hInstance; } // Returns owning module handle


};

ClassDesc2* GetmcipxdevDesc()
{
    static mcipxdevClassDesc mcipxdevDesc;
    return &mcipxdevDesc;
}




//--- mcipxdev -------------------------------------------------------
mcipxdev::mcipxdev()
    : iu(nullptr)
{

}

mcipxdev::~mcipxdev()
{

}

void mcipxdev::BeginEditParams(Interface* ip, IUtil* iu)
{
    this->iu = iu;
    widget = new QWidget;
    ui.setupUi(widget);

    // We can connect UI signals here using Qt Functor syntax
    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &mcipxdev::DoSomething);
    ip->AddRollupPage(*widget, L"Plug-in Rollup");
}

void mcipxdev::EndEditParams(Interface* ip, IUtil*)
{
    this->iu = nullptr;
    ip->DeleteRollupPage(*widget);
}

void mcipxdev::Init(HWND /*handle*/)
{

}

void mcipxdev::Destroy(HWND /*handle*/)
{

}

void mcipxdev::DoSomething()
{
    int spin_value = ui.spinBox->value();
    QMessageBox::information(widget, "Dialog", QString("Spinner value: %1").arg(spin_value));
}
