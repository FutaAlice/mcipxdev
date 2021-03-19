/**********************************************************************
 *<
   FILE: mcdevice.cpp

   DESCRIPTION: Devices for the motion manager

   CREATED BY: Wang Xiaobin

   HISTORY: 2021.03.19

 *>   Copyright (c) 2021, All Rights Reserved.
 **********************************************************************/

#include "mcipxdev.h"

// maxsdk header
#include "mcapdev.h"
#include "3dsmaxport.h"

// qt header
#include "ui_plugin_form.h"
#include "qmessagebox.h"
#include "qobject.h"

// definitions
#define IPX_DEVICE_CLASS_ID         Class_ID(0x475e5012, 0xa20a1115)
#define IPX_DEVICEBINDING_CLASS_ID  Class_ID(0x6cd5a295, 0x963dabe4)

#define IDS_IPX_DEVICENAME          L"iPhoneX Input Device"


// declear
static MCInputDevice* GetIpxDevice();

class IpxDeviceBinding : public MCDeviceBinding
{
public:
    IpxDeviceBinding() {}

public:
    // inherit functions
    MCInputDevice *GetDevice() { return GetIpxDevice(); }
    MSTR BindingName() { return L"iPhoneX AR Face Capture"; }
    float Eval(TimeValue t);
    void DeleteThis() { delete this; }
    virtual void AddRollup(IMCParamDlg *dlg);
    virtual void UpdateRollup(IRollupWindow *iRoll);
};

// dummy
float IpxDeviceBinding::Eval(TimeValue t) { return 0; }
void IpxDeviceBinding::AddRollup(IMCParamDlg *dlg) {}
void IpxDeviceBinding::UpdateRollup(IRollupWindow *iRoll) {}


class IpxDevice : public MCInputDevice
{
public:
    IpxDevice() {}

public:
    // inherit functions
    MSTR DeviceName() { return IDS_IPX_DEVICENAME; }
    MCDeviceBinding *CreateBinding() { return new IpxDeviceBinding; }
};

//--- Class Descriptor -----------------------------------------------

/*
// ClassDesc for Device Binding
class IpxDeviceBindingClassDesc :public ClassDesc
{
public:
    int				IsPublic() { return 0; }
    void*			Create(BOOL loading) { return new IpxDeviceBinding; }
    const TCHAR*	ClassName() { return GetString(IDS_KBD_DEVICENAME); }
    SClass_ID		SuperClassID() { return MOT_CAP_DEVBINDING_CLASS_ID; }
    Class_ID		ClassID() { return KBD_DEVICEBINDING_CLASS_ID; }
    const TCHAR*	Category() { return _T(""); }
};

static IpxDeviceBindingClassDesc ipxBindCD;
ClassDesc* GetIpxBindingClassDesc() { return &ipxBindCD; }
*/

// ClassDesc for Device
class IpxDeviceClassDesc : public ClassDesc
{
public:
    int				IsPublic() { return 1; }
    void*			Create(BOOL loading) { return GetIpxDevice(); }
    const TCHAR*	ClassName() { return IDS_IPX_DEVICENAME; }
    SClass_ID		SuperClassID() { return MOT_CAP_DEV_CLASS_ID; }
    Class_ID		ClassID() { return IPX_DEVICE_CLASS_ID; }
    const TCHAR*	Category() { return _T(""); }
};

ClassDesc* GetIpxDeviceClassDesc()
{
    static IpxDeviceClassDesc ipxDeviceCD; 
    return &ipxDeviceCD;
}

//--- iPhoneX device binding ---------------------------------------------------

static INT_PTR CALLBACK IpxDeviceDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static IpxDevice theIpxDevice;
static MCInputDevice *GetIpxDevice() { return &theIpxDevice; }

//--- iPhoneX device ---------------------------------------------------

class mcipxdev : public UtilityObj, public QObject
{
public:
    // Constructor/Destructor
    mcipxdev() : iu(nullptr) {}
    virtual ~mcipxdev() {}

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
    virtual Class_ID      ClassID() override { return IPX_DEVICE_CLASS_ID; }
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
