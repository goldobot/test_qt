#include "QtTestFrame.h"

#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPainter>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QLocale>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

#include <math.h>

/* Qt Resources path Prefix */
#if defined(_WIN32) || defined(_WIN64) 
#define QRP ":"
#else
#define QRP ""
#endif

#define PLAYGROUND_SZ_X_MM  3000
#define PLAYGROUND_SZ_Y_MM  2000

#define PLAYGROUND_SCALE  4.0

#define ANIMATION_SAMPLING_FREQ 10

#define QT_TEST_WIDGET_SZ_X (PLAYGROUND_SZ_X_MM/PLAYGROUND_SCALE)
#define QT_TEST_WIDGET_SZ_Y (PLAYGROUND_SZ_Y_MM/PLAYGROUND_SCALE)


namespace QT_TEST
{

  RobotImg::RobotImg(double _x_init, double _y_init, double _theta_init)
  {
    pos_x = _x_init;
    pos_y = _y_init;
    pos_theta = _theta_init;
    arrow_start = QPointF(-40.0, 0.0);
    arrow_end = QPointF(40.0, 0.0);
    perimeter << 
      QPointF(-120.0,   0.0) << 
      QPointF(-120.0, 100.0) << 
      QPointF( -70.0, 140.0) << 
      QPointF(  70.0, 140.0) << 
      QPointF( 120.0,  80.0) << 
      QPointF( 120.0,   0.0) << 
      QPointF( 120.0, -80.0) << 
      QPointF(  70.0,-140.0) << 
      QPointF( -70.0,-140.0) << 
      QPointF(-120.0,-100.0);
  }


  void
  RobotImg::RotateQPointF(QPointF &src_p, double angle, QPointF &dst_p)
  {
    double m00=::cos(angle);
    double m01=::sin(-angle);
    double m10=::sin(angle);
    double m11=::cos(angle);

    dst_p.setX(src_p.x()*m00 + src_p.y()*m01);
    dst_p.setY(src_p.x()*m10 + src_p.y()*m11);
  }


  void
  RobotImg::InvertYAndScaleQPointF(QPointF &p, double scale)
  {
    p.setX(p.x()/scale);
    p.setY(-p.y()/scale);
  }


