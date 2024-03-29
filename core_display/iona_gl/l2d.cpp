﻿#include "iona_gl/l2d.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QScreen>
#include <cmath>

#include "app/app_msg_handler.h"
#include "iona_gl/l2d_config.h"
#include "iona_gl/l2d_utils.h"

using namespace IonaDesktop::CoreDisplay;
using namespace IonaDesktop::CoreDisplay;

L2dAllocator GLObj_L2d::_cubismAllocator;
Csm::CubismFramework::Option GLObj_L2d::_cubismOption;

GLObj_L2d::GLObj_L2d(QOpenGLWidget* parent, const QMatrix4x4& tf_camera_)
    : CoreDisplay::GLObjectBase(parent),
      _flag_path_ready(false),
      attr_sp_general_Transform(-1),
      attr_sp_general_Texture_0(-1),
      base_position(0, -5.0, -75.0),
      tf_camera(tf_camera_),
      _canvas_width(parent->width()),
      _canvas_height(parent->height()),
      _touchManager(new L2dTouchManager()),
      _deviceToScreen(new Csm::CubismMatrix44()),
      _viewMatrix(new Csm::CubismViewMatrix()),
      flag_mouse_pressed(false) {
  _clearColor[0] = 0.0f;
  _clearColor[1] = 0.0f;
  _clearColor[2] = 0.0f;
  _clearColor[3] = 0.0f;

  int virtual_width = 2000;
  int virtual_height = 1080;
  QList<QScreen*> L_screen = QApplication::screens();
  if (L_screen.size() == 1) {
    virtual_width = L_screen[0]->geometry().width();
    virtual_height = L_screen[0]->geometry().height();
  }
  virtscr_geometry = QRect(0, 0, virtual_width, virtual_height);

  // 縦サイズを基準とする
  float ratio =
      static_cast<float>(virtual_width) / static_cast<float>(virtual_height);
  float left = -ratio;
  float right = ratio;
  float bottom = L2dConfig::ViewLogicalLeft;
  float top = L2dConfig::ViewLogicalRight;

  _viewMatrix->SetScreenRect(left, right, bottom,
                             top);  // デバイスに対応する画面の範囲。 Xの左端,
                                    // Xの右端, Yの下端, Yの上端
  _viewMatrix->Scale(L2dConfig::ViewScale, L2dConfig::ViewScale);

  _deviceToScreen->LoadIdentity();  // サイズが変わった際などリセット必須
  if (virtual_width > virtual_height) {
    float screenW = fabsf(right - left);
    _deviceToScreen->ScaleRelative(screenW / virtual_width,
                                   -screenW / virtual_width);
  } else {
    float screenH = fabsf(top - bottom);
    _deviceToScreen->ScaleRelative(screenH / virtual_height,
                                   -screenH / virtual_height);
  }
  _deviceToScreen->TranslateRelative(-virtual_width * 0.5f,
                                     -virtual_height * 0.5f);

  // 表示範囲の設定
  _viewMatrix->SetMaxScale(L2dConfig::ViewMaxScale);  // 限界拡大率
  _viewMatrix->SetMinScale(L2dConfig::ViewMinScale);  // 限界縮小率

  // 表示できる最大範囲
  _viewMatrix->SetMaxScreenRect(
      L2dConfig::ViewLogicalMaxLeft, L2dConfig::ViewLogicalMaxRight,
      L2dConfig::ViewLogicalMaxBottom, L2dConfig::ViewLogicalMaxTop);

  double canvas_ratio =
      static_cast<double>(_canvas_width) / static_cast<double>(_canvas_height);
  GLdouble plane_height = 64.0;
  GLdouble plane_width = plane_height * canvas_ratio;

  // -------position------     ---tex---
  //    - _width / 2, + _width / 2, 0.0,    0.0, 1.0,
  //    - _width / 2, - _width / 2, 0.0,    0.0, 0.0,
  //   + _width / 2, - _width / 2, 0.0,   1.0, 0.0,
  //   + _width / 2, + _width / 2, 0.0,   1.0, 1.0
  memset(vertices, 0, sizeof(vertices));
  vertices[0] = -static_cast<GLfloat>(plane_width / 2);
  vertices[1] = +static_cast<GLfloat>(plane_height / 2);
  vertices[4] = 1.0f;
  vertices[5] = -static_cast<GLfloat>(plane_width / 2);
  vertices[6] = -static_cast<GLfloat>(plane_height / 2);
  vertices[10] = +static_cast<GLfloat>(plane_width / 2);
  vertices[11] = -static_cast<GLfloat>(plane_height / 2);
  vertices[13] = 1.0f;
  vertices[15] = +static_cast<GLfloat>(plane_width / 2);
  vertices[16] = +static_cast<GLfloat>(plane_height / 2);
  vertices[18] = 1.0f;
  vertices[19] = 1.0f;

  AppMsgHandler::getInstance().bindSlot("/animate/l2d_lbutton_down", this,
                                        SLOT(onL2dLButtonDown(const QPointF&)));
  AppMsgHandler::getInstance().bindSlot("/animate/l2d_mouse_move", this,
                                        SLOT(onL2dMouseMove(const QPointF&)));
  AppMsgHandler::getInstance().bindSlot("/animate/l2d_lbutton_up", this,
                                        SLOT(onL2dLButtonUp(const QPointF&)));
}

