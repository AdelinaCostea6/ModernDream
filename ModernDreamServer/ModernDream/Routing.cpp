#include "Routing.h"
#include <vector>
#include <regex>
#include <iostream>
//#include <mutex>

using namespace http;

//std::mutex globalMutex; 

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

    
    CROW_ROUTE(m_app, "/game/status/<string>")
        .methods("GET"_method)([this](std::string sessionId) {
        return GetSessionStatusRoute(sessionId);
            });

    CROW_ROUTE(m_app, "/generateMap")
        .methods("POST"_method)([this](const crow::request& req) {
        return GenerateMapRoute(req); 
            });

    CROW_ROUTE(m_app, "/game/move").methods("POST"_method)([this](const crow::request& req) {
        return MovePlayerRoute(req);
        });

    //CROW_ROUTE(m_app, "/game/positions").methods("GET"_method)([this](const crow::request& req) {
    //    auto json = crow::json::load(req.body);
    //    if (!json) {
    //        return crow::response(400, "Invalid JSON format");
    //    }

    //    std::string sessionId = json["sessionId"].s();
    //    auto session = m_gameSessionManager.GetSession(sessionId);

    //    if (!session) {
    //        return crow::response(404, "Session not found");
    //    }

    //    crow::json::wvalue response;
    //    for (const auto& player : session->players) {
    //        auto pos = player.second->GetPosition();
    //        // Construim array-ul JSON pentru poziție
    //        response["players"][player.first] = crow::json::wvalue::list({ pos.first, pos.second });
    //    }

    //    return crow::response(200, response);
    //    });

 
   /* CROW_ROUTE(m_app, "/game/syncBullets").methods("POST"_method)([this](const crow::request& req) { 
        return SyncBulletsRoute(req);
        });*/
    /*CROW_ROUTE(m_app, "/game/sync").methods("POST"_method)([&](const crow::request& req) {
        std::cout << "[DEBUG] Received SyncBullets request: " << req.body << std::endl;
        return SyncBulletsRoute(req);
        });*/


    /*CROW_ROUTE(m_app, "/game/shoot").methods("POST"_method)([this](const crow::request& req) {
        return ShootBulletRoute(req);
        });*/


    CROW_ROUTE(m_app, "/game/syncBullets").methods("POST"_method)([this](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);

            if (!body || !body.has("sessionId")) {
                return crow::response(400, "Invalid request: Missing sessionId.");
            }

            std::string sessionId = body["sessionId"].s();
            auto session = m_gameSessionManager.GetSession(sessionId);
            if (!session) {
                return crow::response(404, "Session not found.");
            }

            // Actualizează pozițiile gloanțelor înainte de a le trimite către client
            session->game.UpdateBullets();

            // Construiește răspunsul cu pozițiile actualizate ale gloanțelor
            crow::json::wvalue response;
            size_t bulletIndex = 0;
            for (const auto& bullet : session->game.GetBullets()) {
                response["bullets"][bulletIndex]["x"] = bullet.GetPosition().first;
                response["bullets"][bulletIndex]["y"] = bullet.GetPosition().second;
                response["bullets"][bulletIndex]["direction"] = std::string(1, bullet.GetDirection());
                ++bulletIndex;
            }

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            CROW_LOG_ERROR << "Error in /game/syncBullets: " << e.what();
            return crow::response(500, "Error in /game/syncBullets: " + std::string(e.what()));
        }
        });





    CROW_ROUTE(m_app, "/game/shoot").methods("POST"_method)([this](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);

            // Validează datele din cerere
            if (!body || !body.has("sessionId") || !body.has("username") || !body.has("direction")) {
                return crow::response(400, "Invalid request: Missing required fields.");
            }

            std::string sessionId = body["sessionId"].s();
            std::string username = body["username"].s();

            std::string directionString = body["direction"].s();  // Obține direcția ca string
            if (directionString.empty()) {
                return crow::response(400, "Invalid request: Direction is empty.");
            }

            char direction = directionString[0];  // Extrage primul caracter

            //char direction = body["direction"].s()[0];

            // Găsește sesiunea de joc
            auto session = m_gameSessionManager.GetSession(sessionId);
            if (!session) {
                return crow::response(404, "Session not found.");
            }

            // Găsește jucătorul în sesiune
            Player* player = session->GetPlayerByUsername(username);
            if (!player) {
                return crow::response(404, "Player not found.");
            }

            // Creează glonțul pe baza poziției și direcției jucătorului
            int startX = player->GetPosition().first;
            int startY = player->GetPosition().second;

            // Adaugă glonțul la lista jocului
            session->game.ShootBullet(*player);

            // Construiește răspunsul pentru client
            crow::json::wvalue response;
            response["startX"] = startX;
            response["startY"] = startY;
            response["direction"] = std::string(1, direction);  // Convertim char în string

            return crow::response(200, response);

        }
        catch (const std::exception& e) {
            CROW_LOG_ERROR << "Error in /game/shoot: " << e.what();
            return crow::response(500, "Error: " + std::string(e.what()));
        }
        });

    CROW_ROUTE(m_app, "/game/updateWalls").methods("POST"_method)([this](const crow::request& req) {
        try {
            auto body = crow::json::load(req.body);
            if (!body || !body.has("sessionId")) {
                return crow::response(400, "Invalid request: Missing sessionId.");
            }

            std::string sessionId = body["sessionId"].s();
            auto session = m_gameSessionManager.GetSession(sessionId);
            if (!session) {
                return crow::response(404, "Session not found.");
            }

            crow::json::wvalue response;
            response["updatedCells"] = crow::json::wvalue::list();  // Inițializează o listă JSON

            size_t index = 0;
            for (const auto& [x, y] : session->game.GetUpdatedCells()) {
                response["updatedCells"][index]["x"] = x;
                response["updatedCells"][index]["y"] = y;
                ++index;
            }

            session->game.ClearUpdatedCells();  // Curăță lista de actualizări după trimitere

            return crow::response(200, response);
        }
        catch (const std::exception& e) {
            CROW_LOG_ERROR << "Error in /game/updateWalls: " << e.what();
            return crow::response(500, "Error: " + std::string(e.what()));
        }
        });





    /*CROW_ROUTE(m_app, "/game/triggerBomb").methods("POST"_method)([this](const crow::request& req) {
        return TriggerBombRoute(req);
        });*/

    std::thread matchingThread([this]() {
        while (true) {
            m_gameSessionManager.MatchPlayers();
            std::this_thread::sleep_for(std::chrono::seconds(1));  // Check every second
        }
        });
    matchingThread.detach();

    CROW_ROUTE(m_app, "/game/joinQueue").methods("POST"_method)([this](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json.has("username") || !json.has("score")) {
            return crow::response(400, "Invalid request: Missing username or score.");
        }

        std::string username = json["username"].s();
        int score = json["score"].i();

        std::cout << "[INFO] Received joinQueue request. Username: " << username << ", Score: " << score << std::endl;

        m_gameSessionManager.AddToQueue(username, score);
        return crow::response(200, "Player added to queue.");
        });


    CROW_ROUTE(m_app, "/matchmaking/queue").methods("POST"_method)([this](const crow::request& req) {
        auto json = crow::json::load(req.body);

        if (!json.has("username") || !json.has("score")) {
            return crow::response(400, "Invalid request: Missing username or score.");
        }

        std::string username = json["username"].s();
        int score = json["score"].i();

        // Caută o sesiune activă
        std::string sessionId = m_gameSessionManager.FindOrCreateSession(username, score);

        crow::json::wvalue response;
        response["status"] = "success";
        response["sessionId"] = sessionId;

        CROW_LOG_INFO << "Player " << username << " added to session " << sessionId;
        return crow::response(200, response);
        });

    //CROW_ROUTE(m_app, "/matchmaking/status/<string>").methods("GET"_method)([this](const std::string& sessionId) {
    //    auto session = m_gameSessionManager.GetSession(sessionId);
    //    if (!session) {
    //        CROW_LOG_ERROR << "Session not found: " << sessionId;
    //        return crow::response(404, "Session not found.");
    //    }

    //    crow::json::wvalue response;
    //    response["status"] = session->isReady ? "ready" : "waiting";
    //    response["currentPlayers"] = session->players.size();
    //    response["requiredPlayers"] = session->requiredPlayers;

    //    // Adaugă lista jucătorilor
    //    
    //    crow::json::wvalue::list playersList;  // Inițializează explicit ca listă (array)

    //    for (const auto& [username, player] : session->players) {
    //        playersList.push_back(username);  // Adaugă utilizatorii în listă
    //    }

    //    response["players"] = std::move(playersList);  // Atribuie lista array-ului JSON

    //    return crow::response(200, response);
    //    });

    CROW_ROUTE(m_app, "/matchmaking/status/<string>").methods("GET"_method)([this](const std::string& sessionId) {
        auto session = m_gameSessionManager.GetSession(sessionId);
        if (!session) {
            CROW_LOG_ERROR << "Session not found: " << sessionId;
            return crow::response(404, "Session not found.");
        }

        crow::json::wvalue response;
        response["status"] = session->isReady ? "ready" : "waiting";
        response["sessionId"] = sessionId;  // Asigură-te că `sessionId` este inclus
        response["currentPlayers"] = session->players.size();
        response["requiredPlayers"] = session->requiredPlayers;

        // Adaugă lista jucătorilor
        
        crow::json::wvalue::list playersList;  // Inițializează explicit ca listă (array)

        for (const auto& [username, player] : session->players) {
            playersList.push_back(username);  // Adaugă utilizatorii în listă
        }

        response["players"] = std::move(playersList);
        return crow::response(200, response);
        });
    
   /* CROW_ROUTE(m_app, "/get-updates")
        ([this](const crow::request& req) {
        return this->GetUpdatesRoute(req);
            });*/


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
    //return crow::response(200, "Successfully joined game
    // 
  




    //crow::response Routing::JoinSessionRoute(const crow::request & req) {
    //    auto json = crow::json::load(req.body);

    //    if (!json) {
    //        CROW_LOG_ERROR << "Invalid JSON format";
    //        return crow::response(400, "Invalid JSON format");
    //    }
    //    if (!json.has("username")) {
    //        CROW_LOG_ERROR << "Missing 'username'";
    //        return crow::response(400, "Missing 'username'");
    //    }
    //    if (!json.has("mapType")) {
    //        CROW_LOG_ERROR << "Missing 'mapType'";
    //        return crow::response(400, "Missing 'mapType'");
    //    }
    //    if (!json.has("requiredPlayers")) {
    //        CROW_LOG_ERROR << "Missing 'requiredPlayers'";
    //        return crow::response(400, "Missing 'requiredPlayers'");
    //    }

    //    std::string username = json["username"].s();
    //    std::string mapType = json["mapType"].s();
    //    std::string sessionId = json.has("sessionId") ? std::string(json["sessionId"].s()) : "new";
    //    int requiredPlayers = json["requiredPlayers"].i();

    //    /*if (std::string(sessionId) == "new") {
    //        sessionId = m_gameSessionManager.CreateSession(requiredPlayers);
    //    }*/
    //    if (std::string(sessionId) == "new") {
    //        /*for (auto& [existingSessionId, session] : m_gameSessionManager.GetSessions()) {
    //            if (!session->isReady && session->requiredPlayers == requiredPlayers &&
    //                session->GetMapType() == mapType) { 
    //                sessionId = existingSessionId;
    //                break;
    //            }
    //        }*/

    //        if (sessionId == "new") {
    //            sessionId = m_gameSessionManager.CreateSession(requiredPlayers);
    //        }
    //    }


    //    if (!m_gameSessionManager.JoinSession(sessionId, username)) {
    //        return crow::response(400, "Failed to join session");
    //    }

    //    CROW_LOG_INFO << "Join request processed: username=" << username
    //        << ", mapType=" << mapType
    //        << ", sessionId=" << sessionId;

    //    crow::json::wvalue response;
    //    response["message"] = "Player joined successfully";
    //    response["sessionId"] = sessionId;
    //    response["username"] = username;
    //    response["requiredPlayers"] = requiredPlayers;

    //    return crow::response(200, response);
    //}




