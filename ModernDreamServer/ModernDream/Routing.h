#pragma once
#include "crow.h"
#include "DatabaseManager.h"
#include "GameSessionManager.h"

namespace http {
    class Routing {
    public:
        Routing();
        void Run(DatabaseManager& storage);

    private:
        crow::response LoginRoute(const crow::request& req, const std::string& username);
        crow::response RegisterRoute(const crow::request& req);
        crow::response CreateSessionRoute(const crow::request& req);
        crow::response JoinSessionRoute(const crow::request& req);
        crow::response LeaveSessionRoute(const crow::request& req);
        //crow::response GetSessionStatusRoute(const crow::request& req);
        crow::response GetSessionStatusRoute(const std::string& sessionId); 
        crow::response GenerateMapRoute(const crow::request& req);
        crow::response MovePlayerRoute(const crow::request& req);
        crow::response SyncBulletsRoute(const crow::request& req);
        crow::response ShootBulletRoute(const crow::request& req);


    private:
        DatabaseManager m_db;
        GameSessionManager m_gameSessionManager;
        crow::SimpleApp m_app;
    };
}

