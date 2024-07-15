#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    ui2WidgetBase::ui2WidgetBase() {
        m_refCount = 0;
    }

    ui2WidgetBase::~ui2WidgetBase() {
    }
    
    void ui2WidgetBase::addRef() {
        m_refCount++;
    }
    
    void ui2WidgetBase::baseReleaseRef() {
        m_refCount--;

        // uhhhh... Rockstar? Why have you done this
        if (m_refCount == 0 && this) delete this;
    }
    
    u32 ui2WidgetBase::getRefCount() const {
        return m_refCount;
    }
};