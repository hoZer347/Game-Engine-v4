#pragma once

#include "../Data.h"
#include "../Text.h"

#ifndef DIALOGUE_FILE_LOCATION
#define DIALOGUE_FILE_LOCATION "Assets/"
#endif

namespace loom
{
	struct DialogueChunk final
	{
		std::vector<Task> commands;

		std::string
			line0,
			line1,
			line2,
			line3;
	};


	struct Dialogue final :
		virtual public Updateable
	{
		Dialogue(std::string file_name = "DefaultDialogue.txt", ptr<Font> font, Task&& on_end);
		~Dialogue();

	private:
		void update() override;

		Task on_end;
		std::fstream file;
		std::string line;
		ptr<DynamicText> text;
	};

	Dialogue::Dialogue(std::string file_name, ptr<Font> font, Task&& on_end) :
		text(1, font, ""),
		file(DIALOGUE_FILE_LOCATION + file_name),
		on_end(on_end)
	{ };
	Dialogue::~Dialogue()
	{ };

	void Dialogue::update()
	{ };
};
