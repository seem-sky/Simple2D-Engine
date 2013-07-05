#ifndef TRANSFORMATION_HOLDER_H
#define TRANSFORMATION_HOLDER_H

#include "Global.h"

namespace TRANSFORMATION
{
    enum TransformationProcess
    {
        IN_PROGRESS,
        DONE
    };

    class Transformation
    {
    private:
        virtual void _update(uint32 uiDiff) = 0;

    public:
        Transformation(uint32 uiTime);

        TransformationProcess update(uint32 uiDiff);

        uint32 getTimeRemain() const { return m_uiTimer; }

    private:
        uint32 m_uiTimer;
    };
    typedef std::unique_ptr<Transformation> TransformationPtr;
    typedef std::list<TransformationPtr> TransformationPtrList;

    class Move : public Transformation
    {
    private:
        void _update(uint32 uiDiff);

    public:
        Move(uint32 uiTime, Int32Point &position, Int32Point range);

    private:
        DoublePoint m_RangePerMSEC;
        DoublePoint m_RangeBuffer;
        Int32Point m_Range;
        Int32Point &m_Position;
    };

    class TransformationHolder
    {
    public:
        void updateTransformations(uint32 uiDiff);
        void addTransformation(TransformationPtr &pTransformation);

    private:
        TransformationPtrList m_Transformations;
    };
}
#endif