crow::response Routing::JoinSessionRoute(const crow::request& req) {
    auto json = crow::json::load(req.body);

    if (!json) {
        CROW_LOG_ERROR << "Invalid JSON format";
        return crow::response(400, "Invalid JSON format");
    }

    if (!json.has("username") || !json.has("sessionId")) {
        CROW_LOG_ERROR << "Missing required parameters: username or sessionId";
        return crow::response(400, "Missing required parameters");
    }

    std::string username = json["username"].s();
    std::string sessionId = json["sessionId"].s();

    CROW_LOG_INFO << "Join session called for username: " << username << ", sessionId: " << sessionId;

    if (m_gameSessionManager.JoinSession(sessionId, username)) {
        CROW_LOG_INFO << "Player joined session successfully: " << sessionId;

        crow::json::wvalue response;
        response["message"] = "Player joined successfully";
        response["sessionId"] = sessionId;
        response["username"] = username;

        return crow::response(200, response);
    }

    CROW_LOG_ERROR << "Failed to join session for username: " << username;
    return crow::response(400, "Failed to join session");
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

        //response["mapType"] = session.GetMapType();  // Include map type in the response


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

//crow::response Routing::GenerateMapRoute(const crow::request& req) {
//    crow::json::rvalue data = crow::json::load(req.body);
//    if (!data) {
//        return crow::response(400, "Invalid JSON");
//    }
//
//    int numPlayers = data["numPlayers"].i();
//    MapGenerator mapGen;
//    mapGen.GenerateMap(numPlayers);
//
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> mapJson;
//
//    for (const auto& row : mapGen.GetMapMatrix()) {
//        std::vector<crow::json::wvalue> rowJson;
//        for (int val : row) {
//            rowJson.push_back(crow::json::wvalue(val));  // Convert each element to a wvalue
//        }
//        mapJson.push_back(crow::json::wvalue(rowJson));
//    }
//
//    response["map"] = crow::json::wvalue(mapJson);
//    return crow::response(200, response);
//}


//crow::response Routing::GenerateMapRoute(const crow::request& req) {
//    crow::json::rvalue data = crow::json::load(req.body);
//    if (!data) {
//        return crow::response(400, "Invalid JSON");
//    }
//
//    if (!data.has("numPlayers")) {
//        return crow::response(400, "Missing required keys: 'numPlayers' or 'sessionId'");
//    }
//
//    int numPlayers = data["numPlayers"].i();
//    //std::string sessionId = data["sessionId"].s();
//
//   /* auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }*/
//
//
//    Game game;
//    game.GenerateMap(numPlayers);  // Generăm harta prin clasa `Game`
//
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> mapJson;
//
//    for (const auto& row :game.GetMap().GetMapMatrix()) {
//        std::vector<crow::json::wvalue> rowJson;
//        for (int val : row) {
//            rowJson.push_back(crow::json::wvalue(val));
//        }
//        mapJson.push_back(crow::json::wvalue(rowJson));
//    }
//
//    response["map"] = crow::json::wvalue(mapJson);
//    return crow::response(200, response);
//}


//crow::response Routing::GenerateMapRoute(const crow::request& req) {
//    crow::json::rvalue data = crow::json::load(req.body);
//    if (!data) {
//        return crow::response(400, "Invalid JSON");
//    }
//
//    if (!data.has("numPlayers") || !data.has("sessionId")) {
//        return crow::response(400, "Missing required keys: 'numPlayers' or 'sessionId'");
//    }
//
//    int numPlayers = data["numPlayers"].i();
//    std::string sessionId = data["sessionId"].s();
//
//    auto session = m_gameSessionManager.GetSession(sessionId);  // Obține sesiunea activă
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }
//
//    session->game.GenerateMap(numPlayers);  // Generează harta pentru sesiunea curentă
//
//    // Actualizează pozițiile jucătorilor după generarea hărții
//    auto startPositions = session->game.GetMap().GetPlayerStartPositions();
//    int i = 0;
//    for (auto& player : session->players) {
//        if (i < startPositions.size()) {
//            player.second->SetPosition(startPositions[i]);
//            std::cout << "Jucătorul " << player.first << " este plasat la ("
//                << startPositions[i].first << ", " << startPositions[i].second << ")\n";
//            ++i;
//        }
//    }
//
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> mapJson;
//    for (const auto& row : session->game.GetMap().GetMapMatrix()) {
//        std::vector<crow::json::wvalue> rowJson;
//        for (int val : row) {
//            rowJson.push_back(crow::json::wvalue(val));
//        }
//        mapJson.push_back(crow::json::wvalue(rowJson));
//    }
//
//    response["map"] = crow::json::wvalue(mapJson);
//    std::cout << "Dimensiunea JSON trimisă: " << mapJson.size() << " linii, " << mapJson[0].size() << " coloane\n";
//    //std::cout << "Dimensiunea răspunsului JSON în bytes: " << crow::json::dump(response).size() << "\n";
//    return crow::response(200, response);
//}

