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

#include <math.h>


namespace QT_TEST
{

  typedef enum _TrajElementType {TrajElementTypeNull = 0, TrajElementTypeStraight, TrajElementTypeRot} TrajElementType;

  class TrajElementStraight;
  class TrajElementRot;

  class TrajElement
  {
    public:
      TrajElement()
      {
        my_type = TrajElementTypeNull;
        my_dist = 0.0;
        my_T0 = 0.0;
        my_Tf = 0.0;
        my_delta_T = 0.0;
        my_sampling_freq = 1.0;
        my_inc_D = 0.0;
        my_inc_X = 0.0;
        my_inc_Y = 0.0;
        my_inc_Theta = 0.0;
        my_X = 0.0;
        my_Y = 0.0;
        my_Theta = 0.0;
        my_Theta0 = 0.0;
        my_Thetaf = 0.0;
        my_delta_Theta = 0.0;
      }

      TrajElement(TrajElementType _type, QPointF _P0, QPointF _Pf, double _Theta0, double _Thetaf, double _T0, double _Tf, double _sampling_f)
      {
        my_type = _type;

        switch (my_type) {
        case TrajElementTypeNull:
          my_dist = 0.0;
          my_T0 = _T0;
          my_Tf = _Tf;
          my_delta_T = my_Tf - my_T0;
          my_sampling_freq = _sampling_f;
          my_inc_D = 0.0;
          my_inc_X = 0.0;
          my_inc_Y = 0.0;
          my_inc_Theta = 0.0;
          my_X = 0.0;
          my_Y = 0.0;
          my_Theta = 0.0;
          my_Theta0 = 0.0;
          my_Thetaf = 0.0;
          my_delta_Theta = 0.0;
          break;
        case TrajElementTypeStraight:
          double _X0, _Y0, _Xf, _Yf;
          double delta_X, delta_Y;
          my_P0 = _P0;
          my_Pf = _Pf;
          _X0 = _P0.x();
          _Y0 = _P0.y();
          _Xf = _Pf.x();
          _Yf = _Pf.y();
          my_X = _X0;
          my_Y = _Y0;
          delta_X = _Xf - _X0;
          delta_Y = _Yf - _Y0;
          my_dist = sqrt(delta_X*delta_X + delta_Y*delta_Y);
          if (abs(delta_X)>0.000000001) {
            my_Theta0 = atan (delta_Y/delta_X);
            if (delta_X<0) {
              if (my_Theta0>0) my_Theta0 -= M_PI;
              else my_Theta0 += M_PI;
            }
          } else {
            if (delta_Y<0) my_Theta0 = -M_PI_2;
            else my_Theta0 = M_PI_2;
          }
          my_Theta = my_Theta0;
          my_Thetaf = my_Theta0;
          my_T0 = _T0;
          my_Tf = _Tf;
          my_delta_T = my_Tf - my_T0;
          my_sampling_freq = _sampling_f;
          my_inc_D = (my_dist/(my_delta_T*my_sampling_freq));
          my_inc_X = my_inc_D*cos(my_Theta);
          my_inc_Y = my_inc_D*sin(my_Theta);
          my_inc_Theta = 0.0;
          my_delta_Theta = 0.0;
          break;
        case TrajElementTypeRot:
          my_P0 = _P0;
          my_Pf = _P0;
          my_dist = 0.0;
          my_T0 = _T0;
          my_Tf = _Tf;
          my_delta_T = my_Tf - my_T0;
          my_sampling_freq = _sampling_f;
          my_inc_D = 0.0;
          my_inc_X = 0.0;
          my_inc_Y = 0.0;
          my_delta_Theta = _Thetaf - _Theta0;
          my_inc_Theta = (my_delta_Theta/(my_delta_T*my_sampling_freq));
          my_X = my_P0.x();
          my_Y = my_P0.y();
          my_Theta0 = _Theta0;
          my_Theta = my_Theta0;
          my_Thetaf = _Thetaf;
          break;
        }
      };

      TrajElementType GetType() {return my_type;};

      double GetT0() {return my_T0;};
      double GetTf() {return my_Tf;};

      QPointF GetP0() {return my_P0;};
      QPointF GetPf() {return my_Pf;};

      double GetTheta0() {return my_Theta0;};
      double GetThetaf() {return my_Thetaf;};

      double GetX() {return my_X;};
      double GetY() {return my_Y;};
      double GetTheta() {return my_Theta;};

      void SetX(double _x) {my_X = _x;};
      void SetY(double _y) {my_Y = _y;};
      void SetTheta(double _theta) {my_Theta = _theta;};

      void Reset() {my_X = my_P0.x(); my_Y = my_P0.y(); my_Theta = my_Theta0;};

      void Increment () 
      {
        switch (my_type) {
        case TrajElementTypeNull:
          break;
        case TrajElementTypeStraight:
          my_X += my_inc_X;
          my_Y += my_inc_Y;
          break;
        case TrajElementTypeRot:
          my_Theta += my_inc_Theta;
          break;
        }
      };

      TrajElement& operator++ () {Increment(); return *this;};
      TrajElement operator++ (int) {TrajElement result = *this; Increment(); return result;};

    protected:
      TrajElementType my_type;

      QPointF my_P0;
      QPointF my_Pf;
      double my_Theta0;
      double my_Thetaf;
      double my_dist;
      double my_T0;
      double my_Tf;
      double my_delta_T;
      double my_sampling_freq;
      double my_inc_D;
      double my_inc_X;
      double my_inc_Y;
      double my_inc_Theta;
      double my_delta_Theta;

      double my_X;
      double my_Y;
      double my_Theta;
  };


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


  class QtRobotPlaygroundWidget : public QWidget
  {
    Q_OBJECT

    // constructors
    public:
      QtRobotPlaygroundWidget();
      virtual ~QtRobotPlaygroundWidget() {}

      void SetColor(const QColor &_color) {my_color = _color;};

      void CoordVirtToGUI(QPointF &v_P, QPoint &g_P);
      void CoordGUIToVirt(QPoint &g_P, QPointF &v_P);

    // public Qt slots
    public slots:
      void InitAnimation();
      void StartAnimation();
      void StopAnimation();
      void AnimationTimerTick();

      void DumpTrajectory();

    protected:
      virtual void paintEvent(QPaintEvent *);
      virtual void mousePressEvent(QMouseEvent * event);

    private:
      QColor my_color;
      RobotImg *my_robot;
      double m_anim_time;
      bool m_anim_running;
      int m_anim_index;
      QVector<QPoint> edit_traj_p;
      QVector<TrajElement> edit_traj_te;
      double edit_traj_curr_T;
      double edit_traj_curr_Theta;
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

      QTimer m_AnimTimer;
  };
}

#endif /* __QT_TEST_FRAME_H__ */
