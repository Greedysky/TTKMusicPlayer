#include <QSettings>
#include <qmmp/qmmp.h>
#include <initguid.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <mmreg.h>
#include <functiondiscoverykeys.h>
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
#ifdef Q_OS_UNIX
    for(QAbstractButton *button : m_ui.buttonBox->buttons())
    {
        button->setFocusPolicy(Qt::NoFocus);
    }
#endif
    enumDevices();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString id = settings.value("WASAPI/device", "default").toString();
    int index = m_ui.deviceComboBox->findData(id);
    m_ui.deviceComboBox->setCurrentIndex(qMax(index, 0));
    m_ui.bufferSizeSpinBox->setValue(settings.value("WASAPI/buffer_size", 1000).toInt());
    m_ui.exclusiveModeCheckBox->setChecked(settings.value("WASAPI/exclusive_mode", false).toBool());
}

void SettingsDialog::accept()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    int index = m_ui.deviceComboBox->currentIndex();
    settings.setValue("WASAPI/device", m_ui.deviceComboBox->itemData(index).toString());
    settings.setValue("WASAPI/buffer_size", m_ui.bufferSizeSpinBox->value());
    settings.setValue("WASAPI/exclusive_mode", m_ui.exclusiveModeCheckBox->isChecked());
    QDialog::accept();
}

void SettingsDialog::enumDevices()
{
    m_ui.deviceComboBox->clear();
    m_ui.deviceComboBox->addItem(tr("Default"), "default");

    IMMDeviceEnumerator *pEnumerator = nullptr;
    HRESULT result = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    if(result != S_OK)
    {
        qWarning("SettingsDialog: CoCreateInstance failed, error code = 0x%lx", result);
        pEnumerator = nullptr;
    }

    IMMDeviceCollection *pEndpoints = nullptr;
    IMMDevice *pEndpoint = nullptr;
    LPWSTR pwszID = nullptr;
    IPropertyStore *pProps = nullptr;
    UINT count = 0;

    result = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pEndpoints);
    if(result != S_OK)
    {
        qWarning("SettingsDialog: IMMDeviceEnumerator::EnumAudioEndpoints failed, error code = 0x%lx", result);
        pEndpoints = nullptr;
    }

    if(pEndpoints)
    {
        pEndpoints->GetCount(&count);
        if(result != S_OK)
        {
            qWarning("SettingsDialog: IMMDeviceCollection::GetCount failed, error code = 0x%lx", result);
            count = 0;
        }

        for(UINT i = 0; i < count; ++i)
        {
            result = pEndpoints->Item(i, &pEndpoint);
            if(result != S_OK)
            {
                qWarning("SettingsDialog: IMMDeviceCollection::Item failed, error code = 0x%lx", result);
                pEndpoint = nullptr;
                break;
            }

            result = pEndpoint->GetId(&pwszID);
            if(result != S_OK)
            {
                qWarning("SettingsDialog: IMMDevice::GetId failed, error code = 0x%lx", result);
                pwszID = nullptr;
                break;
            }

            result = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);
            if(result != S_OK)
            {
                qWarning("SettingsDialog: IMMDevice::GetId failed, error code = 0x%lx", result);
                pProps = nullptr;
                break;
            }

            static PROPERTYKEY key;
            key.pid = 14;
            key.fmtid = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };

            PROPVARIANT varName;
            PropVariantInit(&varName);  // Initialize container for property value.
            result = pProps->GetValue(key, &varName);  // Get the endpoint's friendly-name property.
            if(result != S_OK)
            {
                qWarning("SettingsDialog: IMMDevice::GetId failed, error code = 0x%lx", result);
                PropVariantClear(&varName);
                break;
            }

            m_ui.deviceComboBox->addItem(QString::fromWCharArray(varName.pwszVal), QString::fromWCharArray(pwszID));

            CoTaskMemFree(pwszID);
            pwszID = nullptr;
            PropVariantClear(&varName);
            pProps->Release();
            pProps = nullptr;
            pEndpoint->Release();
            pEndpoint = nullptr;
        }
    }

    if(pProps)
    {
        pProps->Release();
        pProps = nullptr;
    }

    if(pwszID)
    {
        CoTaskMemFree(pwszID);
        pwszID = nullptr;
    }

    if(pEndpoint)
    {
        pEndpoint->Release();
        pEndpoint = nullptr;
    }

    if(pEndpoints)
    {
        pEndpoints->Release();
        pEndpoints = nullptr;
    }

    if(pEnumerator)
    {
       pEnumerator->Release();
       pEnumerator = nullptr;
    }
}
