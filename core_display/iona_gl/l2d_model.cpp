#include "iona_gl/l2d_model.h"

#include <CubismDefaultParameterId.hpp>
#include <CubismModelSettingJson.hpp>
#include <Id/CubismIdManager.hpp>
#include <Motion/CubismMotion.hpp>
#include <Motion/CubismMotionQueueEntry.hpp>
#include <Physics/CubismPhysics.hpp>
#include <Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp>
#include <Utils/CubismString.hpp>
#include <fstream>
#include <vector>

#include "app/app_param.h"
#include "iona_gl/l2d_config.h"
#include "iona_gl/l2d_utils.h"

using namespace IonaDesktop::CoreDisplay;
using namespace Live2D::Cubism::Framework;
using namespace Live2D::Cubism::Framework::DefaultParameterId;

L2dModel::L2dModel()
    : QObject(nullptr),
      CubismUserModel(),
      _modelSetting(nullptr),
      _userTimeSeconds(0.0f) {
  if (L2dConfig::DebugLogEnable) _debugMode = true;

  _idParamAngleX = CubismFramework::GetIdManager()->GetId(ParamAngleX);
  _idParamAngleY = CubismFramework::GetIdManager()->GetId(ParamAngleY);
  _idParamAngleZ = CubismFramework::GetIdManager()->GetId(ParamAngleZ);
  _idParamBodyAngleX = CubismFramework::GetIdManager()->GetId(ParamBodyAngleX);
  _idParamEyeBallX = CubismFramework::GetIdManager()->GetId(ParamEyeBallX);
  _idParamEyeBallY = CubismFramework::GetIdManager()->GetId(ParamEyeBallY);

  _texManager = new L2dTexManager();
}

L2dModel::~L2dModel() {
  ReleaseMotions();
  ReleaseExpressions();
  for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++) {
    const csmChar* group = _modelSetting->GetMotionGroupName(i);
    ReleaseMotionGroup(group);
  }
  delete (_modelSetting);
  delete _texManager;
}

void L2dModel::LoadAssets(const Csm::csmChar* dir,
                          const Csm::csmChar* fileName) {
  _modelHomeDir = dir;
  if (_debugMode) L2dPal::PrintLog("[APP]load model setting: %s", fileName);
  csmSizeInt size;
  const csmString path = csmString(_modelHomeDir) + fileName;
  csmByte* buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
  ICubismModelSetting* setting = new CubismModelSettingJson(buffer, size);
  L2dPal::ReleaseBytes(buffer);

  SetupModel(setting);
  if (_model == NULL) {
    L2dPal::PrintLog("Failed to LoadAssets().");
    return;
  }
  CreateRenderer();
  SetupTextures();
}

