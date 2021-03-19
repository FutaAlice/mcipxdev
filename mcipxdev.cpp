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
float IpxDeviceBinding::Eval(TimeValue) { return 0; }
void IpxDeviceBinding::AddRollup(IMCParamDlg *) {}
void IpxDeviceBinding::UpdateRollup(IRollupWindow *) {}


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

// ClassDesc for Device
class IpxDeviceClassDesc : public ClassDesc2
{
public:
    int				IsPublic() { return 1; }
    void*			Create(BOOL loading) { return GetIpxDevice(); }
    const TCHAR*	ClassName() { return IDS_IPX_DEVICENAME; }
    SClass_ID		SuperClassID() { return MOT_CAP_DEV_CLASS_ID; }
    Class_ID		ClassID() { return IPX_DEVICE_CLASS_ID; }
    const TCHAR*	Category() { return _T(""); }
};

ClassDesc2* GetIpxDeviceClassDesc()
{
    static IpxDeviceClassDesc ipxDeviceCD; 
    return &ipxDeviceCD;
}

//--- iPhoneX device binding ---------------------------------------------------

static INT_PTR CALLBACK IpxDeviceDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static MCInputDevice *GetIpxDevice()
{
    static IpxDevice theIpxDevice;
    return &theIpxDevice;
}

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
