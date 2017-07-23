#ifndef __QT_TESTCOMPONENTMANAGER_H__
#define __QT_TESTCOMPONENTMANAGER_H__

#include "QtTestFrame.h"
#include "QtTestNetworkInterfaceStatus.h"

#include <QtGui/QLabel>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGui/QProgressDialog>


namespace QT_TEST
{
  class QtTestComponentManager : public QObject
  {
    Q_OBJECT

    // constructors
    public:
      QtTestComponentManager();
      virtual ~QtTestComponentManager();

    // public Qt slots
    public slots:
      void ForwardNetworkInterfaceIsActive();
      void ForwardNetworkInterfaceIsInactive();

    // Qt signals
    signals:
      void NetworkInterfaceStatusAlarmChanged(int);

    // data members
    private:

      // Core Components

      // File System Components

      // GUI Components
      QtTestFrame m_Frame;

      // Network Components
      QtTestNetworkInterfaceStatus m_NetworkInterfaceStatus;
      QThread m_NetworkInterfaceStatusThread;

  };
}

#endif /* __QT_TESTCOMPONENTMANAGER_H__ */
