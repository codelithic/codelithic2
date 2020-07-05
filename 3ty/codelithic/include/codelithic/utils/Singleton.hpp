#ifndef KUNG_FU_CONFIG_HPP
#define KUNG_FU_CONFIG_HPP

#include <memory>

namespace codelithic {

    // //CONFIG* Config::single = nullptr;
    template<typename T>
    class Singleton {
    public:
        static T *GetInstance();

        static T *GetInstance(void *);

        static T *SetInstance(T *theT);

        static void destroy();

    private:
        Singleton(Singleton const &) = default;

        Singleton &operator=(Singleton const &){};

    protected:
        static T *m_instance;

        Singleton() { m_instance = static_cast<T *>(this); };

        ~Singleton() = default;
    };

    template<typename T>
    T *Singleton<T>::m_instance{nullptr};

    template<typename T>
    T *Singleton<T>::GetInstance() {
        if (!m_instance) {
            Singleton<T>::m_instance = new T();

            Singleton<T>::m_instance->init(nullptr);
        }

        return m_instance;
    }

    template<typename T>
    T *Singleton<T>::GetInstance(void *params) {
        if (!m_instance) {
            Singleton<T>::m_instance = new T();

            Singleton<T>::m_instance->init(params);
        }

        return m_instance;
    }

    template<typename T>
    T *Singleton<T>::SetInstance(T *theT) {
        if (!m_instance) {
            Singleton<T>::m_instance = theT;
        }

        return m_instance;
    }

    template<typename T>
    void Singleton<T>::destroy() {
        delete Singleton<T>::m_instance;
        Singleton<T>::m_instance = nullptr;
    }
}// namespace codelithic
#endif