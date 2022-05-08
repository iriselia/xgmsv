cmaid_project(
	CONFIGURATION CONSOLE
	DEFINE
	INCLUDE asio docopt imgui simple_web_server
	LINK asio docopt imgui simple_web_server)

if(MSVC)
	add_definitions(/experimental:module)
	add_definitions(/std:c++latest)
endif()