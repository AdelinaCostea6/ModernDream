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
	
	/*CROW_ROUTE(m_app, "/cors")
		.methods(crow::HTTPMethod::OPTIONS)
		([](const crow::request& req) {
		crow::response res;
		res.set_header("Access-Control-Allow-Origin", "*");
		res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
		res.set_header("Access-Control-Allow-Headers", "Content-Type");
		return res;
			});

	CROW_ROUTE(m_app, "/login/<string>")([this](const crow::request& req, std::string username) {
		return LoginRoute(req, username);
		});

	CROW_ROUTE(m_app, "/register")
		.methods("POST"_method)([this](const crow::request& req) {
		return RegisterRoute(req);
			});

	m_app.port(8080).multithreaded().run();
	*/
	CROW_ROUTE(m_app, "/cors")
		.methods(crow::HTTPMethod::OPTIONS)
		([](const crow::request& req) {
		crow::response res;
		res.set_header("Access-Control-Allow-Origin", "*");  // Permite accesul din orice sursă
		res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  // Permite GET, POST și OPTIONS
		res.set_header("Access-Control-Allow-Headers", "Content-Type");  // Permite Content-Type
		return res;
			});

	// Adăugarea CORS pe ruta de login
	CROW_ROUTE(m_app, "/login/<string>")
		.methods("GET"_method, "OPTIONS"_method)
		([this](const crow::request& req, std::string username) {
		// Verificăm cererea OPTIONS înainte de a procesa GET
		if (req.method == crow::HTTPMethod::OPTIONS) {
			crow::response res;
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
			res.set_header("Access-Control-Allow-Headers", "Content-Type");
			return res;
		}
		return LoginRoute(req, username);
			});

	// Adăugarea CORS pe ruta de înregistrare
	CROW_ROUTE(m_app, "/register")
		.methods("POST"_method, "OPTIONS"_method)
		([this](const crow::request& req) {
		// Verificăm cererea OPTIONS înainte de a procesa POST
		if (req.method == crow::HTTPMethod::OPTIONS) {
			crow::response res;
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
			res.set_header("Access-Control-Allow-Headers", "Content-Type");
			return res;
		}
		return RegisterRoute(req);
			});

	m_app.port(8080).multithreaded().run();
}

crow::response http::Routing::LoginRoute(const crow::request& req, const std::string& username)
{
	try {
		auto user = m_db.GetUser(username);
		crow::json::wvalue response;
		response["username"] = user->username;
		response["score"] = user->score;
		return crow::response(response);
	}
	catch (const std::system_error)
	{
		return crow::response(404, "User not found\n");
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
