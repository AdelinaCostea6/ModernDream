#include "Routing.h"
#include <vector>
#include <regex>
#include <iostream>

using namespace http;

Routing::Routing()
    : m_db(DatabaseManager()) {}

void Routing::Run(DatabaseManager& storage) {
    // Login route
    CROW_ROUTE(m_app, "/login/<string>")
        ([this](const crow::request& req, std::string username) {
        return LoginRoute(req, username);
            });

    // Register route
    CROW_ROUTE(m_app, "/register")
        .methods("POST"_method)([this](const crow::request& req) {
        return RegisterRoute(req);
            });

    // Game session routes
    CROW_ROUTE(m_app, "/game/create").methods("POST"_method)([this](const crow::request& req) {
        return CreateSessionRoute(req);
        });

    CROW_ROUTE(m_app, "/game/join").methods("POST"_method)([this](const crow::request& req) {
        return JoinSessionRoute(req);
        });

    CROW_ROUTE(m_app, "/game/leave").methods("POST"_method)([this](const crow::request& req) {
        return LeaveSessionRoute(req);
        });

    /*CROW_ROUTE(m_app, "/game/status").methods("POST"_method)([this](const crow::request& req) {
        return GetSessionStatusRoute(req);
        });*/
    CROW_ROUTE(m_app, "/game/status/<string>")
        .methods("GET"_method)([this](std::string sessionId) {
        return GetSessionStatusRoute(sessionId);
            });

    m_app.port(8080).multithreaded().run();
}

crow::response Routing::LoginRoute(const crow::request& req, const std::string& username) {
    try {
        auto user = m_db.GetUser(username);

        if (!user) {
            crow::json::wvalue errorResponse;
            errorResponse["error"] = "User not found";
            errorResponse["username"] = username;
            return crow::response(404, errorResponse);
        }

        crow::json::wvalue response;
        response["username"] = user->username;
        response["score"] = user->score;
        return crow::response(response);
    }
    catch (const std::system_error& e) {
        crow::json::wvalue errorResponse;
        errorResponse["error"] = "Internal Server Error";
        errorResponse["details"] = e.what();
        return crow::response(500, errorResponse);
    }
}

crow::response Routing::RegisterRoute(const crow::request& req) {
    crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        std::cout << "Invalid JSON received.\n";
        return crow::response(400, "Invalid JSON\n");
    }

    std::string username = data["username"].s();
    std::cout << "Attempting to register username: " << username << std::endl;

    auto existingUser = m_db.GetUser(username);
    if (existingUser) {
        std::cout << "Username already exists: " << username << std::endl;
        return crow::response(409, "Username Already Exists\n");
    }

    DataUser newUser{ username };
    m_db.AddUser(newUser);
    std::cout << "User registered successfully: " << username << std::endl;
    return crow::response(200, "User registered successfully\n");
}

crow::response Routing::CreateSessionRoute(const crow::request& req) {
    crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        return crow::response(400, "Invalid JSON");
    }

    int requiredPlayers = data["requiredPlayers"].i();
    std::string sessionId = m_gameSessionManager.CreateSession(requiredPlayers);

    crow::json::wvalue response;
    response["sessionId"] = sessionId;
    response["requiredPlayers"] = requiredPlayers;

    return crow::response(200, response);
}