crow::response Routing::GenerateMapRoute(const crow::request& req) {
    crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        return crow::response(400, "Invalid JSON");
    }

    if (!data.has("sessionId")) {
        return crow::response(400, "Missing required key: 'sessionId'");
    }

    std::string sessionId = data["sessionId"].s();

    // Retrieve the active session
    auto session = m_gameSessionManager.GetSession(sessionId);
    if (!session) {
        return crow::response(404, "Session not found");
    }

    // Retrieve the pre-generated map from the session's game object
    const Map& map = session->GetGame().GetMap();

    // Generate the JSON response for the map
    crow::json::wvalue response;
    std::vector<crow::json::wvalue> mapJson;
    for (const auto& row : map.GetMapMatrix()) {
        std::vector<crow::json::wvalue> rowJson;
        for (int val : row) {
            rowJson.push_back(crow::json::wvalue(val));
        }
        mapJson.push_back(crow::json::wvalue(rowJson));
    }

    response["map"] = crow::json::wvalue(mapJson);

    // Update player positions using start positions
    auto startPositions = map.GetPlayerStartPositions();
    int i = 0;
    for (auto& player : session->players) {
        if (i < startPositions.size()) {
            player.second->SetPosition(startPositions[i]);
            std::cout << "Player " << player.first << " placed at ("
                << startPositions[i].first << ", " << startPositions[i].second << ")\n";
            ++i;
        }
    }

    std::cout << "JSON response size: " << mapJson.size() << " rows, "
        << (mapJson.empty() ? 0 : mapJson[0].size()) << " columns\n";

    return crow::response(200, response);
}



