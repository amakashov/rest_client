#pragma (once)

#include <cpprest/http_client.h>
#include <QJsonObject>

typedef std::pair<web::http::status_code, QJsonObject> rozumResponse;

class RozumExchange
{
public:
    RozumExchange(std::string ip, int port);
    ~RozumExchange();
    //  GET requests
    rozumResponse getPose();
    rozumResponse getPosition();
    //  
    rozumResponse freeze();
    rozumResponse relax();
    rozumResponse setPosition(QJsonObject position, QJsonObject params);

private:
    rozumResponse getRequestJSON(web::http::http_request request);
    rozumResponse getRequestString(web::http::http_request request);
    web::http::client::http_client m_client;
};