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

 
    /*CROW_ROUTE(m_app, "/game/syncBullets").methods("POST"_method)([this](const crow::request& req) {
        return SyncBulletsRoute(req);
        });*/

    CROW_ROUTE(m_app, "/game/shoot").methods("POST"_method)([this](const crow::request& req) {
        return ShootBulletRoute(req);
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


crow::response Routing::GenerateMapRoute(const crow::request& req) {
    crow::json::rvalue data = crow::json::load(req.body);
    if (!data) {
        return crow::response(400, "Invalid JSON");
    }

    if (!data.has("numPlayers") || !data.has("sessionId")) {
        return crow::response(400, "Missing required keys: 'numPlayers' or 'sessionId'");
    }

    int numPlayers = data["numPlayers"].i();
    std::string sessionId = data["sessionId"].s();

    auto session = m_gameSessionManager.GetSession(sessionId);  // Obține sesiunea activă
    if (!session) {
        return crow::response(404, "Session not found");
    }

    session->game.GenerateMap(numPlayers);  // Generează harta pentru sesiunea curentă

    // Actualizează pozițiile jucătorilor după generarea hărții
    auto startPositions = session->game.GetMap().GetPlayerStartPositions();
    int i = 0;
    for (auto& player : session->players) {
        if (i < startPositions.size()) {
            player.second->SetPosition(startPositions[i]);
            std::cout << "Jucătorul " << player.first << " este plasat la ("
                << startPositions[i].first << ", " << startPositions[i].second << ")\n";
            ++i;
        }
    }

    crow::json::wvalue response;
    std::vector<crow::json::wvalue> mapJson;
    for (const auto& row : session->game.GetMap().GetMapMatrix()) {
        std::vector<crow::json::wvalue> rowJson;
        for (int val : row) {
            rowJson.push_back(crow::json::wvalue(val));
        }
        mapJson.push_back(crow::json::wvalue(rowJson));
    }

    response["map"] = crow::json::wvalue(mapJson);
    std::cout << "Dimensiunea JSON trimisă: " << mapJson.size() << " linii, " << mapJson[0].size() << " coloane\n";
    //std::cout << "Dimensiunea răspunsului JSON în bytes: " << crow::json::dump(response).size() << "\n";
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
//


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


crow::response Routing::ShootBulletRoute(const crow::request& req) {
    try {
        // Parse JSON request body
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400, "Invalid JSON format");
        }

        // Extract session ID, username, and direction from the JSON
        std::string sessionId = json["sessionId"].s();
        std::string username = json["username"].s();
        std::string directionStr = json["direction"].s();

        if (directionStr.empty() || directionStr.size() != 1) {
            return crow::response(400, "Direction is missing or invalid");
        }

        char direction = directionStr[0]; // Get the direction as a single character

        // Retrieve the session using the session ID
        auto session = m_gameSessionManager.GetSession(sessionId);
        if (!session) {
            return crow::response(404, "Session not found");
        }

        // Retrieve the player using the username
        auto player = session->GetPlayerByUsername(username);
        if (!player) {
            return crow::response(404, "Player not found");
        }

        // Set the player's direction
        player->SetDirection(direction);

        // Create and shoot the bullet
        session->game.ShootBullet(*player);

        // Build the success response
        crow::json::wvalue response;
        response["message"] = "Bullet shot successfully!";
        response["player"] = username;
        response["direction"] = std::string(1, direction);
        auto position = player->GetPosition(); // Assuming GetPosition() exists
        response["position"]["x"] = position.first;
        response["position"]["y"] = position.second;

        return crow::response(200, response);
    }
    catch (const std::exception& e) {
        // Handle any unexpected exceptions
        std::cerr << "Error in ShootBulletRoute: " << e.what() << std::endl;
        return crow::response(500, "Internal Server Error");
    }
    catch (...) {
        // Catch-all for non-standard exceptions
        std::cerr << "Unknown error in ShootBulletRoute" << std::endl;
        return crow::response(500, "Internal Server Error");
    }
}