crow::response Routing::MovePlayerRoute(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON format");
    }

    std::string sessionId = json["sessionId"].s();
    std::string username = json["username"].s();
    std::string direction = json["direction"].s();  // "w", "a", "s", "d"

    auto session = m_gameSessionManager.GetSession(sessionId);
    if (!session) {
        return crow::response(404, "Session not found");
    }

    auto playerIt = session->players.find(username);
    if (playerIt == session->players.end()) {
        return crow::response(404, "Player not found");
    }

    auto& player = playerIt->second;
    player->Movement(session->GetMap(), direction[0]);


    // Creează un `crow::json::wvalue::list`
    crow::json::wvalue response;
    response["position"] = crow::json::wvalue::list({ player->GetPosition().first, player->GetPosition().second });

    response["message"] = "Player moved successfully";

    return crow::response(200, response);
}


//crow::response Routing::ShootBulletRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json) {
//        return crow::response(400, "Invalid JSON format");
//    }
//    std::string sessionId = json["sessionId"].s();
//    std::string username = json["username"].s();
//    std::string directionStr = json["direction"].s();  // Obține direcția ca string
//    if (directionStr.empty()) {
//        return crow::response(400, "Direction is missing or invalid");
//    }
//    char direction = directionStr[0];  // Extrage primul caracter din string
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }
//    auto player = session->GetPlayerByUsername(username);
//    if (!player) {
//        return crow::response(404, "Player not found");
//    }
//    player->SetDirection(direction);
//    session->game.ShootBullet(*player);  // Creează bullet
//    crow::json::wvalue response;
//    response["message"] = "Bullet shot successfully!";
//    return crow::response(200, response);
//}


