#ifndef __QT_TEST_FRAME_H__
#define __QT_TEST_FRAME_H__

#include <QtCore/qvector.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>

#include <QtGui/QFrame>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>


namespace QT_TEST
{

  class UtilArrow : public QVector<QPoint>
  {
    public:
      UtilArrow(int _x_off, int _y_off, int _w);

      void GenPolygon();

      const QPolygon &GetPolygon() {GenPolygon(); return Poly;};

    private:
      QPolygon Poly;
      int x_off, y_off;
      int W;
  };


  class RobotImg
  {
    public:
      RobotImg(double _x_init, double _y_init, double _theta_init);

      void Draw(QPainter &painter);

      void SetX(double _x) {pos_x=_x;};
      void SetY(double _y) {pos_y=_y;};
      void SetTheta(double _theta) {pos_theta=_theta;};

      double GetX() {return pos_x;};
      double GetY() {return pos_y;};
      double GetTheta() {return pos_theta;};
    private:
      static void RotateQPointF(QPointF &src_p, double angle, QPointF &dst_p);
      static void InvertYAndScaleQPointF(QPointF &p, double scale);

      QVector<QPointF> perimeter;
      QPointF arrow_start;
      QPointF arrow_end;
      double pos_x;
      double pos_y;
      double pos_theta;
  };


  class QtTestWidget : public QWidget
  {
    Q_OBJECT

    // constructors
    public:
      QtTestWidget();
      virtual ~QtTestWidget() {}

      void SetColor(const QColor &_color) {my_color = _color;};

    protected:
      virtual void paintEvent(QPaintEvent *);

    // public Qt slots
    //public slots:

    private:
      QColor my_color;
  };


  class QtRobotPlaygroundWidget : public QWidget
  {
    Q_OBJECT

    // constructors
    public:
      QtRobotPlaygroundWidget();
      virtual ~QtRobotPlaygroundWidget() {}

      void SetColor(const QColor &_color) {my_color = _color;};

    protected:
      virtual void paintEvent(QPaintEvent *);

    // public Qt slots
    public slots:
      void StartAnimation();
      void AnimationTimerTick();

    protected:
      virtual void mousePressEvent(QMouseEvent * event);

    private:
      QColor my_color;
      RobotImg *my_robot;
      double m_anim_time;
      bool m_anim_running;
      QVector<QPoint> edit_traj_p;
  };


  class QtTestAlarmIcon : public QLabel
  {
    Q_OBJECT

    // constructors
    public:
      QtTestAlarmIcon();
      virtual ~QtTestAlarmIcon() {}

    // public Qt slots
    public slots:
      void SetAlarmStatus(int status);
  };


  class QtTestFrame : public QFrame
  {
    Q_OBJECT

    // constructors
    public:
      QtTestFrame();
      virtual ~QtTestFrame() {}

    // public methods
    public:

    protected:
      void Init();
      virtual void paintEvent(QPaintEvent *);

    // protected Qt slots to manage UI
    //protected slots:
    //  void UpdateUTCDateTime();

    // public Qt slots
    public slots:
      void SetNetworkInterfaceStatusAlarm(int status) { m_NetworkInterfaceStatusAlarmIcon->SetAlarmStatus(status); }
      void RequestShutdown();

    // Qt signals
    signals:
      void ShutdownSig(void);

    // data members
    private:
      QtTestAlarmIcon *m_NetworkInterfaceStatusAlarmIcon;

      QLabel m_QtTestTopLabel;

      QPushButton m_ShutdownButton;
      QPushButton m_StartAnimationButton;

      QTimer m_AnimTimer;
  };
}

#endif /* __QT_TEST_FRAME_H__ */
