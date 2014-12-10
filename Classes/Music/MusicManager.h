#ifndef __MUSIC_MANAGER_H__
#define __MUSIC_MANAGER_H__

#include <string>

#define SOUND_CLICK		"click.mp3"
#define SOUND_CHANGE	"clound.mp3"
class MusicManager
{
public:
	static void initAll();
	static void playSound(std::string sound);
};
#endif //__MUSIC_MANAGER_H__