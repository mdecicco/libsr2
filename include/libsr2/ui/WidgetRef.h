#pragma once
#include <libsr2/types.h>

#include <type_traits>

namespace sr2 {
    class ui2Widget;
    class ui2WidgetBase;
    class ui2Master;

    template <typename T>
    class WidgetRef {
        // Circular dependencies in includes prevents me from being able to do this
        // static_assert(std::is_base_of_v<ui2WidgetBase, T>, "WidgetRef template argument must have ui2WidgetBase as a base class");

        public:
            WidgetRef() : m_ref(nullptr) {}
            WidgetRef(nullptr_t) : m_ref(nullptr) {}
            WidgetRef(T* ref) : m_ref(ref) {
                if (m_ref) m_ref->addRef();
            }
            
            WidgetRef(const WidgetRef<T>& ref) : m_ref(ref.m_ref) {
                if (m_ref) m_ref->addRef();
            }

            template <typename W>
            WidgetRef(W* ref) requires (std::is_base_of_v<T, W>) {
                m_ref = ref;
                if (ref) ref->addRef();
            }

            template <typename W>
            WidgetRef(const WidgetRef<W>& ref) requires (std::is_base_of_v<T, W>) {
                m_ref = (T*)*ref;
                if (m_ref) m_ref->addRef();
            }

            ~WidgetRef() {
                release();
            }

            operator bool() const {
                return m_ref != nullptr;
            }

            bool operator !() const {
                return m_ref == nullptr;
            }

            T* operator->() const {
                return m_ref;
            }

            T* operator*() const {
                return m_ref;
            }

            template <typename W>
            std::enable_if_t<std::is_base_of_v<T, W> || std::is_same_v<T, W>, void>
            operator=(W* ref) {
                release();
                
                m_ref = ref;
                if (m_ref) m_ref->addRef();
            }

            template <typename W>
            bool operator==(W* ref) {
                return m_ref == (T*)ref;
            }
            
            template <typename W>
            bool operator!=(W* ref) {
                return m_ref != (T*)ref;
            }

            void operator=(const WidgetRef<T>& ref) {
                release();
                
                m_ref = (T*)*ref;
                if (m_ref) m_ref->addRef();
            }

            template <typename W>
            bool operator==(const WidgetRef<W>& ref) {
                return m_ref == (T*)*ref;
            }

            template <typename W>
            bool operator!=(const WidgetRef<W>& ref) {
                return m_ref != (T*)*ref;
            }

            void operator=(nullptr_t) {
                release();
            }

            template <typename W>
            std::enable_if_t<std::is_base_of_v<T, W> || std::is_base_of_v<W, T>, WidgetRef<W>>
            cast() const {
                return WidgetRef<W>((W*)m_ref);
            }
        
            inline void release() {
                if (m_ref) {
                    if constexpr (std::is_base_of_v<ui2Widget, T>) m_ref->releaseRef();
                    else m_ref->baseReleaseRef();

                    if (m_ref->getRefCount() == 0) delete m_ref;
                    m_ref = nullptr;
                }
            }

        protected:
            T* m_ref;
    };

    namespace ui {
        typedef WidgetRef<ui2WidgetBase> BaseRef;
        typedef WidgetRef<ui2Widget> NamedRef;
    };
};