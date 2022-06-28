#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "stdafx.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class handler
{
    public:
        enum class GetAction : unsigned char {Pose, Position};
        handler();
        handler(utility::string_t url);
        virtual ~handler();

        pplx::task<void>open(){return m_listener.open();}
        pplx::task<void>close(){return m_listener.close();}

    protected:

    private:
        void handle_get(http_request message);
        void handle_put(http_request message);
        void handle_post(http_request message);
        void handle_delete(http_request message);
        void handle_error(pplx::task<void>& t);
        http_listener m_listener;
    
        static map<string, handler::GetAction> const getMapper;
};

#endif // HANDLER_H

