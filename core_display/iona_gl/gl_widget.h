﻿#ifndef IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H
#define IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H

#include <QTimer>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QThread>

#include <QWidget>
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
        explicit GLWidget(QWidget *parent);
        ~GLWidget() override;

    private:
        // Camera
        QMatrix4x4  tf_camera;
        // Assets
        GLObj_L2d* asset_iona;
        GLObj_DataRing* asset_data_ring;

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

    private slots:
        void glAnimateUpdate();
    };
}
}

#endif // IONADESKTOP_CORE_DISPLAY_GL_WIDGET_H
