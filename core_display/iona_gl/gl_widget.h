#ifndef IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H
#define IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H

#include <QTimer>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QThread>

#include <QWidget>
#include "gl_entry.h"
#include <QOpenGLWidget>
#include <QPixmap>
#include "data_ring.h"
#include "l2d.h"

namespace IonaDesktop {
namespace CoreDisplay {
    class GLWidget : public QOpenGLWidget
    {
        Q_OBJECT
        /* Widget Initialization */
    public:
        explicit GLWidget(QWidget *parent = nullptr);
        ~GLWidget() override;

    private:
        // Geometry of GLWidget, also determine the canvas size for drawing
        QRect widget_geo;
        QTimer* update_timer;

        // Camera
        QMatrix4x4  tf_camera;
        // Assets
        CoreDisplay::GLObj_L2d* asset_iona;
        CoreDisplay::GLObj_DataRing* asset_data_ring;

        // Hook
        bool flag_lm_pressed; // is left button pressed?

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        // void mousePressEvent(QMouseEvent *ev) override;
        // void mouseMoveEvent(QMouseEvent *ev) override;
        // void mouseReleaseEvent(QMouseEvent *ev) override;

    public slots:
        void glPrivateMouseEventDispatch(const QEvent::Type type, const Qt::MouseButton b, const QPoint pt);

    private slots:
        void glAnimateUpdate();
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H
