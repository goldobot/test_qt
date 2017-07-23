#include "QtTestComponentManager.h"
#include "QtTestFrame.h"


#include <QtCore/QProcess>
#include <QtCore/QStateMachine>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtGui/QProgressDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtNetwork/QNetworkInterface>



namespace QT_TEST
{
  QtTestComponentManager::QtTestComponentManager()
  :m_NetworkInterfaceStatus("m_NetworkInterfaceStatus")
  {
    m_NetworkInterfaceStatusThread.setObjectName("m_NetworkInterfaceStatusThread");
    m_NetworkInterfaceStatus.moveToThread(&m_NetworkInterfaceStatusThread);

    connect(&m_NetworkInterfaceStatus, SIGNAL(NetworkInterfaceIsActive()), this, SLOT(ForwardNetworkInterfaceIsActive()));
    connect(&m_NetworkInterfaceStatus, SIGNAL(NetworkInterfaceIsInactive()), this, SLOT(ForwardNetworkInterfaceIsInactive()));

    connect(this, SIGNAL(NetworkInterfaceStatusAlarmChanged(int)), &m_Frame, SLOT(SetNetworkInterfaceStatusAlarm(int)));

    m_NetworkInterfaceStatusThread.start();

    m_Frame.show();
  }


  QtTestComponentManager::~QtTestComponentManager()
  {
    m_NetworkInterfaceStatusThread.quit();
    m_NetworkInterfaceStatusThread.wait();
  }


  void
  QtTestComponentManager::ForwardNetworkInterfaceIsActive()
  {
    emit NetworkInterfaceStatusAlarmChanged(1);
  }


  void
  QtTestComponentManager::ForwardNetworkInterfaceIsInactive()
  {
    emit NetworkInterfaceStatusAlarmChanged(-1);
  }


}
