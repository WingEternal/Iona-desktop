﻿#ifndef IONA_DESKTOP_CORE_EX1_L2D_SPRITE_H
#define IONA_DESKTOP_CORE_EX1_L2D_SPRITE_H

#include "core/gl_entry.h"

namespace IonaDesktop {
namespace CoreEx1 {
    class L2dSprite
    {
    public:
        /**
        * @brief Rect 構造体。
        */
        struct Rect
        {
        public:
            float left;     ///< 左辺
            float right;    ///< 右辺
            float up;       ///< 上辺
            float down;     ///< 下辺
        };

        /**
        * @brief コンストラクタ
        *
        * @param[in]       x            x座標
        * @param[in]       y            y座標
        * @param[in]       width        横幅
        * @param[in]       height       高さ
        * @param[in]       textureId    テクスチャID
        * @param[in]       programId    シェーダID
        */
        L2dSprite(float x, float y, float width, float height, GLuint textureId, GLuint programId);

        /**
        * @brief デストラクタ
        */
        ~L2dSprite();

        /**
        * @brief Getter テクスチャID
        * @return テクスチャIDを返す
        */
        GLuint GetTextureId() { return _textureId; }

        /**
        * @brief 描画する
        *
        */
        void Render() const;

        /**
        * @brief テクスチャIDを指定して描画する
        *
        */
        void RenderImmidiate(GLuint textureId, const GLfloat uvVertex[8]) const;

        /**
        * @brief コンストラクタ
        *
        * @param[in]       pointX    x座標
        * @param[in]       pointY    y座標
        */
        bool IsHit(float pointX, float pointY) const;

        /**
         * @brief 色設定
         *
         * @param[in]       r (0.0~1.0)
         * @param[in]       g (0.0~1.0)
         * @param[in]       b (0.0~1.0)
         * @param[in]       a (0.0~1.0)
         */
        void SetColor(float r, float g, float b, float a);

        /**
         * @brief サイズ再設定
         *
         * @param[in]       x            x座標
         * @param[in]       y            y座標
         * @param[in]       width        横幅
         * @param[in]       height       高さ
         */
        void ResetRect(float x, float y, float width, float height);

    private:
        GLuint _textureId;      ///< テクスチャID
        Rect _rect;             ///< 矩形
        int _positionLocation;  ///< 位置アトリビュート
        int _uvLocation;        ///< UVアトリビュート
        int _textureLocation;   ///< テクスチャアトリビュート
        int _colorLocation;     ///< カラーアトリビュート

        float _spriteColor[4];  ///< 表示カラー
    };
}
}

#endif // IONA_DESKTOP_CORE_EX1_L2D_SPRITE_H
