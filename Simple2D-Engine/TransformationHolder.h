#ifndef TRANSFORMATION_HOLDER_H
#define TRANSFORMATION_HOLDER_H

#include <Global.h>

namespace TRANSFORMATION
{
    enum TransformationProcess
    {
        IN_PROGRESS,
        DONE
    };

    enum TransformationType
    {
        TRANSFORMATION_MOVE
    };
    const uint32 TRANSFORMATION_TYPES_COUNT = 1;

    /*#####
    # Transformations
    #####*/
    class Transformation
    {
    private:
        virtual void _update(uint32 uiDiff) = 0;

    public:
        Transformation(uint32 uiTime, TransformationType type);

        TransformationProcess update(uint32 uiDiff);

        inline uint32 getTimeRemain() const { return m_uiTimer; }

        inline TransformationType getType() const { return m_Type; }

    private:
        TransformationType m_Type;
        uint32 m_uiTimer;
    };
    typedef std::unique_ptr<Transformation> TransformationPtr;
    typedef std::list<TransformationPtr> TransformationPtrList;

    class Move : public Transformation
    {
    private:
        void _update(uint32 uiDiff);

    public:
        Move(uint32 uiTime, Int32Point range, Int32Point& position);

    private:
        DoublePoint m_RangePerMSEC;
        DoublePoint m_RangeBuffer;
        Int32Point m_Range;
        Int32Point& m_Position;
    };

    /*#####
    # TransformationHolder
    #####*/
    class TransformationHolder
    {
    public:
        void updateTransformations(uint32 uiDiff);

        void addTransformation(Transformation* pTransformation);
        void clearTransformation(TransformationType type);
        void clearAllTransformations();

    private:
        typedef std::array<TransformationPtrList, TRANSFORMATION_TYPES_COUNT> TransformationArray;
        TransformationArray m_Transformations;
    };
}
#endif