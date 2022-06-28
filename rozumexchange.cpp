#include <iostream>
#include <sstream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include "rozumexchange.hpp"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

RozumExchange::RozumExchange(std::string ip, int port)
    : m_client(uri_builder().set_host(ip).set_port(port).set_scheme("http").to_uri())
{
    std::cout << m_client.base_uri().to_string() << std::endl;
}

RozumExchange::~RozumExchange()
{
}

rozumResponse RozumExchange::getRequestJSON(web::http::http_request request)
{
    std::stringstream stream;
    status_code result;
    auto requestJson = m_client.request(request)
    .then([&result](http_response response)
    {
        result = response.status_code();
        if (response.status_code()!=status_codes::OK)
        {
            auto getMessage = response.extract_string().then([](std::string message)
            {
                throw std::runtime_error("Robot returned: " + message);
            });

            try
            {
                getMessage.wait();
            }    
            catch (...)
            {
                throw ;
            };
        }
        return response.extract_json();
    })
    .then([&stream](json::value value)
    {
        value.serialize(stream);      
    });
    try
    {
        requestJson.wait();
        QJsonDocument pose = QJsonDocument::fromJson(QByteArray::fromStdString(stream.str()));
        return {result,pose.object()};
    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << std::endl;
        QJsonObject res;
        res["answer"] = QString::fromLocal8Bit(e.what());
        return {result,res};
    }
}

rozumResponse RozumExchange::getRequestString(web::http::http_request request)
{
    std::string responseString;
    status_code result;
    auto requestJson = m_client.request(request)
    .then([&result](http_response response)
    {
        result = response.status_code();
        if (response.status_code()!=status_codes::OK)
        {
            auto getMessage = response.extract_string().then([result](std::string message)
            {
                throw std::runtime_error("Robot returned: " + message);
            });

            try
            {
                getMessage.wait();
            }    
            catch (...)
            {
                throw ;
            };
        }
        return response.extract_string();
    })
    .then([&responseString](std::string value)
    {
        responseString = value;      
    });
    try
    {
        requestJson.wait();
        QJsonObject res;
        res["answer"] = QString::fromStdString(responseString);
        return {result,res};
    }
    catch(const std::exception& e)
    {
        // std::cerr << e.what() << std::endl;
        QJsonObject res;
        res["answer"] = QString::fromLocal8Bit(e.what());
        return {result,res};
    }
}

rozumResponse RozumExchange::getPose()
{
    http_request req(methods::GET);
    req.set_request_uri(uri_builder("pose").to_uri());
    return getRequestJSON(req);
}

rozumResponse RozumExchange::getPosition()
{
    http_request req(methods::GET);
    req.set_request_uri(uri_builder("position").to_uri());
    return getRequestJSON(req);
}

rozumResponse RozumExchange::setPosition(QJsonObject position, QJsonObject params)
{
    http_request req(methods::PUT);
    uri_builder builder("position");
    for (auto& key : params.keys())
        builder.append_query(key.toStdString(), params[key].toVariant().toString().toStdString());
    req.set_request_uri(builder.to_uri());
    utility::stringstream_t stream;
    QJsonDocument doc (position);
    stream<< doc.toJson().toStdString();
    req.set_body(web::json::value::parse(stream));
    return getRequestString(req);
}

rozumResponse RozumExchange::freeze()
{
    http_request req(methods::PUT);
    req.set_request_uri(uri_builder("freeze").to_uri());
    return getRequestString(req);    
}

rozumResponse RozumExchange::relax()
{
    http_request req(methods::PUT);
    req.set_request_uri(uri_builder("relax").to_uri());
    return getRequestString(req);    
}