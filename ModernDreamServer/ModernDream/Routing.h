#pragma once
#include <crow.h>
#include "DatabaseManager.h"

namespace http
{
	class Routing
	{
	public:
		Routing();
		void Run(DatabaseManager& storage);
	private:
		crow::response LoginRoute(const crow::request& req, const std::string& username);
		crow::response RegisterRoute(const crow::request& req);

	private:
		DatabaseManager m_db;
		//crow::SimpleApp m_app;
		crow::SimpleApp m_app;
	};
}