  void
  RobotImg::Draw(QPainter &painter)
  {
    int draw_off_x = PLAYGROUND_SZ_X_MM / 2 / PLAYGROUND_SCALE;
    int draw_off_y = PLAYGROUND_SZ_Y_MM / 2 / PLAYGROUND_SCALE;

    QPolygon robot_poly;

    for (QVector<QPointF>::const_iterator iP_n = perimeter.constBegin(); iP_n != perimeter.constEnd(); iP_n++) {
      QPointF orig_P = (*iP_n);
      QPointF real_P;

      RotateQPointF(orig_P, pos_theta, real_P);

      real_P += QPointF(pos_x,pos_y);

      InvertYAndScaleQPointF(real_P, PLAYGROUND_SCALE);

      robot_poly << (real_P.toPoint() + QPoint(draw_off_x,draw_off_y));
    }

    QPen my_pen1 (Qt::black, 4, Qt::SolidLine);
    painter.setPen(my_pen1);
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

    painter.drawPolygon(robot_poly);

    QPointF real_arrow_start;
    QPointF real_arrow_end;

    RotateQPointF(arrow_start, pos_theta, real_arrow_start);
    RotateQPointF(arrow_end  , pos_theta, real_arrow_end);

    real_arrow_start += QPointF(pos_x,pos_y);
    real_arrow_end += QPointF(pos_x,pos_y);

    InvertYAndScaleQPointF(real_arrow_start, PLAYGROUND_SCALE);
    InvertYAndScaleQPointF(real_arrow_end, PLAYGROUND_SCALE);

    UtilArrow arrow(draw_off_x,draw_off_y,5);
    arrow << 
      real_arrow_start.toPoint() <<
      real_arrow_end.toPoint();

    QPolygon arrow_poly = arrow.GetPolygon();

    QPen my_pen2 (Qt::black, 2, Qt::SolidLine);
    painter.setPen(my_pen2);
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));

    painter.drawPolygon(arrow_poly);
  }


  QtRobotPlaygroundWidget::QtRobotPlaygroundWidget()
    :QWidget()
  {
    m_anim_time = 0.0;

    m_anim_running = false;

    my_color = Qt::white;
    setFixedSize(QT_TEST_WIDGET_SZ_X,QT_TEST_WIDGET_SZ_Y);

    my_robot = new RobotImg(-400.0, -800.0, 0.5*M_PI);

    edit_traj_p << QPoint(375-100,250+200);
  }


  void 
  QtRobotPlaygroundWidget::StartAnimation() 
  {
    m_anim_time = 0.0;

    my_robot->SetX(-400.0);
    my_robot->SetY(-800.0);
    my_robot->SetTheta(0.5*M_PI);

    m_anim_running = true;
  }


  void 
  QtRobotPlaygroundWidget::AnimationTimerTick() 
  {
    if (!m_anim_running) return;

    if (m_anim_time < 4.0) {
      double my_dist = 1000.0;
      double my_time = 4.0;
      double inc_D = (my_dist/(my_time*ANIMATION_SAMPLING_FREQ));
      double inc_X = inc_D*cos(my_robot->GetTheta());
      double inc_Y = inc_D*sin(my_robot->GetTheta());
      my_robot->SetX(my_robot->GetX() + inc_X);
      my_robot->SetY(my_robot->GetY() + inc_Y);
    } else if (m_anim_time < 7.9) {
      double my_angle = (-M_PI/2);
      double my_time = 4.0;
      double inc_Theta = (my_angle/(my_time*ANIMATION_SAMPLING_FREQ));
      my_robot->SetTheta(my_robot->GetTheta() + inc_Theta);
    } else if (m_anim_time < 12.0) {
      double my_dist = 1500.0;
      double my_time = 4.0;
      double inc_D = (my_dist/(my_time*ANIMATION_SAMPLING_FREQ));
      double inc_X = inc_D*cos(my_robot->GetTheta());
      double inc_Y = inc_D*sin(my_robot->GetTheta());
      my_robot->SetX(my_robot->GetX() + inc_X);
      my_robot->SetY(my_robot->GetY() + inc_Y);
    } else if (m_anim_time < 15.9) {
      double my_angle = (-2.0*M_PI/3);
      double my_time = 4.0;
      double inc_Theta = (my_angle/(my_time*ANIMATION_SAMPLING_FREQ));
      my_robot->SetTheta(my_robot->GetTheta() + inc_Theta);
    } else if (m_anim_time < 20.0) {
      double my_dist = 500.0;
      double my_time = 4.0;
      double inc_D = (my_dist/(my_time*ANIMATION_SAMPLING_FREQ));
      double inc_X = inc_D*cos(my_robot->GetTheta());
      double inc_Y = inc_D*sin(my_robot->GetTheta());
      my_robot->SetX(my_robot->GetX() + inc_X);
      my_robot->SetY(my_robot->GetY() + inc_Y);
    } else {
      m_anim_running = false;
    }

    repaint();

    m_anim_time += 1.0/ANIMATION_SAMPLING_FREQ;
  }


  void
  QtRobotPlaygroundWidget::mousePressEvent(QMouseEvent * event)
  {
    edit_traj_p << QPoint(event->x(),event->y());
    repaint();
  }


  void
  QtRobotPlaygroundWidget::paintEvent(QPaintEvent *event)
  {
    QWidget::paintEvent(event);

    QPainter painter(this);

#if 1 /* pour forcer la couleur du background du widget (sinon il utilise les parametres du parent) */
    QStyleOption sopt;
    sopt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &sopt, &painter, this);
#endif

#if 1 /* pour dessiner un cadre autour du widget */
    QPolygon cadre;
    cadre << QPoint(                  0,                   0) 
          << QPoint(QT_TEST_WIDGET_SZ_X,                   0) 
          << QPoint(QT_TEST_WIDGET_SZ_X, QT_TEST_WIDGET_SZ_Y) 
          << QPoint(                  0, QT_TEST_WIDGET_SZ_Y); 

    QPen my_pen0 (Qt::black, 2, Qt::SolidLine);
    painter.setPen(my_pen0);

    painter.drawPolygon(cadre);