GLObj_L2d::~GLObj_L2d() {
  _renderBuffer.DestroyOffscreenFrame();
  delete _model;
  delete _viewMatrix;
  delete _deviceToScreen;
  delete _touchManager;
  Csm::CubismFramework::Dispose();
}

void GLObj_L2d::init() {
  initializeCubism();
  if (!_flag_path_ready) return;
  _model = new L2dModel();
  _model->LoadAssets(_modelHomeDir.GetRawString(),
                     _modelFileName.GetRawString());
  initializeShaders();
  initializeRenderPlane();
}

void GLObj_L2d::initializeCubism() {
  static bool _flag_cubismReady = false;
  if (!_flag_cubismReady) {
    _cubismOption.LogFunction = L2dPal::PrintMessage;
    _cubismOption.LoggingLevel = L2dConfig::CubismLoggingLevel;
    Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);
    Csm::CubismFramework::Initialize();
    _flag_cubismReady = true;
  } else
    L2dPal::PrintMessage(
        "Cubism framework has already been initialized. Abort.");
}

void GLObj_L2d::setModelPath(const Csm::csmChar* path,
                             const Csm::csmChar* fileName) {
  _modelHomeDir = path;
  _modelFileName = fileName;
  _flag_path_ready = true;
}

void GLObj_L2d::initializeShaders() {
  static constexpr const char* vs_general_src =
      "attribute vec4 Pos4f; \
            attribute vec2 TexCoord2f_0; \
            uniform mat4 Transform; \
            varying vec2 Tex2f_0; \
            void main(void) \
            { \
                gl_Position = Transform * Pos4f; \
                Tex2f_0 = TexCoord2f_0; \
            }";
  static constexpr const char* fs_general_src =
      "uniform sampler2D Texture_0; \
            varying vec2 Tex2f_0; \
            void main(void) \
            { \
                gl_FragColor = texture2D(Texture_0, Tex2f_0.st); \
            }";
  sprogram_general = new QOpenGLShaderProgram(this);
  sprogram_general->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                            vs_general_src);
  sprogram_general->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                            fs_general_src);
  if (!sprogram_general->link()) {
    qDebug() << "General Shader Program Link Failed";
  }
  attr_sp_general_Transform = sprogram_general->uniformLocation("Transform");
  attr_sp_general_Texture_0 = sprogram_general->uniformLocation("Texture_0");
}

void GLObj_L2d::initializeRenderPlane() {
  vao_plane = new QOpenGLVertexArrayObject(this);
  QOpenGLVertexArrayObject::Binder bind_vao(vao_plane);
  QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
  vbo.create();
  vbo.bind();
  vbo.allocate(vertices, sizeof(vertices));
  vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
  auto attr = -1;
  attr = sprogram_general->attributeLocation("Pos4f");
  sprogram_general->setAttributeBuffer(attr, GL_FLOAT, 0, 3,
                                       5 * sizeof(GLfloat));
  sprogram_general->enableAttributeArray(attr);
  attr = sprogram_general->attributeLocation("TexCoord2f_0");
  sprogram_general->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(GLfloat), 2,
                                       5 * sizeof(GLfloat));
  sprogram_general->enableAttributeArray(attr);
  // Clean
  vbo.release();
  vao_plane->release();
  vbo.destroy();
}

void GLObj_L2d::paint() {
  if (!_flag_path_ready) return;
  PreModelDraw();
  ModelDraw();
  PostModelDraw();
}

void GLObj_L2d::PreModelDraw() {
  if (!_renderBuffer.IsValid()) {  // 描画ターゲット内部未作成の場合はここで作成
    if (_canvas_width != 0 && _canvas_height != 0)
      _renderBuffer.CreateOffscreenFrame(
          static_cast<Csm::csmUint32>(_canvas_width),
          static_cast<Csm::csmUint32>(_canvas_height));
  }
  // レンダリング開始
  _renderBuffer.BeginDraw();
  _renderBuffer.Clear(_clearColor[0], _clearColor[1], _clearColor[2],
                      _clearColor[3]);  // 背景クリアカラー
}

void GLObj_L2d::ModelDraw() {
  _model->Update();
  Csm::CubismMatrix44 projection;
  projection.Scale(1.75f, 1.75f * static_cast<float>(_canvas_width) /
                              static_cast<float>(_canvas_height));
  _model->Draw(projection);
}

