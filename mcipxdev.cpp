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
#define IDS_IPX_DEVICE_BINDING_NAME L"iPhoneX AR Face Capture"
#define IDS_IPX_DEVICE_NAME         L"iPhoneX Input Device"


// declear
static MCInputDevice* GetIpxDevice();

class IpxDeviceBinding : public MCDeviceBinding, public QObject
{
public:
    IpxDeviceBinding() {}

public:
    // inherit from MCDeviceBinding
    MCInputDevice *GetDevice() { return GetIpxDevice(); }
    MSTR BindingName() { return IDS_IPX_DEVICE_BINDING_NAME; }
    float Eval(TimeValue t);
    void DeleteThis() { delete this; }
    void AddRollup(IMCParamDlg *dlg);
    void UpdateRollup(IRollupWindow *iRoll);

private:
    void DoSomething();

private:
    QWidget *widget;
    Ui::PluginRollup ui;
};

class IpxDevice : public MCInputDevice
{
public:
    IpxDevice() {}

public:
    // inherit functions
    MSTR DeviceName() { return IDS_IPX_DEVICE_NAME; }
    MCDeviceBinding *CreateBinding() { return new IpxDeviceBinding; }
};

//--- Class Descriptor -----------------------------------------------

// ClassDesc for Device
class IpxDeviceClassDesc : public ClassDesc2
{
public:
    int				IsPublic() { return 1; }
    void*			Create(BOOL loading) { return GetIpxDevice(); }
    const TCHAR*	ClassName() { return IDS_IPX_DEVICE_NAME; }
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

#include <chrono>
#include <cmath>

float IpxDeviceBinding::Eval(TimeValue t)
{

    auto d = std::chrono::system_clock::now().time_since_epoch();
    auto mil = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    auto x = std::sinf(mil.count() / 1000.0f);

    //float val = 0.0f;
    //if (which == MC_MOUSE_X) {
    //    return theMouseDevice.DX() * scale * (invert ? -1.0f : 1.0f);
    //}
    //else {
    //    return theMouseDevice.DY() * scale * (invert ? -1.0f : 1.0f);
    //}
    return x;
}

//RefTargetHandle IpxDeviceBinding::Clone(RemapDir& remap)
//{
//    IpxDeviceBinding *b = new IpxDeviceBinding;
//    b->which = which;
//    b->invert = invert;
//    b->scale = scale;
//    BaseClone(this, b, remap);
//    return b;
//}

//void IpxDeviceBinding::BeginActivate(BOOL reset)
//{
//    if (reset) theMouseDevice.ZeroMouse();
//}

void IpxDeviceBinding::AddRollup(IMCParamDlg *dlg)
{
    widget = new QWidget;
    ui.setupUi(widget);
    QObject::connect(ui.pushButton, &QPushButton::clicked, this, &IpxDeviceBinding::DoSomething);


    dlg->iRoll->AppendRollup(*widget, IDS_IPX_DEVICE_NAME, APPENDROLL_CLOSED, ROLLUP_CAT_STANDARD);
}

void IpxDeviceBinding::UpdateRollup(IRollupWindow *iRoll)
{
}

void IpxDeviceBinding::DoSomething()
{
    int spin_value = ui.spinBox->value();
    QMessageBox::information(widget, "Dialog", QString("Spinner value: %1").arg(spin_value));
}


//--- iPhoneX device ---------------------------------------------------


//--- mcipxdev -------------------------------------------------------

//void mcipxdev::EndEditParams(Interface* ip, IUtil*)
//{
//    this->iu = nullptr;
//    ip->DeleteRollupPage(*widget);
//}
