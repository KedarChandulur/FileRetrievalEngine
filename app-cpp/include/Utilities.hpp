#ifndef UTILITIES_H
#define UTILITIES_H

struct Utilites
{
	static void Mutex_Print(const int id, const char* message, bool printEndLine = true);

	//big 4
	Utilites() = delete;
	Utilites(const Utilites& cpy) = delete;
	Utilites& operator=(const Utilites& cpy) = delete;
	~Utilites() = delete;

private:
	//static std::mutex cout_mutex;
};

#endif