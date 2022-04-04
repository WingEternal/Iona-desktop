#ifndef IONA_DESKTOP_CORE_EX1_L2D_DEFINE_H
#define IONA_DESKTOP_CORE_EX1_L2D_DEFINE_H

#include <CubismFramework.hpp>

namespace IonaDesktop {
namespace CoreEx1 {
    class L2dConfig
    {
    public:
        // 拡大縮小率の最大値
        static constexpr Csm::csmFloat32 ViewMaxScale = 2.0f;
        // 拡大縮小率の最小値
        static constexpr Csm::csmFloat32 ViewMinScale = 0.8f;

        // 論理的なビュー座標系の左端の値
        static constexpr Csm::csmFloat32 ViewLogicalLeft = -1.0f;
        // 論理的なビュー座標系の右端の値
        static constexpr Csm::csmFloat32 ViewLogicalRight = -1.0f;

        // 論理的なビュー座標系の左端の最大値
        static constexpr Csm::csmFloat32 ViewLogicalMaxLeft = -2.0f;
        // 論理的なビュー座標系の右端の最大値
        static constexpr Csm::csmFloat32 ViewLogicalMaxRight = 2.0f;
        // 論理的なビュー座標系の下端の最大値
        static constexpr Csm::csmFloat32 ViewLogicalMaxBottom = -2.0f;
        // 論理的なビュー座標系の上端の最大値
        static constexpr Csm::csmFloat32 ViewLogicalMaxTop = 2.0f;

        // モデル定義--------------------------------------------
        // モデルを配置したディレクトリ名の配列. ディレクトリ名とmodel3.jsonの名前を一致させておく.
        static constexpr const Csm::csmChar* ModelDir[] = {"Haru"};
        // モデルディレクトリ配列のサイズ
        static constexpr Csm::csmInt32 ModelDirSize =
            sizeof(ModelDir) / sizeof(const Csm::csmChar*);

                                                        // 外部定義ファイル(json)と合わせる
        // アイドリング時に再生するモーションのリスト
        static constexpr const Csm::csmChar* MotionGroupIdle = "Idle";
        // 体をタップした時に再生するモーションのリスト
        static constexpr const Csm::csmChar* MotionGroupTapBody = "TapBody";

                                                        // 外部定義ファイル(json)と合わせる
        // 当たり判定の[Head]タグ
        static constexpr const Csm::csmChar* HitAreaNameHead = "Head";
        // 当たり判定の[Body]タグ
        static constexpr const Csm::csmChar* HitAreaNameBody = "Body";

                                                        // モーションの優先度定数
        // モーションの優先度定数: 0
        static constexpr Csm::csmInt32 PriorityNone = 0;
        // モーションの優先度定数: 1
        static constexpr Csm::csmInt32 PriorityIdle = 1;
        // モーションの優先度定数: 2
        static constexpr Csm::csmInt32 PriorityNormal = 2;
        // モーションの優先度定数: 3
        static constexpr Csm::csmInt32 PriorityForce = 3;

                                                        // デバッグ用ログの表示
        // デバッグ用ログ表示の有効・無効
        static constexpr Csm::csmBool DebugLogEnable = true;
        // タッチ処理のデバッグ用ログ表示の有効・無効
        static constexpr Csm::csmBool DebugTouchLogEnable = false;

        // Frameworkから出力するログのレベル設定
        static constexpr Csm::CubismFramework::Option::LogLevel CubismLoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;

        // デフォルトのレンダーターゲットサイズ
//        static constexpr Csm::csmInt32 RenderTargetWidth = 1900;
//        static constexpr Csm::csmInt32 RenderTargetHeight = 1000;
    private:
        // Prohibit new instance
        L2dConfig(){}
        ~L2dConfig(){}
    };
}
}



#endif // IONA_DESKTOP_CORE_EX1_L2D_DEFINE_H
