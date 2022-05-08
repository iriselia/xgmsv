cmaid_project(
	CONFIGURATION CONSOLE
	DEFINE
	INCLUDE core
	LINK core)

if(MSVC)
	add_definitions(/experimental:module)
	add_definitions(/std:c++latest)
endif()