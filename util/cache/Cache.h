//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_CACHE_H
#define PROXY_CACHE_H

#include <map>
#include <sys/param.h>
#include <pthread.h>
#include "../logging/Logger.h"
#include "../../exception/CacheException.h"

template<typename K, typename V>
class Cache {
public:
    Cache() : logger(Logger()) {
        int creationResult = pthread_rwlock_init(&rwlock, NULL);
        if (0 != creationResult) {
            logger.warn("Cache creation error at : creation rw_lock");
            throw CacheException();
        }
        logger.info("Cache creation success");
    }
    ~Cache(){
        pthread_rwlock_destroy(&rwlock);
    }

    void insert(K key, V value) {
        wLock();
        storage.insert(std::pair<K, V>(key, value));
        unlock();
    }

    void erase(K key){
        wLock();
        storage.erase(key);
        unlock();
    };

    V get(K key) {
        V result;
        rLock();
        result = storage[key];
        unlock();
        return result;
    };

private:
    void rLock(){
        int res = pthread_rwlock_rdlock(&rwlock);
        if( 0!= res ){
            logger.warn( "error during rLock" );
            throw  CacheException();
        }
    }
    void wLock(){
        int res = pthread_rwlock_wrlock(&rwlock);
        if( 0!= res ){
            logger.warn( "error during wLock" );
            throw  CacheException();
        }
    }
    void unlock(){
        int res = pthread_rwlock_unlock(&rwlock);
        if( 0!= res ){
            logger.warn( "error during unlock" );
            throw  CacheException();
        }
    }
    std::map<K, V> storage;
    pthread_rwlock_t rwlock;
    Logger logger;
};


#endif //PROXY_CACHE_H
