namespace engine
{
    class Application final
    {
	private:
		Application() = default;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:
		static Application& GetInstance();

	public:
		bool Start();
    };
}