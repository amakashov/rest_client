#include "handler.h"

handler::handler()
{
    //ctor
}
handler::handler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));

}
handler::~handler()
{
    //dtor
}

void handler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request 
//
void handler::handle_get(http_request message)
{
    // ucout << "GET Request: "<< message.to_string() << endl;

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    for (auto& path : paths)
    {
        ucout << "In GET: " << path << endl;
        try
        {
            handler::GetAction code = handler::getMapper.at(path);
            switch (code)
            {
                case handler::GetAction::Pose:
                {
                    utility::stringstream_t ss;
                    ss <<U("{\"angles\": [")  
                        << 0 <<","
                        << 1 <<","
                        << 2 <<","
                        << 3 <<","
                        << 4 <<","
                        << 5 
                        << U("]}");
                    web::json::value pose = web::json::value::parse(ss);
                    message.reply(status_codes::OK, pose);
                    break;
                }
                case handler::GetAction::Position:
                {
                    utility::stringstream_t ss;
                    ss <<U("{\"point\":{")  
                        << "\"x\":"<< 0 <<","
                        << "\"y\":"<< 1 <<","
                        << "\"z\":"<< 2 <<"},"
                        <<"\"rotation\":{"
                        << "\"roll\":"<< 3 <<","
                        << "\"pitch\":"<< 4 <<","
                        << "\"yaw\":"<< 5 
                        << U("}}");
                    cout << ss.str() << endl;
                    web::json::value position = web::json::value::parse(ss);
                    message.reply(status_codes::OK, position);
                    break;
                }
                default:
                    cout << "Unknown type of action : " << path << endl;
                    message.reply(status_codes::NotFound, U("No such action available"));
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
                    message.reply(status_codes::NotFound, U("No such action available"));
        }
    }

    


    // message.relative_uri().path();

    // concurrency::streams::fstream::open_istream(U("static/index.html"), std::ios::in).then([=](concurrency::streams::istream is)
    // {
    //     message.reply(status_codes::OK, is,  U("text/html"))
	// 	.then([](pplx::task<void> t)
	// 	{
	// 		try{
	// 			t.get();
	// 		}
	// 		catch(...){
	// 			//
	// 		}
	// });
    // }).then([=](pplx::task<void>t)
	// {
	// 	try{
	// 		t.get();
	// 	}
	// 	catch(...){
	// 		message.reply(status_codes::InternalError,U("INTERNAL ERROR "));
	// 	}
	// });

    return;

};

//
// A POST request
//
void handler::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;
    message.reply(status_codes::OK,message.to_string());
    return ;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
    ucout <<  message.to_string() << endl;

    string rep = U("WRITE YOUR OWN DELETE OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request 
//
void handler::handle_put(http_request message)
{
    ucout <<  message.to_string() << endl;
    auto val = message.extract_json().then([](json::value value)
    {
        value.serialize(cout);
    });
    val.wait();

    
    string rep = U("WRITE YOUR OWN PUT OPERATION");
    message.reply(status_codes::OK,rep);
    return;
};

map<string, handler::GetAction> const handler::getMapper =
 {
    {"pose", handler::GetAction::Pose},
    {"position",handler::GetAction::Position}
}; 