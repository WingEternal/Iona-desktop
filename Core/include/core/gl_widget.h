#ifndef IONADESKTOP_CORE_GL_WIDGET_H
#define IONADESKTOP_CORE_GL_WIDGET_H
/* GLOBAL SETTINGS */
#include "core/core_global.h"
#include <QTimer>
#include <QElapsedTimer>
/* GL INCLUDES */
#include <QWidget>
#include "core/gl_entry.h"
#include <QOpenGLWidget>
#include <QPixmap>
#include "core_ex1/data_ring.h"
#include "core_ex1/l2d.h"

namespace IonaDesktop {
namespace Core {
    class GLWidget : public QOpenGLWidget
    {
        Q_OBJECT
        /* Widget Initialization */
    public:
        explicit GLWidget(const QRect& geo, QWidget *parent = nullptr);
        ~GLWidget() override;

        inline double getLogTime()
        { return log_timer_ptr->elapsed(); }
    private:
        QRect widget_geo;
        QElapsedTimer *log_timer_ptr;

        /* GL Methods */
    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;
        // Camera
    private:
        QMatrix4x4  tf_camera;
        // Assets
        CoreEx1::GLObj_DataRing* asset_data_ring;
        CoreEx1::GLObj_L2d* asset_iona;

    private slots:
        void Slot_GLObj_RequestUpdate();
    };
}
}

#endif // IONADESKTOP_CORE_GL_WIDGET_H