void L2dModel::SetupModel(ICubismModelSetting* setting) {
  _updating = true;
  _initialized = false;

  _modelSetting = setting;

  csmByte* buffer;
  csmSizeInt size;

  // Cubism Model
  if (strcmp(_modelSetting->GetModelFileName(), "") != 0) {
    csmString path = _modelSetting->GetModelFileName();
    path = _modelHomeDir + path;

    if (_debugMode)
      L2dPal::PrintLog("[APP]create model: %s", setting->GetModelFileName());

    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    LoadModel(buffer, size);
    L2dPal::ReleaseBytes(buffer);
  }

  // Expression
  if (_modelSetting->GetExpressionCount() > 0) {
    const csmInt32 count = _modelSetting->GetExpressionCount();
    for (csmInt32 i = 0; i < count; i++) {
      csmString name = _modelSetting->GetExpressionName(i);
      csmString path = _modelSetting->GetExpressionFileName(i);
      path = _modelHomeDir + path;

      buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
      ACubismMotion* motion = LoadExpression(buffer, size, name.GetRawString());

      if (_expressions[name] != nullptr) {
        ACubismMotion::Delete(_expressions[name]);
        _expressions[name] = nullptr;
      }
      _expressions[name] = motion;

      L2dPal::ReleaseBytes(buffer);
    }
  }

  // Physics
  if (strcmp(_modelSetting->GetPhysicsFileName(), "") != 0) {
    csmString path = _modelSetting->GetPhysicsFileName();
    path = _modelHomeDir + path;
    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    LoadPhysics(buffer, size);
    L2dPal::ReleaseBytes(buffer);
  }

  // Pose
  if (strcmp(_modelSetting->GetPoseFileName(), "") != 0) {
    csmString path = _modelSetting->GetPoseFileName();
    path = _modelHomeDir + path;

    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    LoadPose(buffer, size);
    L2dPal::ReleaseBytes(buffer);
  }

  // EyeBlink
  if (_modelSetting->GetEyeBlinkParameterCount() > 0) {
    _eyeBlink = CubismEyeBlink::Create(_modelSetting);
  }

  // Breath
  {
    _breath = CubismBreath::Create();

    csmVector<CubismBreath::BreathParameterData> breathParameters;

    breathParameters.PushBack(CubismBreath::BreathParameterData(
        _idParamAngleX, 0.0f, 15.0f, 6.5345f, 0.5f));
    breathParameters.PushBack(CubismBreath::BreathParameterData(
        _idParamAngleY, 0.0f, 8.0f, 3.5345f, 0.5f));
    breathParameters.PushBack(CubismBreath::BreathParameterData(
        _idParamAngleZ, 0.0f, 10.0f, 5.5345f, 0.5f));
    breathParameters.PushBack(CubismBreath::BreathParameterData(
        _idParamBodyAngleX, 0.0f, 4.0f, 15.5345f, 0.5f));
    breathParameters.PushBack(CubismBreath::BreathParameterData(
        CubismFramework::GetIdManager()->GetId(ParamBreath), 0.5f, 0.5f,
        3.2345f, 0.5f));

    _breath->SetParameters(breathParameters);
  }

  // UserData
  if (strcmp(_modelSetting->GetUserDataFile(), "") != 0) {
    csmString path = _modelSetting->GetUserDataFile();
    path = _modelHomeDir + path;
    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    LoadUserData(buffer, size);
    L2dPal::ReleaseBytes(buffer);
  }

  // EyeBlinkIds
  {
    csmInt32 eyeBlinkIdCount = _modelSetting->GetEyeBlinkParameterCount();
    for (csmInt32 i = 0; i < eyeBlinkIdCount; ++i)
      _eyeBlinkIds.PushBack(_modelSetting->GetEyeBlinkParameterId(i));
  }

  // LipSyncIds
  {
    csmInt32 lipSyncIdCount = _modelSetting->GetLipSyncParameterCount();
    for (csmInt32 i = 0; i < lipSyncIdCount; ++i)
      _lipSyncIds.PushBack(_modelSetting->GetLipSyncParameterId(i));
  }

  if (_modelSetting == NULL || _modelMatrix == NULL) {
    L2dPal::PrintLog("Failed to SetupModel().");
    return;
  }

  // Layout
  csmMap<csmString, csmFloat32> layout;
  _modelSetting->GetLayoutMap(layout);
  _modelMatrix->SetupFromLayout(layout);

  _model->SaveParameters();

  for (csmInt32 i = 0; i < _modelSetting->GetMotionGroupCount(); i++) {
    const csmChar* group = _modelSetting->GetMotionGroupName(i);
    PreloadMotionGroup(group);
  }

  _motionManager->StopAllMotions();

  _updating = false;
  _initialized = true;
}

void L2dModel::PreloadMotionGroup(const csmChar* group) {
  const csmInt32 count = _modelSetting->GetMotionCount(group);

  for (csmInt32 i = 0; i < count; i++) {
    // ex) idle_0
    csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, i);
    csmString path = _modelSetting->GetMotionFileName(group, i);
    path = _modelHomeDir + path;

    if (_debugMode)
      L2dPal::PrintLog("[APP]load motion: %s => [%s_%d] ", path.GetRawString(),
                       group, i);

    csmByte* buffer;
    csmSizeInt size;
    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    CubismMotion* tmpMotion = static_cast<CubismMotion*>(
        LoadMotion(buffer, size, name.GetRawString()));

    csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, i);
    if (fadeTime >= 0.0f) tmpMotion->SetFadeInTime(fadeTime);

    fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, i);
    if (fadeTime >= 0.0f) tmpMotion->SetFadeOutTime(fadeTime);
    tmpMotion->SetEffectIds(_eyeBlinkIds, _lipSyncIds);

    if (_motions[name] != nullptr) ACubismMotion::Delete(_motions[name]);
    _motions[name] = tmpMotion;

    L2dPal::ReleaseBytes(buffer);
  }
}

void L2dModel::ReleaseMotionGroup(const csmChar* group) const {
  const csmInt32 count = _modelSetting->GetMotionCount(group);
  for (csmInt32 i = 0; i < count; i++) {
    csmString voice = _modelSetting->GetMotionSoundFileName(group, i);
    if (strcmp(voice.GetRawString(), "") != 0) {
      csmString path = voice;
      path = _modelHomeDir + path;
    }
  }
}

void L2dModel::ReleaseMotions() {
  for (csmMap<csmString, ACubismMotion*>::const_iterator iter =
           _motions.Begin();
       iter != _motions.End(); ++iter)
    ACubismMotion::Delete(iter->Second);
  _motions.Clear();
}

