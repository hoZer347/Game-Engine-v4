#include "Helper.h"

#include <iostream>

namespace loom
{
	Helper::Helper()
	{
		thread = std::thread([this]()
		{
			while (!KILL)
			{
				mut.lock();
				while (!in.empty() && !KILL)
				{
					out.push(in.front());
					in.pop();
				};
				mut.unlock();

				while (!out.empty() && !KILL)
				{
					out.front()();
					out.pop();
				};
			};
		});
	};
	Helper::~Helper()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
	void Helper::assign(Task task)
	{
		mut.lock();
		in.push(task);
		mut.unlock();
	};
	void Helper::assign(std::vector<Task> tasks)
	{
		mut.lock();
		for(auto& task : tasks)
			in.push(task);
		mut.unlock();
	};
	void Helper::shared_assign(std::vector<Task> tasks)
	{
		// TODO: support this
		std::cerr << "Helper::shared_assign Not Supported Yet" << std::endl;
	};
	SyncHelper::SyncHelper(Task task)
	: task(task)
	{ helpers.push_back(this); };
	SyncHelper::~SyncHelper()
	{
		KILL = true;
		if (thread.joinable())
			thread.join();
	};
};
