#ifndef __QT_TESTNETWORKINTERFACESTATUS_H__
#define __QT_TESTNETWORKINTERFACESTATUS_H__


#include <QtCore/QObject>
#include <QtCore/QTimer>


namespace QT_TEST
{

  class QtTestNetworkInterfaceStatus : public QObject
  {
    Q_OBJECT

    // constructors
    public:
      QtTestNetworkInterfaceStatus(const QString &interfaceName);
      virtual ~QtTestNetworkInterfaceStatus() {}

    // public Qt slots
    public slots:
      void Monitor();

    // Qt signals
    signals:
      void NetworkInterfaceIsOK();
      void NetworkInterfaceIsKO();
      void NetworkInterfaceIsActive();
      void NetworkInterfaceIsInactive();

    // data members
    private:
      QString m_IPAddress;
      int m_NetMask;
      QTimer m_TimerMonitoring;
  };
}

#endif /* __QT_TESTNETWORKINTERFACESTATUS_H__ */