void L2dModel::ReleaseExpressions() {
  for (csmMap<csmString, ACubismMotion*>::const_iterator iter =
           _expressions.Begin();
       iter != _expressions.End(); ++iter)
    ACubismMotion::Delete(iter->Second);
  _expressions.Clear();
}

void L2dModel::Update() {
  csmFloat32 deltaTimeSeconds = 0.0;
  AppParam::getInstance().getParam("D$clock/delta", deltaTimeSeconds);
  _userTimeSeconds += deltaTimeSeconds;

  _dragManager->Update(deltaTimeSeconds);
  _dragX = _dragManager->GetX();
  _dragY = _dragManager->GetY();

  // モーションによるパラメータ更新の有無
  csmBool motionUpdated = false;

  //-----------------------------------------------------------------
  _model->LoadParameters();  // 前回セーブされた状態をロード
  if (_motionManager->IsFinished())
    // モーションの再生がない場合、待機モーションの中からランダムで再生する
    StartRandomMotion(L2dConfig::MotionGroupIdle, L2dConfig::PriorityIdle);
  else
    motionUpdated = _motionManager->UpdateMotion(
        _model, deltaTimeSeconds);  // モーションを更新
  _model->SaveParameters();         // 状態を保存
  //-----------------------------------------------------------------

  // まばたき
  if (!motionUpdated && _eyeBlink != nullptr)
    // メインモーションの更新がないとき
    _eyeBlink->UpdateParameters(_model, deltaTimeSeconds);  // 目パチ

  if (_expressionManager != nullptr)
    _expressionManager->UpdateMotion(
        _model, deltaTimeSeconds);  // 表情でパラメータ更新（相対変化）

  // ドラッグによる変化
  // ドラッグによる顔の向きの調整
  _model->AddParameterValue(_idParamAngleX,
                            _dragX * 30);  // -30から30の値を加える
  _model->AddParameterValue(_idParamAngleY, _dragY * 30);
  _model->AddParameterValue(_idParamAngleZ, _dragX * _dragY * -30);

  // ドラッグによる体の向きの調整
  _model->AddParameterValue(_idParamBodyAngleX,
                            _dragX * 10);  // -10から10の値を加える

  // ドラッグによる目の向きの調整
  _model->AddParameterValue(_idParamEyeBallX, _dragX);  // -1から1の値を加える
  _model->AddParameterValue(_idParamEyeBallY, _dragY);

  // 呼吸など
  if (_breath != nullptr) _breath->UpdateParameters(_model, deltaTimeSeconds);

  // 物理演算の設定
  if (_physics != nullptr) _physics->Evaluate(_model, deltaTimeSeconds);

  // リップシンクの設定
  if (_lipSync) {
    // リアルタイムでリップシンクを行う場合、システムから音量を取得して0〜1の範囲で値を入力します。
    csmFloat32 rms_value = 0.0f;

    // 状態更新/RMS値取得
    AppParam::getInstance().getParam("D$voice/rms", rms_value);
    for (csmUint32 i = 0; i < _lipSyncIds.GetSize(); ++i)
      _model->AddParameterValue(_lipSyncIds[i], rms_value, 3.0f);
  }

  // ポーズの設定
  if (_pose != nullptr) _pose->UpdateParameters(_model, deltaTimeSeconds);

  _model->Update();
}

void L2dModel::Draw(CubismMatrix44& matrix) {
  if (_model == nullptr) return;
  matrix.MultiplyByMatrix(_modelMatrix);
  GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->SetMvpMatrix(&matrix);
  GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->DrawModel();
}

