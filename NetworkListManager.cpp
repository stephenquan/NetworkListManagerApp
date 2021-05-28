//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "NetworkListManager.h"
#include <QMetaEnum>
#include <QDebug>

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define CHECKHR(stmt) \
    hr = (stmt); \
    if (FAILED(hr)) \
    { \
        return hr; \
    }

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkListManager::NetworkListManager(QObject* parent) :
    QObject(parent),
    m_Cookie(0),
    m_Connectivity(ConnectivityDisconnected)
{
    Init();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

NetworkListManager::~NetworkListManager()
{
    UnInit();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

HRESULT NetworkListManager::Init()
{
    HRESULT hr = S_OK;
    CHECKHR(m_NetworkListManager.CoCreateInstance(CLSID_NetworkListManager));
    CHECKHR(m_NetworkListManager->QueryInterface(&m_ConnectionPointContainer));
    CHECKHR(m_ConnectionPointContainer->FindConnectionPoint(IID_INetworkListManagerEvents, &m_ConnectionPoint));
    CHECKHR(m_ConnectionPoint->Advise((IUnknown*)(this), &m_Cookie));
    NLM_CONNECTIVITY connectivity = NLM_CONNECTIVITY_DISCONNECTED;
    CHECKHR(m_NetworkListManager->GetConnectivity(&connectivity));
    setConnectivity(static_cast<ConnectivityFlags>(connectivity));
    return hr;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

HRESULT NetworkListManager::UnInit()
{
    HRESULT hr = S_OK;
    if (m_ConnectionPoint)
    {
        if (m_Cookie)
        {
            CHECKHR(m_ConnectionPoint->Unadvise(m_Cookie));
            m_Cookie = 0;
        }
        m_ConnectionPoint = nullptr;
    }
    m_ConnectionPointContainer = nullptr;
    m_NetworkListManager = nullptr;
    return hr;
}
//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

STDMETHODIMP NetworkListManager::ConnectivityChanged(NLM_CONNECTIVITY connectivity)
{
    setConnectivity(static_cast<ConnectivityFlags>(connectivity));
    return S_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void NetworkListManager::setConnectivity(ConnectivityFlags connectivity)
{
    qDebug() << Q_FUNC_INFO << connectivity;

    if (connectivity == m_Connectivity)
    {
        return;
    }

    m_Connectivity = connectivity;

    emit connectivityChanged();
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

QString NetworkListManager::connectivityString() const
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ConnectivityFlag>();
    ConnectivityFlags _connectivity = connectivity();
    if (!_connectivity)
    {
        return metaEnum.valueToKey(0);
    }
    QStringList stringList;
    for (int index = 0; index < metaEnum.keyCount(); index++)
    {
        int flag = metaEnum.value(index);
        if ((_connectivity & flag) != 0)
        {
            stringList.append(metaEnum.valueToKey(flag));
        }
    }
    return stringList.join("|");
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
