#ifndef SANLACLASSIC_SINGLETON_H
#define SANLACLASSIC_SINGLETON_H

#include <stddef.h>

namespace sanla {

template <class T>
class Singleton {
public:

static T* Instance() {
    if(!m_pInstance) m_pInstance = new T;
    if(m_pInstance != NULL){
        return m_pInstance;
    }
    else {
        return nullptr;
    }
}
protected:
Singleton();
~Singleton();

private:
Singleton(Singleton const&);
Singleton& operator=(Singleton const&);
static T* m_pInstance;

};

template <class T> 
T* Singleton<T>::m_pInstance=NULL;
}
#endif