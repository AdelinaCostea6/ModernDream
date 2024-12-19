#include "Routing.h"

#include "DatabaseManager.h"
#include <vector>
#include <regex>

using namespace http;

http::Routing::Routing()
	:m_db(DatabaseManager())
{
}

void http::Routing::Run(DatabaseManager& storage)
{

	CROW_ROUTE(m_app, "/login/<string>")([this](const crow::request& req, std::string username) {
		return LoginRoute(req, username);
		});

	CROW_ROUTE(m_app, "/register")
		.methods("POST"_method)([this](const crow::request& req) {
		return RegisterRoute(req);
			});

	m_app.port(8080).multithreaded().run();
	
}

crow::response http::Routing::LoginRoute(const crow::request& req, const std::string& username)
{
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

crow::response http::Routing::RegisterRoute(const crow::request& req)
{
	
	crow::json::rvalue data = crow::json::load(req.body);
	if (!data)
	{
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
