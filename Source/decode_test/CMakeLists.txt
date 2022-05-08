cmaid_project(
	CONFIGURATION CONSOLE
	DEFINE
	INCLUDE core
	LINK core)

add_definitions("/utf-8")
#SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} /utf-8")