//crow::response Routing::ShootBulletRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json) {
//        return crow::response(400, "Invalid JSON format");
//    }
//
//    std::string sessionId = json["sessionId"].s();
//    std::string username = json["username"].s();
//    std::string directionStr = json["direction"].s();  // Obține direcția ca string
//    if (directionStr.empty()) {
//        return crow::response(400, "Direction is missing or invalid");
//    }
//    char direction = directionStr[0];  // Extrage primul caracter din string
//
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }
//
//    auto player = session->GetPlayerByUsername(username);
//    if (!player) {
//        return crow::response(404, "Player not found");
//    }
//
//    player->SetDirection(direction);  // Setează direcția jucătorului
//    session->game.ShootBullet(*player);  // Creează bullet
//
//    // Obține lista de gloanțe active din sesiune
//    const auto& bullets = session->game.GetBullets();
//
//    // Construiește răspunsul JSON
//    crow::json::wvalue response;
//    response["message"] = "Bullet shot successfully!";
//
//    std::vector<crow::json::wvalue> bulletsJson;
//    for (const auto& bullet : bullets) {
//        if (bullet.GetIsActive()) {  // Adaugă doar gloanțele active
//            crow::json::wvalue bulletJson;
//            bulletJson["x"] = bullet.GetPosition().first;  // Coordonata X
//            bulletJson["y"] = bullet.GetPosition().second;  // Coordonata Y
//            bulletsJson.emplace_back(std::move(bulletJson));
//        }
//    }
//
//    response["bullets"] = std::move(bulletsJson);  // Adaugă lista de gloanțe în răspuns
//
//    return crow::response(200, response);  // Trimite răspunsul complet
//}