//crow::response Routing::JoinSessionRoute(const crow::request& req) {
   /* crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        return crow::response(400, "Invalid JSON");
    }

    std::string sessionId = data["sessionId"].s();
    std::string username = data["username"].s();

    if (m_gameSessionManager.JoinSession(sessionId, username)) {
        crow::json::wvalue response;
        response["message"] = "Player joined successfully";
        response["sessionId"] = sessionId;
        response["username"] = username;
        return crow::response(200, response);
    }

    return crow::response(400, "Failed to join session");*/
    //auto json = crow::json::load(req.body);

    //if (!json) {
    //    CROW_LOG_ERROR << "Invalid JSON format";
    //    return crow::response(400, "Invalid JSON format");
    //}
    //if (!json.has("username")) {
    //    CROW_LOG_ERROR << "Missing 'username'";
    //    return crow::response(400, "Missing 'username'");
    //}
    //if (!json.has("mapType")) {
    //    CROW_LOG_ERROR << "Missing 'mapType'";
    //    return crow::response(400, "Missing 'mapType'");
    //}
    //if (!json.has("requiredPlayers")) {
    //    CROW_LOG_ERROR << "Missing 'requiredPlayers'";
    //    return crow::response(400, "Missing 'requiredPlayers'");
    //}

    //std::string username = json["username"].s();
    //std::string mapType = json["mapType"].s();
    //std::string sessionId = json["sessionId"].s();
    //int requiredPlayers = json["requiredPlayers"].i();

    //CROW_LOG_INFO << "Join request received: username=" << username
    //    << ", mapType=" << mapType
    //    << ", requiredPlayers=" << requiredPlayers
    //    << " si id: " << sessionId;

    //// Logică pentru sesiune...
    //return crow::response(200, "Successfully joined game");
    crow::response Routing::JoinSessionRoute(const crow::request & req) {
        auto json = crow::json::load(req.body);

        if (!json) {
            CROW_LOG_ERROR << "Invalid JSON format";
            return crow::response(400, "Invalid JSON format");
        }
        if (!json.has("username")) {
            CROW_LOG_ERROR << "Missing 'username'";
            return crow::response(400, "Missing 'username'");
        }
        if (!json.has("mapType")) {
            CROW_LOG_ERROR << "Missing 'mapType'";
            return crow::response(400, "Missing 'mapType'");
        }
        if (!json.has("requiredPlayers")) {
            CROW_LOG_ERROR << "Missing 'requiredPlayers'";
            return crow::response(400, "Missing 'requiredPlayers'");
        }

        std::string username = json["username"].s();
        std::string mapType = json["mapType"].s();
        std::string sessionId = json.has("sessionId") ? std::string(json["sessionId"].s()) : "new";
        int requiredPlayers = json["requiredPlayers"].i();

        if (std::string(sessionId) == "new") {
            sessionId = m_gameSessionManager.CreateSession(requiredPlayers);
        }

        if (!m_gameSessionManager.JoinSession(sessionId, username)) {
            return crow::response(400, "Failed to join session");
        }

        CROW_LOG_INFO << "Join request processed: username=" << username
            << ", mapType=" << mapType
            << ", sessionId=" << sessionId;

        crow::json::wvalue response;
        response["message"] = "Player joined successfully";
        response["sessionId"] = sessionId;
        response["username"] = username;
        response["requiredPlayers"] = requiredPlayers;

        return crow::response(200, response);
    }



crow::response Routing::LeaveSessionRoute(const crow::request& req) {
    crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        return crow::response(400, "Invalid JSON");
    }

    std::string sessionId = data["sessionId"].s();
    std::string username = data["username"].s();

    m_gameSessionManager.LeaveSession(sessionId, username);

    crow::json::wvalue response;
    response["message"] = "Player left successfully";
    response["sessionId"] = sessionId;
    response["username"] = username;

    return crow::response(200, response);
}

//crow::response Routing::GetSessionStatusRoute(const crow::request& req) {
//    crow::json::rvalue data = crow::json::load(req.body);
//    if (!data) {
//        return crow::response(400, "Invalid JSON");
//    }
//
//    std::string sessionId = data["sessionId"].s();
//
//    try {
//        auto session = m_gameSessionManager.GetSessionStatus(sessionId);
//
//        crow::json::wvalue response;
//        response["sessionId"] = session.sessionId;
//        response["requiredPlayers"] = session.requiredPlayers;
//        response["currentPlayers"] = session.players.size();
//        response["isReady"] = session.isReady;
//
//        response["status"] = session.isReady ? "ready" : "waiting";
//
//        /*crow::json::wvalue playersJson;
//        for (const auto& [username, player] : session.players) {
//            playersJson[username] = player->GetName();
//        }
//        response["players"] = std::move(playersJson);*/
//        // Create player list
//        std::vector<std::string> playerList;
//        for (const auto& [username, player] : session.players) {
//            playerList.push_back(username);
//        }
//        response["players"] = std::move(playerList);
//
//        // Include last joined/left players if available
//        if (session.lastJoinedPlayer != "") { 
//            response["lastJoined"] = session.lastJoinedPlayer;
//        }
//        if (session.lastLeftPlayer != "") {
//            response["lastLeft"] = session.lastLeftPlayer;
//        }
//
//        return crow::response(200, response);
//    }
//    catch (const std::out_of_range&) {
//        return crow::response(404, "Session not found");
//    }
//}
crow::response Routing::GetSessionStatusRoute(const std::string& sessionId) {
    try {
        //auto session = m_gameSessionManager.GetSessionStatus(sessionId);
        const auto& session = m_gameSessionManager.GetSessionStatus(sessionId);

        crow::json::wvalue response;
        response["sessionId"] = session.sessionId;
        response["requiredPlayers"] = session.requiredPlayers;
        response["currentPlayers"] = session.players.size();

        // Set status based on session state
        response["status"] = session.isReady ? "ready" : "waiting";

        // Create player list
        std::vector<std::string> playerList;
        for (const auto& [username, player] : session.players) {
            playerList.push_back(username);
        }
        response["players"] = std::move(playerList);

        // Include last joined/left players if available
        if (session.lastJoinedPlayer != "") {
            response["lastJoined"] = session.lastJoinedPlayer;
        }
        if (session.lastLeftPlayer != "") {
            response["lastLeft"] = session.lastLeftPlayer;
        }

        return crow::response(200, response);
    }
    catch (const std::out_of_range&) {
        return crow::response(404, "Session not found");
    } 
}