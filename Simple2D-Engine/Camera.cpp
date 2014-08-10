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

void Camera::setPosition(GEOMETRY::Point<int32> newPos)
{
    m_SceneRect.setTopLeft(GEOMETRY::Point<int32>(newPos.getX() - m_SceneRect.getWidth(), newPos.getY() - m_SceneRect.getHeight()));
}

void Camera::move(GEOMETRY::Point<int32> range, uint32 uiTime)
{
    m_TransformationHolder.addTransformation(new TRANSFORMATION::Move(uiTime, range, m_SceneRect.getTopLeft()));
}

void Camera::stopMoving()
{
    m_TransformationHolder.clearTransformation(TRANSFORMATION::TRANSFORMATION_MOVE);
}

/*#####
# MapCamera
#####*/
MapCamera::MapCamera(PLAYER::GamePlayer* pOwner) : Camera(), m_pOwner(pOwner), m_CameraSettings(FREE)
{}

void MapCamera::centerOnPlayer()
{
    stopMoving();
    m_CameraSettings = CENTER_ON_PLAYER;
}

void MapCamera::centerOn(const MAP::OBJECT::WorldObject* pWorldObj)
{
    stopMoving();
    m_CameraSettings = CENTER_ON_WORLD_OBJECT;
}

void MapCamera::update(uint32 uiDiff)
{
    switch (m_CameraSettings)
    {
    case CENTER_ON_PLAYER:
        if (auto pObj = m_pOwner->getControledObject())
        {
            pObj->getPosition();
        }
        break;
    case CENTER_ON_WORLD_OBJECT: break;
    default: Camera::update(uiDiff); break;
    }
}

void MapCamera::move(GEOMETRY::Point<int32> range, uint32 uiTime)
{
    Camera::move(range, uiTime);
    m_CameraSettings = FREE;
}