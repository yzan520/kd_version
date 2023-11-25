//
// Created by zps on 23-11-24.
//

#ifndef MY_TEST_MESSAGE_H
#define MY_TEST_MESSAGE_H

#include <pybind11/pybind11.h> //用于连接c++和python的库，创建python拓展模块，使得c++代码可以直接被python调用
#include <pybind11/embed.h> // 用于支持在c++中嵌入python解释器
#include <queue> //队列容器
#include <mutex> //用于包含互斥锁相关的定义和功能
#include <condition_variable> //用于支持条件变量,条件变量是多线程编程中用于线程间同步的一种机制
#include <list> // 上交中没有???

namespace umt {
    /**
     * @beief 消息异常类型
     */
    class MessageError : public std::runtime_error {
    protected:
        using std::runtime_error::runtime_error;
    };

    /**
     * @brief 消息异常类型，当前消息上无publisher
     */
    class MessageError_Stopped : public MessageError {
    public:
        MessageError_Stopped() : MessageError("no publisher on this message !"){}
    };

    /**
     * @brief 消息异常类型，消息读取超时
     */
    class MessageError_Timeout : public MessageError {
    public:
        MessageError_Timeout() : MessageError("message read timeout!") {}
    };

     /**
     * @brief 消息异常类型，空消息（未初始化或使用过std::move）
     */
    class MessageError_Empty : public MessageError {
    public:
        MessageError_Empty() : MessageError("empty message. maybe uninitailized or moved!") {}
    };

    template<class T>
    class Publisher;

    template<class T>
    class Subscriber;

    namespace utils {
        template<class T>
        class MessagePipe {
            friend class Publisher<T>;

            friend class Subscriber<T>;

        public:
            using MsgType = T;
        private:
            std::mutex pubs_mtx;
            std::list<Publisher<T> *> pubs;
            std::mutex subs_mtx;
            std::list<Subscriber<T> *> subs;
        };
    }

    /**
     * @brief 消息订阅器类型
     * @details 使用队列存储收到的消息，可以设置队列最大长度，当超出最大队列长度时，新消息会覆盖最老的消息
     * @tparam T 消息对象类型
     */
     template<class T>
     class Subscriber {
         friend Publisher<T>;
     private:
        using MsgManager = ObjManager<utils::MessagePipe<T>>;
     public:
        using MsgType = T;

        Subscriber() = default;

        /**
         * @details 构造函数
         * @param msg_name 消息名称
         * @param max_file_size 最大消息长度
         */
        explicit Subscriber(const std::string &msg_name, size_t size = 1) : fifo_size(size) {
            bind(msg_name);
        }

        /// 拷贝构造函数
        Subscriber(const Subscriber &other) : fifo_size(other.fifo_size)


    private:
        void write_obj(const T &obj) {
            std::unique_lock lock(mtx);
            if (fifo_size > 0 && fifo.size() >= fifo_size) {
                fifo.pop();
            }
            fifo.push(obj);
        }

        void notify() const {
            cv.notify_one();
        }

    private:
        mutable std::mutex mtx;
        mutable std::condition_variable cv;
        size_t fifo_size{};
        std::queue<T> fifo;
        typename MsgManager::sptr p_msg;
};



#endif //MY_TEST_MESSAGE_H