#endif

#if 1 /* pour dessiner des axes */
    QPen my_pen1 (Qt::black, 1, Qt::DotLine);
    painter.setPen(my_pen1);

    // axe X
    painter.drawLine(0, QT_TEST_WIDGET_SZ_Y/2, QT_TEST_WIDGET_SZ_X, QT_TEST_WIDGET_SZ_Y/2);

    // axe Y
    painter.drawLine(QT_TEST_WIDGET_SZ_X/2, 0, QT_TEST_WIDGET_SZ_X/2, QT_TEST_WIDGET_SZ_Y);
#endif

    my_robot->Draw(painter);


    QPen my_pen2 (Qt::blue, 2, Qt::DotLine);
    painter.setPen(my_pen2);

    QPoint P_0 = edit_traj_p[0];
    QPoint P_n;
    QPoint P_n_1 = P_0;
    for (QVector<QPoint>::const_iterator iP_n = edit_traj_p.constBegin(); iP_n != edit_traj_p.constEnd(); iP_n++) {
      P_n = (*iP_n);
      if (P_0==P_n) {
        continue;
      }

      painter.drawLine(P_n_1, P_n);

      P_n_1 = P_n;
    }
  }


  QtTestAlarmIcon::QtTestAlarmIcon()
  {
    setPixmap(QPixmap(QRP"Resources/200px-Button_Icon_Black.svg.png").scaledToHeight(24, Qt::SmoothTransformation));
  }


  void
  QtTestAlarmIcon::SetAlarmStatus(int status)
  {
    if (status == 0)
    {
      setPixmap(QPixmap(QRP"Resources/200px-Button_Icon_Black.svg.png").scaledToHeight(24, Qt::SmoothTransformation));
    }
    else if (status == 1)
    {
      setPixmap(QPixmap(QRP"Resources/200px-Button_Icon_GreenForest.svg.png").scaledToHeight(24, Qt::SmoothTransformation));
    }
    else /* -1 */
    {
      setPixmap(QPixmap(QRP"Resources/200px-Button_Icon_Red.svg.png").scaledToHeight(24, Qt::SmoothTransformation));
    }
  }


  QtTestFrame::QtTestFrame()
  {
    Init();
  }


  void
  QtTestFrame::Init()
  {
    // Main Layout (Vertical Layout)
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);

    QHBoxLayout *pUpperLayout = new QHBoxLayout;
    pUpperLayout->setContentsMargins(0, 0, 0, 0);
    pUpperLayout->setSpacing(0);

    QHBoxLayout *pQtTestTopLayout = new QHBoxLayout;
    pQtTestTopLayout->setContentsMargins(10, 10, 10, 10);
    pQtTestTopLayout->setSpacing(10);

    m_QtTestTopLabel.setStyleSheet("color: white; background-color: green");
    m_QtTestTopLabel.setText("QtTest Ready");
    pQtTestTopLayout->addWidget(&m_QtTestTopLabel);
    pQtTestTopLayout->addStretch();

    m_ShutdownButton.setText("Shutdown");
    m_ShutdownButton.setDisabled(false);
    pQtTestTopLayout->addWidget(&m_ShutdownButton);

    m_StartAnimationButton.setText("Start animationn");
    m_StartAnimationButton.setDisabled(false);
    pQtTestTopLayout->addWidget(&m_StartAnimationButton);

    pUpperLayout->addLayout(pQtTestTopLayout);

    pUpperLayout->addStretch();

    pLayout->addLayout(pUpperLayout);


    // TEST ++
    QGroupBox *pRobotBox = new QGroupBox;
    //pRobotBox->setStyleSheet("background-color: white");
    pRobotBox->setTitle("Robotik");

    QGridLayout *pRobotBoxLayout = new QGridLayout;
    pRobotBoxLayout->setContentsMargins(10, 10, 10, 10);
    pRobotBoxLayout->setSpacing(10);
    pRobotBoxLayout->setAlignment(Qt::AlignCenter);

    QtRobotPlaygroundWidget *pplayground = new QtRobotPlaygroundWidget;
    pplayground->SetColor(Qt::red);
    pplayground->setStyleSheet("background-color: white");
    pRobotBoxLayout->addWidget(pplayground);

    pRobotBox->setLayout(pRobotBoxLayout);

    pLayout->addWidget(pRobotBox);
    // TEST --


    // Alarms Layout (Horizontal Layout)
    QHBoxLayout *pAlarmsLayout = new QHBoxLayout;
    pAlarmsLayout->setContentsMargins(5, 5, 5, 5);
    pAlarmsLayout->setSpacing(5);

    // Network connectivity box
    QGroupBox *pNetworkConnectivityBox = new QGroupBox;
    pNetworkConnectivityBox->setTitle("Network connectivity");

    QGridLayout *pNetworkConnectivityBoxLayout = new QGridLayout;
    pNetworkConnectivityBoxLayout->setContentsMargins(10, 10, 10, 10);
    pNetworkConnectivityBoxLayout->setSpacing(10);

    m_NetworkInterfaceStatusAlarmIcon = new QtTestAlarmIcon;
    pNetworkConnectivityBoxLayout->addWidget(m_NetworkInterfaceStatusAlarmIcon, 0, 0);

    pNetworkConnectivityBox->setLayout(pNetworkConnectivityBoxLayout);

    pAlarmsLayout->addWidget(pNetworkConnectivityBox);

    pLayout->addLayout(pAlarmsLayout);


    // set layout
    setLayout(pLayout);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(&m_ShutdownButton,SIGNAL(clicked()), this, SLOT(RequestShutdown()));

    connect(&m_StartAnimationButton,SIGNAL(clicked()), pplayground, SLOT(StartAnimation()));

    connect(&m_AnimTimer, SIGNAL(timeout()), pplayground, SLOT(AnimationTimerTick()));

