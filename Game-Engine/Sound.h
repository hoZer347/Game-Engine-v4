#pragma once

#include "Data.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_output.h"
#include "fmod_errors.h"
#include "fmod_common.h"

#include <assert.h>
#include <iostream>

#ifndef LOOM_MAX_SOUND_CHANNELS
#define LOOM_MAX_SOUND_CHANNELS 64
#endif

namespace loom
{
	struct Sound final :
		virtual public Updateable
	{
		_NODISCARD static inline const FMOD::System* get_system() { return system; };

		static inline int master_volume = 100;

	protected:
		friend struct SoundStream;
		friend struct SoundInstance;
		friend struct ptr<Sound>;

		static inline FMOD::System* system = nullptr;

		Sound()
		{
			unsigned int version;

			if (FMOD_RESULT result = FMOD::System_Create(&system))
				std::cout << FMOD_ErrorString(result) << std::endl << __LINE__ << std::endl;
			
			if (FMOD_RESULT result = system->init(LOOM_MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, nullptr))
				std::cout << FMOD_ErrorString(result) << std::endl << __LINE__ << std::endl;
			
			if (FMOD_RESULT result = system->getVersion(&version))
				std::cout << FMOD_ErrorString(result) << std::endl << __LINE__ << std::endl;

			assert((version == FMOD_VERSION, "Incorrect Version"));
		};

		void update() override { system->update(); };

	private:
		static inline ptr<Sound> manager { 1 };
	};


	struct SoundStream final
	{
		void play()
		{
			if (FMOD_RESULT result = Sound::system->playSound(sound, 0, false, 0))
				std::cout << FMOD_ErrorString(result) << " at Line: " << __LINE__ << std::endl;
		};
		void stop()
		{
			if (FMOD_RESULT result = Sound::system->playSound(sound, 0, true, 0))
				std::cout << FMOD_ErrorString(result) << " at Line: " << __LINE__ << std::endl;
		};

		int volume = 100;
		static inline int master_volume = 100;

		~SoundStream() { sound->release(); };

	protected:
		friend struct ptr<SoundStream>;
		SoundStream(const char* file_name)
		{
			if (FMOD_RESULT result = Sound::system->createStream(file_name, FMOD_DEFAULT, 0, &sound))
				std::cout << FMOD_ErrorString(result) << " at Line: " << __LINE__ << std::endl;
		};

	private:
		FMOD::Sound* sound = nullptr;
	};


	struct SoundInstance final
	{
		void play()
		{
			if (FMOD_RESULT result = Sound::system->playSound(sound, 0, false, 0))
				std::cout << FMOD_ErrorString(result) << " at Line: " << __LINE__ << std::endl;
		};
		
		int volume = 100;
		static inline int master_volume = 100;

		~SoundInstance() { sound->release(); };

	protected:
		friend struct ptr<SoundStream>;
		SoundInstance(const char* file_name)
		{
			if (FMOD_RESULT result = Sound::system->createSound(file_name, FMOD_DEFAULT, 0, &sound))
				std::cout << FMOD_ErrorString(result) << " at Line: " << __LINE__ << std::endl;
		};

	private:
		FMOD::Sound* sound = nullptr;
	};
};
