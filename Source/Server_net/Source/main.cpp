namespace mmo_server
{
	const char* usage = 1 + R"(
MMO Server.

    Usage:
      mmo_server [options]
      mmo_server -h | --help | --version

    Options:
      -hl, --headless                           Start the server without GUI.
)";

	void init(int argc, char** argv);
	void run();
	void stop();
}

//*
int main(int argc, char** argv)
{
	mmo_server::init(argc, argv);
	mmo_server::run();
	mmo_server::stop();
}
//*/