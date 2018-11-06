#pragma once

#include <xnamath.h>

class GameSprite
{
public:
    GameSprite();
    virtual ~GameSprite();

    XMMATRIX GetWorldMatrix();

    void SetPosition(XMFLOAT2 & position);
    void SetRotation(float rotation);
    void SetScale(XMFLOAT2 & scale);

private:
    XMFLOAT2 position_;
    float rotation_;
    XMFLOAT2 scale_;
};