//
//crow::response Routing::SyncBulletsRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json) {
//        return crow::response(400, "Invalid JSON format");
//    }
//
//    std::string sessionId = json["sessionId"].s();
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }
//    //session->game.UpdateBullets();
//    const auto& bullets = session->game.GetBullets();  // Retrieve bullets from the game session
//
//    // Prepare JSON response
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> bulletsJson;
//
//    for (const auto& bullet : bullets) {
//        if (bullet.GetIsActive()) {  // Include only active bullets
//            crow::json::wvalue bulletJson;
//            bulletJson["x"] = bullet.GetPosition().first;
//            bulletJson["y"] = bullet.GetPosition().second;
//            bulletJson["direction"] = std::string(1, bullet.GetDirection());
//            bulletsJson.emplace_back(std::move(bulletJson));
//        }
//    }
//
//    response["bullets"] = std::move(bulletsJson);
//    return crow::response(200, response);
//}



//crow::response Routing::ShootBulletRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json) {
//        return crow::response(400, "Invalid JSON format");
//    }
//
//    std::string sessionId = json["sessionId"].s();
//    std::string username = json["username"].s();
//    std::string directionStr = json["direction"].s();
//
//    if (directionStr.empty()) {
//        return crow::response(400, "Direction is missing or invalid");
//    }
//
//    char direction = directionStr[0];
//
//    // Retrieve session and player
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        std::cerr << "Session not found: " << sessionId << std::endl;
//        return crow::response(404, "Session not found");
//    }
//
//    auto player = session->GetPlayerByUsername(username);
//    if (!player) {
//        std::cerr << "[ERROR] ShootBulletRoute: Player not found for username=" << username << std::endl;
//        return crow::response(404, "Player not found");
//    }
//
//    try {
//        player->SetDirection(direction);
//        session->game.ShootBullet(*player);
//    }
//    catch (const std::exception& e) {
//        std::cerr << "[ERROR] Exception while shooting bullet: " << e.what() << std::endl;
//        return crow::response(500, "Internal Server Error");
//    }
//
//
//    // Update bullet positions and collisions
//    session->game.UpdateBullets();
//
//    // Prepare response with updated bullet positions
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> bulletsJson;
//
//    for (const auto& bullet : session->game.GetBullets()) {
//        if (bullet.GetIsActive()) {
//            crow::json::wvalue bulletJson;
//            bulletJson["x"] = bullet.GetPosition().first;
//            bulletJson["y"] = bullet.GetPosition().second;
//            bulletJson["direction"] = std::string(1, bullet.GetDirection());
//            bulletsJson.emplace_back(std::move(bulletJson));
//        }
//    }
//
//    response["bullets"] = std::move(bulletsJson);
//    return crow::response(200, response);
//}
//


