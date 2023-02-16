﻿#include "iona_gl/gl_widget.h"
#include "app/app_config.h"
#include "app/app_msg_handle.h"
#include <QMouseEvent>

using namespace IonaDesktop::CoreDisplay;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    AppConfig::getInstance().getParam("/window/size", widget_geo);
    this->setGeometry(0, 0, widget_geo.width(), widget_geo.height());
    update_timer = new QTimer(this);
    update_timer->setTimerType(Qt::PreciseTimer);

    double update_rate;
    AppConfig::getInstance().getParam("/animate/update_rate", update_rate);
    update_timer->start(qRound(1000.0 / update_rate));
    connect(update_timer, SIGNAL(timeout()), this, SLOT(glAnimateUpdate()));

    AppMsgHandler::getInstance().bindSlot("/animate/lookat", this, SLOT(glPrivateMouseEventDispatch(const QEvent::Type, const Qt::MouseButton, const QPoint)));
}
GLWidget::~GLWidget()
{
    makeCurrent();
    /* Do some clean... */
}

void GLWidget::initializeGL()
{
    this->makeCurrent();
    GLHandle::get()->initializeOpenGLFunctions();

    asset_iona = new GLObj_L2d(this, tf_camera,  widget_geo);
    asset_iona->setModelPath(":/charater/live2d/", "tbs.model3.json");
    asset_iona->init();

    asset_data_ring = new GLObj_DataRing(this, tf_camera);
    asset_data_ring->init();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
void GLWidget::resizeGL(int w, int h)
{
    GLfloat fov = 50.0f;
    GLfloat aspect = GLfloat(w) / GLfloat(h);
    GLfloat zMin = 0.1f, zMax = 120.0f;
    tf_camera.setToIdentity();
    tf_camera.perspective(fov, aspect, zMin, zMax);
}

void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);

    asset_iona->paint();
    asset_data_ring->paint();
}

//void GLWidget::mousePressEvent(QMouseEvent *ev)
//{
//    asset_iona->mousePressEvent(ev);
//    asset_data_ring->mousePressEvent(ev);
//    ev->accept();
//}

//void GLWidget::mouseMoveEvent(QMouseEvent *ev)
//{
//    asset_iona->mouseMoveEvent(ev);
//    asset_data_ring->mouseMoveEvent(ev);
//    ev->accept();
//}

//void GLWidget::mouseReleaseEvent(QMouseEvent *ev)
//{
//    asset_iona->mouseReleaseEvent(ev);
//    asset_data_ring->mouseReleaseEvent(ev);
//    ev->accept();
//}

void GLWidget::glPrivateMouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b, const QPoint pt)
{
    QMouseEvent* ev = new QMouseEvent(type, pt, pt, b, b, Qt::NoModifier);
    switch(type)
    {
    case QEvent::MouseButtonPress:
        asset_iona->mousePressEvent(ev);
        asset_data_ring->mousePressEvent(ev);
        break;
    case QEvent::MouseMove:
        asset_iona->mouseMoveEvent(ev);
        asset_data_ring->mouseMoveEvent(ev);
        break;
    case QEvent::MouseButtonRelease:
        asset_iona->mouseReleaseEvent(ev);
        asset_iona->mouseReleaseEvent(ev);
        break;
    default: break;
    }
    delete ev;
}

void GLWidget::glAnimateUpdate()
{ update(); }