#if 0
    setWindowFlags(Qt::SplashScreen);
#endif
    resize(1024, 768);

    m_AnimTimer.setInterval(1000/ANIMATION_SAMPLING_FREQ);

    m_AnimTimer.start();
  }


  void 
  QtTestFrame::RequestShutdown() 
  {
    int choice = QMessageBox::question(this, "QtTest shutdown", "HARD?\n", QMessageBox::No, QMessageBox::Yes);

    if (choice==QMessageBox::No) {
      emit ShutdownSig();
    } else {
      exit (0);
    }
  }


  void 
  QtTestFrame::paintEvent(QPaintEvent *event)
  {
    QFrame::paintEvent(event);

#if 0 /*  FIXME : TODO : belle fleche verte.. on garde ca comme example.. */
    QPainter painter(this);

    QPen my_pen1 (Qt::black, 4, Qt::SolidLine);
    painter.setPen(my_pen1);
    //painter.drawLine(1,1,200,200);

    UtilArrow my_arrow(400,200,5);

    my_arrow << QPoint(0,0) << QPoint(50,0) << QPoint(50,50)
             << QPoint(100,50) << QPoint(150,25) << QPoint(100,0);

    QPolygon fleche = my_arrow.GetPolygon();

    painter.setPen(my_pen1);
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));

    painter.drawPolygon(fleche);
