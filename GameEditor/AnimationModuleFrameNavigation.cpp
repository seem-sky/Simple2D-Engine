#include "AnimationModuleFrameNavigation.h"
#include "moc_AnimationModuleFrameNavigation.h"

using namespace animation::ui::module;

// methods
FrameNavigation::FrameNavigation(QWidget* pParent)
    : QWidget(pParent), Ui_AnimationModuleFrameNavigation()
{
    setupUi(this);
    _connect();

    connect(m_pAdd, SIGNAL(clicked()), this, SLOT(_onFrameAdded()));
    connect(m_pRemove, SIGNAL(clicked()), this, SLOT(_onFrameRemoved()));
    connect(m_pNext, SIGNAL(clicked()), this, SLOT(_onFrameNext()));
    connect(m_pPrevious, SIGNAL(clicked()), this, SLOT(_onFramePrevious()));
}

void FrameNavigation::_connect()
{
    connect(m_pTime, SIGNAL(valueChanged(int)), this, SLOT(_onTimeChanged(int)));
    connect(m_pIndex, SIGNAL(valueChanged(int)), this, SLOT(_onIndexChanged(int)));
}

void FrameNavigation::_disconnect()
{
    disconnect(m_pTime, SIGNAL(valueChanged(int)), this, SLOT(_onTimeChanged(int)));
    disconnect(m_pIndex, SIGNAL(valueChanged(int)), this, SLOT(_onIndexChanged(int)));
}

void FrameNavigation::_setupFrame(uint32 index)
{
    _disconnect();
    if (!m_pFrames)
        return;
    m_pIndex->setValue(index);
    m_CurrentIndex = index;
    auto& frame = m_pFrames->at(index);
    m_pTime->setValue(frame.getTime());

    // setup navigation buttons
    m_pNext->setEnabled(m_pFrames->size() != index + 1);
    m_pPrevious->setEnabled(index != 0);

    _connect();
}

void FrameNavigation::setup(animation::FrameVector& frame)
{
    m_pFrames = &frame;
    if (m_pFrames->empty())
        m_pFrames->resize(1);
    m_pIndex->setMaximum(static_cast<int32>(m_pFrames->size() - 1));

    _setupFrame(0);
    emit frameSelectionChanged(m_pFrames->at(m_pIndex->value()));
}

void FrameNavigation::_onTimeChanged(int value)
{
    if (!m_pFrames)
        return;
    if (auto index = m_pIndex->value())
        m_pFrames->at(index).setTime(value);
}

void FrameNavigation::_onIndexChanged(int value)
{
    if (!m_pFrames)
        return;
    emit saveFrame(m_CurrentIndex, m_pTime->value());
    _setupFrame(value);
    emit frameSelectionChanged(m_pFrames->at(m_pIndex->value()));
}

void FrameNavigation::_onFrameAdded()
{
    if (!m_pFrames)
        return;
    uint32 newIndex = m_pIndex->value() + 1;
    emit saveFrame(newIndex, m_pTime->value());
    m_pFrames->insert(m_pFrames->begin() + newIndex, animation::FrameData());
    m_pIndex->setMaximum(static_cast<int32>(m_pFrames->size() - 1));
    m_pIndex->setValue(newIndex);
}

void FrameNavigation::_onFrameRemoved()
{
    if (!m_pFrames || m_pFrames->size() <= 1)
        return;

    uint32 index = m_pIndex->value();
    m_pFrames->erase(m_pFrames->begin() + index);
    m_pIndex->setMaximum(static_cast<int32>(m_pFrames->size() - 1));
    _setupFrame(index < m_pFrames->size() ? index : static_cast<uint32>(m_pFrames->size()) - 1);
    emit frameSelectionChanged(m_pFrames->at(m_pIndex->value()));
}

void FrameNavigation::_onFrameNext()
{
    if (!m_pFrames || m_pIndex->value() == m_pFrames->size() - 1)
        return;
    emit saveFrame(m_pIndex->value(), m_pTime->value());
    _setupFrame(m_pIndex->value() + 1);
    emit frameSelectionChanged(m_pFrames->at(m_pIndex->value()));
}

void FrameNavigation::_onFramePrevious()
{
    if (m_pIndex->value() == 0)
        return;
    emit saveFrame(m_pIndex->value(), m_pTime->value());
    _setupFrame(m_pIndex->value() - 1);
    emit frameSelectionChanged(m_pFrames->at(m_pIndex->value()));
}

void FrameNavigation::clear()
{
    _disconnect();
    m_pIndex->setValue(0);
    m_pTime->setValue(0);
    m_CurrentIndex = 0;
    m_pFrames = nullptr;
    _connect();
}

void FrameNavigation::saveCurrent()
{
    emit saveFrame(m_pIndex->value(), m_pTime->value());
}