//crow::response Routing::ShootBulletRoute(const crow::request& req) {
//    try {
//        // Parse JSON request body
//        auto json = crow::json::load(req.body);
//        if (!json) {
//            return crow::response(400, "Invalid JSON format");
//        }
//
//        // Extract session ID, username, and direction from the JSON
//        std::string sessionId = json["sessionId"].s();
//        std::string username = json["username"].s();
//        std::string directionStr = json["direction"].s();
//
//        if (directionStr.empty() || directionStr.size() != 1) {
//            return crow::response(400, "Direction is missing or invalid");
//        }
//
//        char direction = directionStr[0]; // Get the direction as a single character
//
//        // Retrieve the session using the session ID
//        auto session = m_gameSessionManager.GetSession(sessionId);
//        if (!session) {
//            return crow::response(404, "Session not found");
//        }
//
//        // Retrieve the player using the username
//        auto player = session->GetPlayerByUsername(username);
//        if (!player) {
//            return crow::response(404, "Player not found");
//        }
//
//        // Set the player's direction
//        player->SetDirection(direction);
//
//        // Create and shoot the bullet
//        session->game.ShootBullet(*player);
//
//        // Build the success response
//        crow::json::wvalue response;
//        response["message"] = "Bullet shot successfully!";
//        response["player"] = username;
//        response["direction"] = std::string(1, direction);
//        auto position = player->GetPosition(); // Assuming GetPosition() exists
//        response["position"]["x"] = position.first;
//        response["position"]["y"] = position.second;
//
//        return crow::response(200, response);
//    }
//    catch (const std::exception& e) {
//        // Handle any unexpected exceptions
//        std::cerr << "Error in ShootBulletRoute: " << e.what() << std::endl;
//        return crow::response(500, "Internal Server Error");
//    }
//    catch (...) {
//        // Catch-all for non-standard exceptions
//        std::cerr << "Unknown error in ShootBulletRoute" << std::endl;
//        return crow::response(500, "Internal Server Error");
//    }
//}
crow::response Routing::ShootBulletRoute(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON format");
    }

    std::string sessionId = json["sessionId"].s();
    std::string username = json["username"].s();
    std::string directionString = json["direction"].s(); // Extract direction as string
    char direction = directionString.empty() ? '\0' : directionString[0]; // Safely get the first character

    auto session = m_gameSessionManager.GetSession(sessionId);
    if (!session) {
        return crow::response(404, "Session not found");
    }

    auto player = session->GetPlayerByUsername(username);
    if (!player) {
        return crow::response(404, "Player not found");
    }

    player->SetDirection(direction);
    session->game.ShootBullet(*player);

    return crow::response(200, "Bullet shot successfully");
}

