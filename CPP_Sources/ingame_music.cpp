#include <Windows.h>

#include "ingame_music.h"

volatile UINT WDEVICEID;

void play_wav_music(int track)
{
    if (track < 0 || track > 9)
    {
        return;
    }
    DWORD dwReturn = 0;
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;
    WCHAR path[] = L"SOUND\\MUSIC\\Track00.wav";
    WCHAR track_char = track + '0';
    path[18] = track_char;

    if (WDEVICEID)
    {
        stop_wav_music();
    }

    mciOpenParms.lpstrElementName = path;
    dwReturn = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);
    if (dwReturn)
    {
        return;
    }
    WDEVICEID = mciOpenParms.wDeviceID;
    mciPlayParms.dwCallback = NULL;
    dwReturn = mciSendCommand(WDEVICEID, MCI_PLAY, NULL, (DWORD)(LPVOID)&mciPlayParms);
    if (dwReturn)
    {
        stop_wav_music();
    }
    return;
}

void stop_wav_music()
{
    mciSendCommand(WDEVICEID, MCI_CLOSE, 0, NULL);
    WDEVICEID = NULL;
}