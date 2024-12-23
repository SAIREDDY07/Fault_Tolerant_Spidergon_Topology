#include "model/EventInfo.h"

#include "model/PortInfo.h"
#include "model/TransitionInfo.h"

namespace DSENT
{
    EventInfo::EventInfo(const String& event_name_, const Map<PortInfo*>* port_infos_)
        : m_event_name_(event_name_)
    {
        m_trans_info_map_ = new Map<TransitionInfo>;

        // Get the name of each input port and add a transition info for it
        Map<PortInfo*>::ConstIterator it_begin = port_infos_->begin();
        Map<PortInfo*>::ConstIterator it_end = port_infos_->end();
        Map<PortInfo*>::ConstIterator it;
        for(it = it_begin; it != it_end; ++it)
        {
            const String& port_name = it->first;
            m_trans_info_map_->set(port_name, TransitionInfo());
        }
    }

    EventInfo::~EventInfo()
    {
        delete m_trans_info_map_;
    }

    const String& EventInfo::getEventName() const
    {
        return m_event_name_;
    }

    void EventInfo::setTransitionInfo(const String& port_name_, const TransitionInfo& trans_info_)
    {
        ASSERT(m_trans_info_map_->keyExist(port_name_), "[Error] " + getEventName() + 
                " -> Port (" + port_name_ + ") does not exist!");

        m_trans_info_map_->set(port_name_, trans_info_);
        return;
    }

    void EventInfo::setStaticTransitionInfo(const String& port_name_)
    {
        ASSERT(m_trans_info_map_->keyExist(port_name_), "[Error] " + getEventName() + 
                " -> Port (" + port_name_ + ") does not exist!");

        m_trans_info_map_->set(port_name_, TransitionInfo(0.5, 0.0, 0.5));
        return;
    }

    void EventInfo::setRandomTransitionInfos()
    {
        Map<TransitionInfo>::Iterator it_begin = m_trans_info_map_->begin();
        Map<TransitionInfo>::Iterator it_end = m_trans_info_map_->end();
        Map<TransitionInfo>::Iterator it;
        for(it = it_begin; it != it_end; ++it)
        {
            TransitionInfo& trans_info = it->second;
            trans_info = TransitionInfo();
        }
        return;
    }

    void EventInfo::setStaticTransitionInfos()
    {
        Map<TransitionInfo>::Iterator it_begin = m_trans_info_map_->begin();
        Map<TransitionInfo>::Iterator it_end = m_trans_info_map_->end();
        Map<TransitionInfo>::Iterator it;
        for(it = it_begin; it != it_end; ++it)
        {
            TransitionInfo& trans_info = it->second;
            trans_info = TransitionInfo(0.5, 0.0, 0.5);
        }
        return;
    }

    const TransitionInfo& EventInfo::getTransitionInfo(const String& port_name_) const
    {
        ASSERT(m_trans_info_map_->keyExist(port_name_), "[Error] " + getEventName() + 
                " -> Port (" + port_name_ + ") does not exist!");

        return m_trans_info_map_->get(port_name_);
    }
} // namespace DSENT

