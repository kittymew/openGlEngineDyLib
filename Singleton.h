//
//  Singleton.h
//  openGlDyLib
//
//  Created by Hoin Kang on 2022/11/04.
//

#ifndef Single_h
#define Single_h

template <typename T>
class Singleton
{
protected:
    static T* pInstance_;
    
protected:
    virtual bool init() { return true; }

public:
    static T* get()
    {
        if(pInstance_ == nullptr)
        {
            pInstance_ = new T();
            if(pInstance_->init() == false)
            {
                delete pInstance_;
                pInstance_ = nullptr;
                assert(0);
            }
        }
        
        return pInstance_;
    }

protected:
    Singleton() = default;
};

template <typename T>
T* Singleton<T>::pInstance_ = nullptr;


#endif /* Single_h */
