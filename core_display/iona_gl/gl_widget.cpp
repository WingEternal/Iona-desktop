#include "iona_gl/gl_widget.h"
#include "app/app_param.h"
#include "app/app_msg_handler.h"
#include "gl_handle.h"
#include <QMouseEvent>

using namespace IonaDesktop::CoreDisplay;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setGeometry(0, 0, parent->width(), parent->height());
    AppMsgHandler::getInstance().bindSlot("/animate/update", this, SLOT(glAnimateUpdate()));
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

    asset_iona = new GLObj_L2d(this, tf_camera);
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

void GLWidget::glAnimateUpdate()
{ update(); }