//crow::response Routing::SyncBulletsRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json) {
//        return crow::response(400, "Invalid JSON format");
//    }
//
//    std::string sessionId = json["sessionId"].s();
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found");
//    }
//
//    session->game.UpdateBullets();
//    const auto& bullets = session->game.GetBullets();
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> bulletsJson;
//    for (const auto& bullet : bullets) {
//        if (bullet.GetIsActive()) {
//            crow::json::wvalue bulletJson;
//            bulletJson["x"] = bullet.GetPosition().first;
//            bulletJson["y"] = bullet.GetPosition().second;
//            bulletsJson.push_back(bulletJson);
//        }
//    }
//    response["bullets"] = std::move(bulletsJson);
//    return crow::response(200, response);
//}
crow::response Routing::SyncBulletsRoute(const crow::request& req) {
    auto json = crow::json::load(req.body);
    if (!json) {
        return crow::response(400, "Invalid JSON format");
    }

    std::string sessionId = json["sessionId"].s();
    auto session = m_gameSessionManager.GetSession(sessionId);
    if (!session) {
        return crow::response(404, "Session not found");
    }

    session->game.UpdateBullets();
    const auto& bullets = session->game.GetBullets();

    crow::json::wvalue response;
    std::vector<crow::json::wvalue> bulletsJson;  // Use a vector to create the JSON array
    for (const auto& bullet : bullets) {
        if (bullet.GetIsActive()) {
            crow::json::wvalue bulletJson;
            bulletJson["x"] = bullet.GetPosition().first;
            bulletJson["y"] = bullet.GetPosition().second;
            bulletsJson.push_back(std::move(bulletJson));
        }
    }

    response["bullets"] = std::move(bulletsJson);// Move the vector into the JSON response
    std::cout << "Response JSON: " << response.dump() << std::endl;
    
    return crow::response(200, response);
}


//crow::response Routing::TriggerBombRoute(const crow::request& req) {
//    auto json = crow::json::load(req.body);
//    if (!json || !json.has("sessionId") || !json.has("bombX") || !json.has("bombY")) {
//        return crow::response(400, "Invalid request: Missing required fields.");
//    }
//
//    // Extract session ID and bomb coordinates from the request
//    std::string sessionId = json["sessionId"].s();
//    int bombX = json["bombX"].i();
//    int bombY = json["bombY"].i();
//
//    // Retrieve the game session
//    auto session = m_gameSessionManager.GetSession(sessionId);
//    if (!session) {
//        return crow::response(404, "Session not found.");
//    }
//
//    // Trigger the bomb in the game logic
//    session->game.TriggerBomb(bombX, bombY);
//
//    crow::json::wvalue response;
//
//    // Declare updatedCells as a list (JSON array)
//    response["updatedCells"] = crow::json::wvalue::list();
//
//    size_t index = 0;
//    for (const auto& [x, y] : session->game.GetUpdatedCells()) {
//        response["updatedCells"][index]["x"] = x;
//        response["updatedCells"][index]["y"] = y;
//        index++;
//    }
//
//    return crow::response(200, response);
//
//
//
//
//}

//crow::response Routing::GetUpdatesRoute(const crow::request& req) {
//    crow::json::rvalue data = crow::json::load(req.body);
//    if (!data || !data.has("sessionId")) {
//        return crow::response(400, "Invalid JSON or missing sessionId");
//    }
//
//    std::string sessionId = data["sessionId"].s();
//    auto updates = m_gameSessionManager.GetUpdatedCellsForSession(sessionId);
//
//    crow::json::wvalue response;
//    std::vector<crow::json::wvalue> updatesJson;
//
//    // Populate updatesJson
//    for (const auto& cell : updates) {
//        crow::json::wvalue cellJson;
//        cellJson["x"] = cell.first;
//        cellJson["y"] = cell.second;
//        updatesJson.push_back(std::move(cellJson));  // Use std::move for efficiency
//    }
//
//    // Assign updatesJson to response["updates"] using std::move
//    response["updates"] = crow::json::wvalue(std::move(updatesJson));
//
//    // Clear updates after sending
//    m_gameSessionManager.ClearUpdatedCellsForSession(sessionId);
//
//    return crow::response(200, response);
//}
