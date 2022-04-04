#include "core_ex1/l2d_sprite.h"

using namespace IonaDesktop::Core;
using namespace IonaDesktop::CoreEx1;

L2dSprite::L2dSprite(float x, float y, float width, float height, GLuint textureId, GLuint programId)
    : _rect()
{
    _rect.left = (x - width * 0.5f);
    _rect.right = (x + width * 0.5f);
    _rect.up = (y + height * 0.5f);
    _rect.down = (y - height * 0.5f);
    _textureId = textureId;

    // 何番目のattribute変数か
    _positionLocation = GLEntry::get()->glGetAttribLocation(programId, "position");
    _uvLocation      = GLEntry::get()->glGetAttribLocation(programId, "uv");
    _textureLocation = GLEntry::get()->glGetUniformLocation(programId, "texture");
    _colorLocation = GLEntry::get()->glGetUniformLocation(programId, "baseColor");

    _spriteColor[0] = 1.0f;
    _spriteColor[1] = 1.0f;
    _spriteColor[2] = 1.0f;
    _spriteColor[3] = 1.0f;
}

L2dSprite::~L2dSprite()
{
}

void L2dSprite::Render() const
{
    // 画面サイズを取得する
    int maxWidth, maxHeight;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &maxWidth, &maxHeight);

    if (maxWidth == 0 || maxHeight == 0)
    {
        return; // この際は描画できず
    }

    glEnable(GL_TEXTURE_2D);
    const GLfloat uvVertex[] =
    {
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };

    // attribute属性を有効にする
    GLEntry::get()->glEnableVertexAttribArray(_positionLocation);
    GLEntry::get()->glEnableVertexAttribArray(_uvLocation);

    // uniform属性の登録
    GLEntry::get()->glUniform1i(_textureLocation, 0);

    // 頂点データ
    float positionVertex[] =
    {
        (_rect.right - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.up   - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.left  - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.up   - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.left  - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.down - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.right - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.down - maxHeight * 0.5f) / (maxHeight * 0.5f)
    };

    // attribute属性を登録
    GLEntry::get()->glVertexAttribPointer(_positionLocation, 2, GL_FLOAT, false, 0, positionVertex);
    GLEntry::get()->glVertexAttribPointer(_uvLocation, 2, GL_FLOAT, false, 0, uvVertex);

    GLEntry::get()->glUniform4f(_colorLocation, _spriteColor[0], _spriteColor[1], _spriteColor[2], _spriteColor[3]);


    // モデルの描画
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void L2dSprite::RenderImmidiate(GLuint textureId, const GLfloat uvVertex[8]) const
{
    // 画面サイズを取得する
    int maxWidth, maxHeight;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &maxWidth, &maxHeight);

    if (maxWidth == 0 || maxHeight == 0)
    {
        return; // この際は描画できず
    }

    glEnable(GL_TEXTURE_2D);

    // attribute属性を有効にする
    GLEntry::get()->glEnableVertexAttribArray(_positionLocation);
    GLEntry::get()->glEnableVertexAttribArray(_uvLocation);

    // uniform属性の登録
    GLEntry::get()->glUniform1i(_textureLocation, 0);

    // 頂点データ
    float positionVertex[] =
    {
        (_rect.right - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.up - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.left - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.up - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.left - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.down - maxHeight * 0.5f) / (maxHeight * 0.5f),
        (_rect.right - maxWidth * 0.5f) / (maxWidth * 0.5f), (_rect.down - maxHeight * 0.5f) / (maxHeight * 0.5f)
    };

    // attribute属性を登録
    GLEntry::get()->glVertexAttribPointer(_positionLocation, 2, GL_FLOAT, false, 0, positionVertex);
    GLEntry::get()->glVertexAttribPointer(_uvLocation, 2, GL_FLOAT, false, 0, uvVertex);

    GLEntry::get()->glUniform4f(_colorLocation, _spriteColor[0], _spriteColor[1], _spriteColor[2], _spriteColor[3]);

    // モデルの描画
    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool L2dSprite::IsHit(float pointX, float pointY) const
{
    // 画面サイズを取得する
    int maxWidth, maxHeight;
    glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &maxWidth, &maxHeight);

    if (maxWidth == 0 || maxHeight == 0)
    {
        return false; // この際は描画できず
    }

    //Y座標は変換する必要あり
    float y = maxHeight - pointY;

    return (pointX >= _rect.left && pointX <= _rect.right && y <= _rect.up && y >= _rect.down);
}

void L2dSprite::SetColor(float r, float g, float b, float a)
{
    _spriteColor[0] = r;
    _spriteColor[1] = g;
    _spriteColor[2] = b;
    _spriteColor[3] = a;
}

void L2dSprite::ResetRect(float x, float y, float width, float height)
{
    _rect.left = (x - width * 0.5f);
    _rect.right = (x + width * 0.5f);
    _rect.up = (y + height * 0.5f);
    _rect.down = (y - height * 0.5f);
}
