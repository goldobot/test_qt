#include "QtTestNetworkInterfaceStatus.h"

#include <QtNetwork/QNetworkInterface>
#include <QtCore/QSettings>


namespace QT_TEST
{
  QtTestNetworkInterfaceStatus::QtTestNetworkInterfaceStatus(const QString &/*interfaceName*/)
  {

    m_IPAddress = QString("192.168.56.101");
    m_NetMask = 32;

    connect(&m_TimerMonitoring, SIGNAL(timeout()), this, SLOT(Monitor()));

    m_TimerMonitoring.start(500);
  }


  void
  QtTestNetworkInterfaceStatus::Monitor()
  {
    QNetworkInterface *modemInterface = NULL;
    QList<QNetworkInterface> networkInterfaces = QNetworkInterface::allInterfaces();
    for (int indexInterface = 0 ; ((indexInterface < networkInterfaces.size()) && (modemInterface == NULL)) ; ++indexInterface)
    {
      QList<QNetworkAddressEntry> interfaceAddresses = networkInterfaces[indexInterface].addressEntries();
      for (int indexAddress = 0 ; ((indexAddress < interfaceAddresses.size()) && (modemInterface == NULL)) ; ++indexAddress)
      {
        if (interfaceAddresses[indexAddress].ip().isInSubnet(QHostAddress(m_IPAddress), m_NetMask))
        {
          modemInterface = &networkInterfaces[indexInterface];
        }
      }
    }

    if (modemInterface == NULL)
    {
      emit NetworkInterfaceIsKO();
      return;
    }
    else
    {
      emit NetworkInterfaceIsOK();
    }

    bool isActive = (bool) (modemInterface->flags() & QNetworkInterface::IsUp);

    if (isActive)
    {
      emit NetworkInterfaceIsActive();
    }
    else
    {
      emit NetworkInterfaceIsInactive();
    }
  }
}
