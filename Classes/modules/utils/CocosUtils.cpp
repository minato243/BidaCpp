#include "CocosUtils.h"

cocos2d::Vec3 CocosUtils::transVecToCCVec(ps::ExtMath::vector vec)
{
    return cocos2d::Vec3(vec.x, vec.z, -vec.y);
}

ps::ExtMath::vector CocosUtils::transVecToCCVec(cocos2d::Vec3 vec)
{
    return ps::ExtMath::vector(vec.x, -vec.z, vec.y);
}
