//
// Created by renz2048 on 2020/7/9.
//

#ifndef FAKEMUDUO_CURRENTTHREAD_H
#define FAKEMUDUO_CURRENTTHREAD_H


namespace fakeMuduo
{
    namespace CurrentThread
    {
        // internal
        extern __thread int t_cachedTid;
    }
}


#endif //FAKEMUDUO_CURRENTTHREAD_H
