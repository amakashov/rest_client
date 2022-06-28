#include <iostream>
#include <QDebug>
#include <thread>

#include "rozumexchange.hpp"

int main() 
{
    // RozumExchange robot("127.0.0.1", 34568);
    RozumExchange robot("10.10.10.20", 8081);
    auto result = robot.getPosition();
    qDebug() <<"Got result"<< result.first << ":"<< result.second;
    result = robot.getPose();
    qDebug() <<"Got result"<< result.first << ":"<< result.second;

    QJsonObject params, pose;
    QJsonObject tmp;
    tmp["x"] = 0.107340525235374602;
    tmp["y"] = 0.7252472702635938;
    tmp["z"] = 0.4109358030039697;
    pose["point"] = tmp;
    tmp = QJsonObject();
    tmp["yaw"] = -2.888051720431865;
    tmp["roll"] = -1.6550353733315393;
    tmp["pitch"] = 0.34828556144141376;
    pose["rotation"] = tmp;

    params["motionType"] = "linear";
    params["speed"] = 1;

    result=robot.freeze();
    qDebug() <<"Got result"<< result.first << ":"<< result.second;
    result=robot.relax();
    qDebug() <<"Got result"<< result.first << ":"<< result.second;
    result = robot.setPosition(pose,params);
    qDebug() <<"Got result"<< result.first << ":"<< result.second;
    for (int i=0; i<5; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto result = robot.getPosition();
        qDebug() <<"Got result"<< result.first << ":"<< result.second;
    }
	return 0;
}