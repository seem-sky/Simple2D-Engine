#include "Camera.h"
#include "Player_Game.h"

using namespace PLAYER;
using namespace CAMERA;

/*#####
# Camera
#####*/
Camera::Camera()
{}

void Camera::update(uint32 uiDiff)
{
    m_TransformationHolder.updateTransformations(uiDiff);
}

void Camera::setPosition(Int32Point newPos)
{
    m_SceneRect.setPosition(Int32Point(newPos.x - m_SceneRect.getWidth(), newPos.y - m_SceneRect.getHeight()));
}

void Camera::move(Int32Point range, uint32 uiTime)
{
    m_TransformationHolder.addTransformation(new TRANSFORMATION::Move(uiTime, range, m_SceneRect.getPosition()));
}

void Camera::stopMoving()
{
    m_TransformationHolder.clearTransformation(TRANSFORMATION::TRANSFORMATION_MOVE);
}

/*#####
# MapCamera
#####*/
MapCamera::MapCamera(PLAYER::GamePlayer *pOwner) : Camera(), m_pOwner(pOwner), m_CameraSettings(FREE)
{}

void MapCamera::centerOnPlayer()
{
    stopMoving();
    m_CameraSettings = CENTER_ON_PLAYER;
}

void MapCamera::centerOn(MAP::OBJECT::WorldObjectPtr pWorldObj)
{
    stopMoving();
    m_CameraSettings = CENTER_ON_WORLD_OBJECT;
}

void MapCamera::update(uint32 uiDiff)
{
    switch (m_CameraSettings)
    {
    case CENTER_ON_PLAYER:
        if (MAP::OBJECT::DynamicObjectPtr pObj = m_pOwner->getControledObject())
        {
            pObj->getPosition();
        }
        break;
    case CENTER_ON_WORLD_OBJECT: break;
    default: Camera::update(uiDiff); break;
    }
}

void MapCamera::move(Int32Point range, uint32 uiTime)
{
    Camera::move(range, uiTime);
    m_CameraSettings = FREE;
}