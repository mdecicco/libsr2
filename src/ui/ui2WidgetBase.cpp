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

        // uhhhh... It looks like the code is doing this:
        // if (m_refCount == 0 && this) delete this;
        //
        // But I don't think I should do that
    }
    
    u32 ui2WidgetBase::getRefCount() const {
        return m_refCount;
    }
};