
#ifndef __INIT_ONCE__H__
#define __INIT_ONCE__H__
//================================================================================//
// The init once idiom, see
// http://stackoverflow.com/questions/3187770/partially-initialize-variable-defined-in-other-module

template <typename T>
class InitOnce
{
    T *instance;
    static unsigned refs;
public:
    InitOnce() {
        if (!refs++) {
            instance = new T();
        }
    }

    ~InitOnce() {
        if (!--refs) {
            delete instance;
        }
    }
};
template <typename T> unsigned InitOnce<T>::refs(0);

#endif