CubismMotionQueueEntryHandle L2dModel::StartMotion(
    const csmChar* group, csmInt32 no, csmInt32 priority,
    ACubismMotion::FinishedMotionCallback onFinishedMotionHandler) {
  if (priority == L2dConfig::PriorityForce)
    _motionManager->SetReservePriority(priority);
  else if (!_motionManager->ReserveMotion(priority)) {
    if (_debugMode) L2dPal::PrintLog("[APP]can't start motion.");
    return InvalidMotionQueueEntryHandleValue;
  }

  const csmString fileName = _modelSetting->GetMotionFileName(group, no);

  // ex) idle_0
  csmString name = Utils::CubismString::GetFormatedString("%s_%d", group, no);
  CubismMotion* motion =
      static_cast<CubismMotion*>(_motions[name.GetRawString()]);
  csmBool autoDelete = false;

  if (motion == nullptr) {
    csmString path = fileName;
    path = _modelHomeDir + path;

    csmByte* buffer;
    csmSizeInt size;
    buffer = L2dPal::LoadFileAsBytes(path.GetRawString(), &size);
    motion = static_cast<CubismMotion*>(
        LoadMotion(buffer, size, nullptr, onFinishedMotionHandler));
    csmFloat32 fadeTime = _modelSetting->GetMotionFadeInTimeValue(group, no);
    if (fadeTime >= 0.0f) motion->SetFadeInTime(fadeTime);
    fadeTime = _modelSetting->GetMotionFadeOutTimeValue(group, no);
    if (fadeTime >= 0.0f) motion->SetFadeOutTime(fadeTime);
    motion->SetEffectIds(_eyeBlinkIds, _lipSyncIds);
    autoDelete = true;  // 終了時にメモリから削除

    L2dPal::ReleaseBytes(buffer);
  } else
    motion->SetFinishedMotionHandler(onFinishedMotionHandler);

  if (_debugMode) L2dPal::PrintLog("[APP]start motion: [%s_%d]", group, no);
  return _motionManager->StartMotionPriority(motion, autoDelete, priority);
}

CubismMotionQueueEntryHandle L2dModel::StartRandomMotion(
    const csmChar* group, csmInt32 priority,
    ACubismMotion::FinishedMotionCallback onFinishedMotionHandler) {
  if (_modelSetting->GetMotionCount(group) == 0)
    return InvalidMotionQueueEntryHandleValue;
  csmInt32 no = rand() % _modelSetting->GetMotionCount(group);
  return StartMotion(group, no, priority, onFinishedMotionHandler);
}

csmBool L2dModel::HitTest(const csmChar* hitAreaName, csmFloat32 x,
                          csmFloat32 y) {
  // 透明時は当たり判定なし。
  if (_opacity < 1) return false;
  const csmInt32 count = _modelSetting->GetHitAreasCount();
  for (csmInt32 i = 0; i < count; i++) {
    if (strcmp(_modelSetting->GetHitAreaName(i), hitAreaName) == 0) {
      const CubismIdHandle drawID = _modelSetting->GetHitAreaId(i);
      return IsHit(drawID, x, y);
    }
  }
  return false;  // 存在しない場合はfalse
}

void L2dModel::SetExpression(const csmChar* expressionID) {
  ACubismMotion* motion = _expressions[expressionID];
  if (_debugMode) L2dPal::PrintLog("[APP]expression: [%s]", expressionID);

  if (motion != nullptr)
    _expressionManager->StartMotionPriority(motion, false,
                                            L2dConfig::PriorityForce);
  else if (_debugMode)
    L2dPal::PrintLog("[APP]expression[%s] is null ", expressionID);
}

void L2dModel::SetRandomExpression() {
  if (_expressions.GetSize() == 0) return;

  csmInt32 no = rand() % _expressions.GetSize();
  csmMap<csmString, ACubismMotion*>::const_iterator map_ite;
  csmInt32 i = 0;
  for (map_ite = _expressions.Begin(); map_ite != _expressions.End();
       map_ite++) {
    if (i == no) {
      csmString name = (*map_ite).First;
      SetExpression(name.GetRawString());
      return;
    }
    i++;
  }
}

void L2dModel::ReloadRenderer() {
  DeleteRenderer();
  CreateRenderer();
  SetupTextures();
}

void L2dModel::SetupTextures() {
  for (csmInt32 modelTextureNumber = 0;
       modelTextureNumber < _modelSetting->GetTextureCount();
       modelTextureNumber++) {
    // テクスチャ名が空文字だった場合はロード・バインド処理をスキップ
    if (strcmp(_modelSetting->GetTextureFileName(modelTextureNumber), "") == 0)
      continue;
    // OpenGLのテクスチャユニットにテクスチャをロードする
    csmString texturePath =
        _modelSetting->GetTextureFileName(modelTextureNumber);
    texturePath = _modelHomeDir + texturePath;

    L2dTexManager::TextureInfo* texture =
        _texManager->CreateTextureFromPngFile(texturePath.GetRawString());
    const csmInt32 glTextueNumber = texture->id;

    // OpenGL
    GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->BindTexture(
        modelTextureNumber, glTextueNumber);
  }
#ifdef PREMULTIPLIED_ALPHA_ENABLE
  GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(
      true);
#else
  GetRenderer<Rendering::CubismRenderer_OpenGLES2>()->IsPremultipliedAlpha(
      false);
#endif
}

void L2dModel::MotionEventFired(const csmString& eventValue) {
  CubismLogInfo("%s is fired on L2dModel!!", eventValue.GetRawString());
}
