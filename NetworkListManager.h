#ifndef __NetworkListManager__
#define __NetworkListManager__

#include <qt_windows.h>
#include <QObject>
#include <netlistmgr.h>
#include <atlbase.h>

class NetworkListManager : public QObject, public INetworkListManagerEvents
{
    Q_OBJECT

    Q_PROPERTY(ConnectivityFlags connectivity READ connectivity NOTIFY connectivityChanged)
    Q_PROPERTY(QString connectivityString READ connectivityString NOTIFY connectivityChanged)

public:
    NetworkListManager(QObject* parent = nullptr);
    virtual ~NetworkListManager();

    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }
    STDMETHODIMP QueryInterface(REFIID riid, void** obj)
    {
        if (riid == IID_IUnknown || riid == IID_INetworkListManagerEvents)
        {
            *obj = dynamic_cast<INetworkListManagerEvents*>(this);
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    STDMETHOD(ConnectivityChanged)(NLM_CONNECTIVITY connectivity);

    enum ConnectivityFlag
    {
        ConnectivityDisconnected = NLM_CONNECTIVITY_DISCONNECTED,
        ConnectivityIPV4NoTraffic = NLM_CONNECTIVITY_IPV4_NOTRAFFIC,
        ConnectivityIPV6NoTraffic = NLM_CONNECTIVITY_IPV6_NOTRAFFIC,
        ConnectivityIPV4Subnet = NLM_CONNECTIVITY_IPV4_SUBNET,
        ConnectivityIPV4LocalNetwork = NLM_CONNECTIVITY_IPV4_LOCALNETWORK,
        ConnectivityIPV4Internet = NLM_CONNECTIVITY_IPV4_INTERNET,
        ConnectivityIPV6Subnet = NLM_CONNECTIVITY_IPV6_SUBNET,
        ConnectivityIPV6LocalNetwork = NLM_CONNECTIVITY_IPV6_LOCALNETWORK,
        ConnectivityIPV6Internet = NLM_CONNECTIVITY_IPV6_INTERNET
    };
    Q_DECLARE_FLAGS(ConnectivityFlags, ConnectivityFlag)
    Q_FLAGS(ConnectivityFlags)

    Q_ENUM(ConnectivityFlag)

signals:
    void connectivityChanged();

protected:
    CComPtr<IConnectionPoint>           m_ConnectionPoint;
    CComPtr<INetworkListManager>        m_NetworkListManager;
    CComPtr<IConnectionPointContainer>  m_ConnectionPointContainer;
    DWORD m_Cookie;
    ConnectivityFlags m_Connectivity;

    HRESULT Init();
    HRESULT UnInit();

    ConnectivityFlags connectivity() const { return m_Connectivity; }
    void setConnectivity(ConnectivityFlags connectivity);

    QString connectivityString() const;

};

#endif
