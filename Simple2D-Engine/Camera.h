#ifndef CAMERA_H
#define CAMERA_H

#include "TransformationHolder.h"
#include "WorldObject.h"

namespace PLAYER
{
    class GamePlayer;
    namespace CAMERA
    {
        class Camera
        {
        public:
            Camera();

            // camera positions
            inline GEOMETRY::Point<int32> getPosition() const { return GEOMETRY::Point<int32>(getPositionX(), getPositionY()); }
            inline int32 getPositionX() const { return m_SceneRect.getX() + m_SceneRect.getWidth()/2; }
            inline int32 getPositionY() const { return m_SceneRect.getY() + m_SceneRect.getHeight()/2; }
            virtual void move(GEOMETRY::Point<int32> about, uint32 uiTime);
            void stopMoving();
            void setPosition(GEOMETRY::Point<int32> newPos);

            void setSize(GEOMETRY::Point<uint32> newSize);
            inline GEOMETRY::Size<uint32> getSize() const { return GEOMETRY::static_size_cast<uint32>(m_SceneRect.getSize()); }
            inline uint32 getWidth() const { return m_SceneRect.getWidth(); }
            inline uint32 getHeight() const { return m_SceneRect.getHeight(); }
            inline GEOMETRY::Point<int32> getScreenPos() const { return m_SceneRect.getPosition(); }
            inline int32 getScreenPosX() const { return m_SceneRect.getX(); }
            inline int32 getScreenPosY() const { return m_SceneRect.getY(); }

            virtual void update(uint32 uiDiff);

        private:
            GEOMETRY::Rectangle<int32> m_SceneRect;
            TRANSFORMATION::TransformationHolder m_TransformationHolder;
        };

        /*#####
        # MapCamera
        #####*/
        enum CameraSettings
        {
            FREE,
            CENTER_ON_PLAYER,
            CENTER_ON_WORLD_OBJECT
        };

        class MapCamera : public Camera
        {
        public:
            MapCamera(GamePlayer* pOwner);

            void move(GEOMETRY::Point<int32> about, uint32 uiTime);

            void update(uint32 uiDiff);

            // camera settings
            void centerOnPlayer();
            void centerOn(const MAP::OBJECT::WorldObject* pWorldObj);

        private:
            GamePlayer* m_pOwner;
            CameraSettings m_CameraSettings;
        };
    }
}
#endif