#endif

  }


  UtilArrow::UtilArrow(int _x_off, int _y_off, int _w)
  {
    x_off = _x_off;
    y_off = _y_off;
    W = _w;
  }

  void 
  UtilArrow::GenPolygon()
  {
    QPoint off(x_off, y_off);
    QPoint P_0 = (*this)[0];
    QPoint P_1 = (*this)[1];
    QPoint P_n;
    QVector<QPoint> back_list;
    double cAx,cAy,cBx,cBy;
    double mod_A, mod_B;
    double dx, dy;
    double Vx, Vy;

    cAx = 0.0;
    cAy = 0.0;
    //icAx = 0;
    //icAy = 0;
    for (QVector<QPoint>::const_iterator iP_n = this->constBegin(); iP_n != this->constEnd(); iP_n++) {
      P_n = (*iP_n);
      if (P_0==P_n) {
        continue;
      }
      dx = P_n.x()-P_0.x();
      dy = P_n.y()-P_0.y();
      if (P_1==P_n) {
        mod_A = sqrt(dx*dx+dy*dy);
        cAx = dx/mod_A;
        cAy = dy/mod_A;
        //icAx = dx;
        //icAy = dy;
        Vx = W*cAy;
        Vy = -W*cAx;
      } else {
        mod_B = sqrt(dx*dx+dy*dy);
        cBx = dx/mod_B;
        cBy = dy/mod_B;
        if ((cAx+cBx)!=0) {
          Vx =  W*(cAx*cBy+cAy*cBx)/(cAx+cBx);
          Vy = -W*(1.0-cAy*cBy+cAx*cBx)/(cAx+cBx);
        } else if ((cBy+cAy)!=0) {
          Vx =  W*(1.0-cAx*cBx+cAy*cBy)/(cBy+cAy);
          Vy = -W*(cAx*cBy+cAy*cBx)/(cBy+cAy);
        } else {
          Vx = 0.0;
          Vy = 0.0;
        }
        cAx = cBx;
        cAy = cBy;
        mod_A = mod_B;
      }
      Poly<<(P_0+QPoint(Vx,Vy)+off);
      back_list.prepend(P_0+QPoint(-Vx,-Vy)+off);
      P_0 = P_n;
    }

    Vx = W*cAy;
    Vy = -W*cAx;
    Poly<<(P_0+QPoint(Vx,Vy)+off);
    back_list.prepend(P_0+QPoint(-Vx,-Vy)+off);

    Vx = 2.0*W*cAy;
    Vy = -2.0*W*cAx;
    Poly<<(P_0+QPoint(Vx,Vy)+off);
    back_list.prepend(P_0+QPoint(-Vx,-Vy)+off);

    Vx = 2.0*W*cAx;
    Vy = 2.0*W*cAy;
    Poly<<(P_0+QPoint(Vx,Vy)+off);

    for (QVector<QPoint>::const_iterator iP_n = back_list.constBegin(); iP_n != back_list.constEnd(); iP_n++) {
      P_n = (*iP_n);
      Poly<<P_n;
    }

  }


  QtTestWidget::QtTestWidget()
    :QWidget()
  {
    my_color = Qt::white;
    setFixedSize(QT_TEST_WIDGET_SZ_X,QT_TEST_WIDGET_SZ_Y);
  }


  void
  QtTestWidget::paintEvent(QPaintEvent *event)
  {
    QWidget::paintEvent(event);

    QPainter painter(this);

#if 1 /* pour forcer la couleur du background du widget (sinon il utilise les parametres du parent) */
    QStyleOption sopt;
    sopt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &sopt, &painter, this);
#endif

#if 1 /* pour dessiner un cadre autour du widget */
    QPolygon cadre;
    cadre << QPoint(                  0,                   0) 
          << QPoint(QT_TEST_WIDGET_SZ_X,                   0) 
          << QPoint(QT_TEST_WIDGET_SZ_X, QT_TEST_WIDGET_SZ_Y) 
          << QPoint(                  0, QT_TEST_WIDGET_SZ_Y); 

    QPen my_pen0 (Qt::black, 2, Qt::SolidLine);
    painter.setPen(my_pen0);
#endif

    painter.drawPolygon(cadre);

    int x_offset = QT_TEST_WIDGET_SZ_X/2;
    int y_offset = QT_TEST_WIDGET_SZ_Y/2;
    QPolygon etoile;
    etoile << QPoint(x_offset-30, y_offset +0) 
           << QPoint(x_offset -5, y_offset +5) 
           << QPoint(x_offset +0, y_offset+30) 
           << QPoint(x_offset +5, y_offset +5) 
           << QPoint(x_offset+30, y_offset +0) 
           << QPoint(x_offset +5, y_offset -5) 
           << QPoint(x_offset +0, y_offset-30)
           << QPoint(x_offset -5, y_offset -5);

    QPen my_pen1 (Qt::black, 4, Qt::SolidLine);
    painter.setPen(my_pen1);
    //painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.setBrush(QBrush(my_color, Qt::SolidPattern));

    painter.drawPolygon(etoile);
  }


}
