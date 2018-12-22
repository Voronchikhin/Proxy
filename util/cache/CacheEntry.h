//
// Created by maxim on 22.12.18.
//

#ifndef PROXY_CACHEENTRY_H
#define PROXY_CACHEENTRY_H


//
// Created by maxim on 22.12.18.
//

#include "CacheEntry.h"
#include "../../Observer.h"
#include "../../RequestData.h"
#include <cstring>
#include <sstream>
#include <cstdio>
#include <sys/socket.h>

class CacheEntry {
    std::string url;
    char* data;
    size_t current_length;
    pthread_mutex_t mutex;
    pthread_cond_t cond_reader;
    pthread_cond_t cond_writer;

    int count_of_readers_which_have_read_all_buffer;
    int count_of_readers ;

    Observer<RequestData> *observer1;

    bool is_finished;
    bool is_invalid;
    bool is_streaming;
    bool is_checked_response;

    const size_t MAX_DATA_SIZE;
    const int OK_RESPONSE;

public:

    const static int SUCCESS = 0;
    const static int COMMON_ERROR = -1;
    const static int DELETE_CACHE_ENTRY = -2;

    explicit CacheEntry(std::string const&url):
                is_finished(false),
                is_invalid(false),
                is_streaming(false),
                is_checked_response(false),
                MAX_DATA_SIZE(5*1024*1024),
                OK_RESPONSE(200),
                count_of_readers_which_have_read_all_buffer(0),
                count_of_readers(0),
                current_length(0),
                observer1(NULL){
        data = new char[MAX_DATA_SIZE];
        //mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond_reader,NULL);
        pthread_cond_init(&cond_writer,NULL);
        this -> url = url;
    }

    int read_to_browser(int socket_fd, char *src) {
        ssize_t pos = 0;

        while(true) {
            pthread_mutex_lock(&mutex);

            while ((pos >= current_length && !is_finished && !is_invalid) || -1 == pos) {
                pthread_cond_wait(&cond_reader, &mutex);

                if (-1 == pos && count_of_readers_which_have_read_all_buffer != count_of_readers) {
                    pos = 0;
                }
            }

            if (is_finished && current_length == pos) {
                count_of_readers--;

                int result = SUCCESS;

                if (is_streaming && 0 == count_of_readers) {
                    result = DELETE_CACHE_ENTRY;
                }

                pthread_mutex_unlock(&mutex);
                return result;

            }

            if (is_invalid) {
                int result = COMMON_ERROR;
                count_of_readers--;

                if (0 == count_of_readers) {
                    result = DELETE_CACHE_ENTRY;
                }

                pthread_mutex_unlock(&mutex);
                return result;
            }


            pthread_mutex_unlock(&mutex);

            ssize_t count_of_sent_chars = send(socket_fd, data + pos, current_length - pos, MSG_NOSIGNAL);

            pthread_mutex_lock(&mutex);
            if (-1 == count_of_sent_chars) {
                count_of_readers--;

                if (is_finished) {
                    if (is_streaming) {
                        if (0 == count_of_readers) {
                            pthread_mutex_unlock(&mutex);
                            return DELETE_CACHE_ENTRY;
                        } else {
                            pthread_mutex_unlock(&mutex);
                            return COMMON_ERROR;
                        }
                    } else {
                        pthread_mutex_unlock(&mutex);
                        return COMMON_ERROR;
                    }
                } else {
                    pthread_cond_signal(&cond_writer);
                    pthread_mutex_unlock(&mutex);
                    return COMMON_ERROR;
                }
            }

            pos += count_of_sent_chars;

            if (pos == MAX_DATA_SIZE && !is_finished) {
                count_of_readers_which_have_read_all_buffer++;

                if (count_of_readers_which_have_read_all_buffer >= count_of_readers) {
                    pthread_cond_signal(&cond_writer);
                }

                pos = -1;
            }

            pthread_mutex_unlock(&mutex);
        }
    }

    int read_from_server(int socket_fd) {
        while(true) {
            pthread_mutex_lock(&mutex);

            while (0 == MAX_DATA_SIZE - current_length &&
                   count_of_readers_which_have_read_all_buffer < count_of_readers) {
                pthread_cond_wait(&cond_writer, &mutex);
            }

            if (0 == MAX_DATA_SIZE - current_length && count_of_readers_which_have_read_all_buffer >= count_of_readers) {
                if (!is_streaming) {
                    observer1->eraseCache(url);
                    is_streaming = true;
                }
                if (0 == count_of_readers) {
                    pthread_mutex_unlock(&mutex);
                    return DELETE_CACHE_ENTRY;
                }

                count_of_readers_which_have_read_all_buffer = 0;
                current_length = 0;
            }

            pthread_mutex_unlock(&mutex);

            ssize_t count_of_received_bytes = recv(socket_fd, data + current_length,
                                                   MAX_DATA_SIZE - current_length, 0);

            pthread_mutex_lock(&mutex);


            current_length += count_of_received_bytes;

            if (-1 == count_of_received_bytes) {
                if (0 == count_of_readers) {
                    observer1->eraseCache(url);
                    pthread_mutex_unlock(&mutex);
                    return DELETE_CACHE_ENTRY;
                } else {
                    is_invalid = true;
                    pthread_cond_broadcast(&cond_reader);
                    pthread_mutex_unlock(&mutex);
                    return COMMON_ERROR;
                }
            }

            if (0 == count_of_received_bytes) {
                is_finished = true;
                pthread_cond_broadcast(&cond_reader);
                pthread_mutex_unlock(&mutex);
                return SUCCESS;
            }

            if (!is_checked_response) {
                for (size_t k = 0; k < count_of_received_bytes; ++k) {
                    if (data[current_length - count_of_received_bytes + k] == '\n') {
                        std::stringstream stringstream1(data);

                        std::string buffer;

                        stringstream1 >> buffer;
                        stringstream1 >> buffer;

                        std::stringstream  ss(buffer);

                        int response_code;
                        ss>>response_code;

                        if (OK_RESPONSE != response_code) {
                            observer1->eraseCache(url);
                        }

                        is_checked_response = true;
                        break;
                    }
                }
            }

            pthread_cond_broadcast(&cond_reader);
            pthread_mutex_unlock(&mutex);
        }
    }

    void add_reader() {
        pthread_mutex_lock(&mutex);
        count_of_readers++;
        pthread_mutex_unlock(&mutex);
    }

    void add_observer(Observer<RequestData> *observer2) {
        pthread_mutex_lock(&mutex);
        this -> observer1 = observer2;
        pthread_mutex_unlock(&mutex);
    }

    int get_count_of_readers() {
        return count_of_readers;
    }

    void mark_invalid() {
        pthread_mutex_lock(&mutex);

        this -> is_invalid = true;
        pthread_cond_broadcast(&cond_reader);

        pthread_mutex_unlock(&mutex);
    }


    virtual ~CacheEntry() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_reader);
        pthread_cond_destroy(&cond_writer);
        delete[] data;
    }

};

#endif //PROXY_CACHEENTRY_H
