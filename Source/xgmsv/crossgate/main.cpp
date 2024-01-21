namespace xgmsv
{
	const char* usage = 1 + R"(
MMO Server.

    Usage:
      xgmsv [options]
      xgmsv -h | --help | --version

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
	xgmsv::init(argc, argv);
	xgmsv::run();
	xgmsv::stop();
}
//*/


