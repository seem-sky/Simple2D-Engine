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
            inline Int32Point getPosition() const { return Int32Point(getPositionX(), getPositionY()); }
            inline int32 getPositionX() const { return m_SceneRect.getPositionX() + m_SceneRect.getWidth()/2; }
            inline int32 getPositionY() const { return m_SceneRect.getPositionY() + m_SceneRect.getHeight()/2; }
            virtual void move(Int32Point about, uint32 uiTime);
            void stopMoving();
            void setPosition(Int32Point newPos);

            void setSize(UInt32Point newSize);
            inline UInt32Point getSize() const { return m_SceneRect.getSize(); }
            inline uint32 getWidth() const { return m_SceneRect.getWidth(); }
            inline uint32 getHeight() const { return m_SceneRect.getHeight(); }
            inline Int32Point getScreenPos() const { return m_SceneRect.getPosition(); }
            inline int32 getScreenPosX() const { return m_SceneRect.getPositionX(); }
            inline int32 getScreenPosY() const { return m_SceneRect.getPositionY(); }

            virtual void update(uint32 uiDiff);

        private:
            Rect<int32, uint32> m_SceneRect;
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

            void move(Int32Point about, uint32 uiTime);

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