void GLObj_L2d::PostModelDraw() {
  // レンダリング終了
  _renderBuffer.EndDraw();

  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  sprogram_general->bind();
  vao_plane->bind();
  GLHandle::get()->glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _renderBuffer.GetColorBuffer());
  sprogram_general->setUniformValue(attr_sp_general_Texture_0, 0);
  QMatrix4x4 tf_portrait;
  tf_portrait.setToIdentity();
  tf_portrait.translate(base_position);
  sprogram_general->setUniformValue(attr_sp_general_Transform,
                                    tf_camera * tf_portrait);
  glDrawArrays(GL_QUADS, 0, 4);
  // Clean
  glDisable(GL_TEXTURE_2D);
  vao_plane->release();
  sprogram_general->release();
  glDisable(GL_ALPHA_TEST);
}

void GLObj_L2d::onL2dLButtonDown(const QPointF& pt) {
  auto gl = (QWidget*)(this->parent());
  auto gl_global_center = gl->mapToGlobal(gl->geometry().center());
  QPointF virt_pos = scrPosSigmoid(
      pt.x() - gl_global_center.x() + virtscr_geometry.center().x(),
      pt.y() - gl_global_center.y() + virtscr_geometry.center().y());
  _touchManager->TouchesBegan(virt_pos.x(), virt_pos.y());
  flag_mouse_pressed = true;
}

void GLObj_L2d::onL2dMouseMove(const QPointF& pt) {
  if (!flag_mouse_pressed) return;
  float viewX = this->TransformViewX(_touchManager->GetX());
  float viewY = this->TransformViewY(_touchManager->GetY());
  auto gl = (QWidget*)(this->parent());
  auto gl_global_center = gl->mapToGlobal(gl->geometry().center());
  QPointF virt_pos = scrPosSigmoid(
      pt.x() - gl_global_center.x() + virtscr_geometry.center().x(),
      pt.y() - gl_global_center.y() + virtscr_geometry.center().y());
  _touchManager->TouchesMoved(virt_pos.x(), virt_pos.y());
  _model->SetDragging(viewX, viewY);
}

void GLObj_L2d::onL2dLButtonUp(const QPointF& pt) {
  Q_UNUSED(pt);
  // タッチ終了
  _model->SetDragging(0.0f, 0.0f);
  // シングルタップ
  float x = _deviceToScreen->TransformX(
      _touchManager->GetX());  // 論理座標変換した座標を取得。
  float y = _deviceToScreen->TransformY(
      _touchManager->GetY());  // 論理座標変換した座標を取得。
  if (L2dConfig::DebugTouchLogEnable)
    L2dPal::PrintLog("[APP]touchesEnded x:%.2f y:%.2f", x, y);
  if (L2dConfig::DebugLogEnable)
    L2dPal::PrintLog("[APP]tap point: {x:%.2f y:%.2f}", x, y);

  if (_model->HitTest(L2dConfig::HitAreaNameHead, x, y)) {
    if (L2dConfig::DebugLogEnable)
      L2dPal::PrintLog("[APP]hit area: [%s]", L2dConfig::HitAreaNameHead);
    _model->SetRandomExpression();
  } else if (_model->HitTest(L2dConfig::HitAreaNameBody, x, y)) {
    if (L2dConfig::DebugLogEnable)
      L2dPal::PrintLog("[APP]hit area: [%s]", L2dConfig::HitAreaNameBody);
    _model->StartRandomMotion(L2dConfig::MotionGroupTapBody,
                              L2dConfig::PriorityNormal);
  }
  flag_mouse_pressed = false;
}

QPointF GLObj_L2d::scrPosSigmoid(const double virt_x,
                                 const double virt_y) const {
  static const double x_coeff = log(19) / virtscr_geometry.width();
  static const double y_coeff = log(19) / virtscr_geometry.height();
  return QPointF(
      virtscr_geometry.width() /
          (1.0 + exp(-x_coeff * (virt_x - virtscr_geometry.center().x()))),
      virtscr_geometry.height() /
          (1.0 + exp(-y_coeff * (virt_y - virtscr_geometry.center().y()))));
}

float GLObj_L2d::TransformViewX(float deviceX) const {
  float screenX =
      _deviceToScreen->TransformX(deviceX);  // 論理座標変換した座標を取得。
  return _viewMatrix->InvertTransformX(screenX);  // 拡大、縮小、移動後の値。
}

float GLObj_L2d::TransformViewY(float deviceY) const {
  float screenY =
      _deviceToScreen->TransformY(deviceY);  // 論理座標変換した座標を取得。
  return _viewMatrix->InvertTransformY(screenY);  // 拡大、縮小、移動後の値。
}

float GLObj_L2d::TransformScreenX(float deviceX) const {
  return _deviceToScreen->TransformX(deviceX);
}

float GLObj_L2d::TransformScreenY(float deviceY) const {
  return _deviceToScreen->TransformY(deviceY);
}
