#include "core/gl_widget.h"

using namespace IonaDesktop::Core;

GLWidget::GLWidget(const QRect& geo, QWidget *parent)
    : QOpenGLWidget(parent),
      widget_geo(geo)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setGeometry(widget_geo);
    log_timer_ptr = new QElapsedTimer();
    log_timer_ptr->start();
}
GLWidget::~GLWidget()
{
    makeCurrent();
    delete log_timer_ptr;
    /* Do some clean... */
}

void GLWidget::initializeGL()
{
    this->makeCurrent();
    GLEntry::get()->initializeOpenGLFunctions();

    asset_data_ring = new CoreEx1::GLObj_DataRing(this, tf_camera);
    asset_data_ring->init();
    connect(asset_data_ring, SIGNAL(RequestUpdate()), this, SLOT(Slot_GLObj_RequestUpdate()));

    asset_iona = new CoreEx1::GLObj_L2d(this, tf_camera,  widget_geo);
    asset_iona->setModelPath(":/charater/live2d/", "Iona_ver_0_5.model3.json");
    asset_iona->init();

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

void GLWidget::Slot_GLObj_RequestUpdate()
{ update(); }
