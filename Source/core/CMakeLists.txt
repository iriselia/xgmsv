cmaid_project(
	CONFIGURATION STATIC
	DEFINE
	INCLUDE asio docopt imgui glfw
	LINK asio docopt imgui glfw)

if(MSVC)
	add_definitions(/experimental:module)
	add_definitions(/std:c++latest)
endif()