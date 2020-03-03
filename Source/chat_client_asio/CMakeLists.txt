SET( DEFINE
)
SET( INCLUDE
asio
)
SET( LINK
asio
)

create_project(CONSOLE DEFINE INCLUDE LINK)

if(MSVC)
	add_definitions(/await)
endif()