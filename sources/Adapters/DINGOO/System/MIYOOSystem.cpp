#include "DINGOOSystem.h"
#include "Adapters/Unix/FileSystem/UnixFileSystem.h"
#include "Adapters/SDL/GUI/GUIFactory.h"
#include "Adapters/SDL/GUI/SDLGUIWindowImp.h"
#include "Adapters/SDL/GUI/SDLEventManager.h"
#include "Adapters/SDL/Process/SDLProcess.h"
#include "Adapters/SDL/Audio/SDLAudio.h"
#include "Adapters/Dummy/Midi/DummyMidi.h"
#include "Externals/TinyXML/tinyxml.h"
#include "Application/Model/Config.h"
#include "Application/Controllers/ControlRoom.h"
#include "Application/Player/SyncMaster.h"
#include "Application/Commands/NodeList.h"
#include "Adapters/SDL/Timer/SDLTimer.h"
#include "System/Console/Logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

EventManager *GPSDLSystem::eventManager_ = NULL;
bool GPSDLSystem::invert_ = false;
int GPSDLSystem::lastBattLevel_ = 100;
unsigned int GPSDLSystem::lastBeatCount_ = 0;
static char* strval;
static size_t n;
static size_t t;
static int secbase;

int GPSDLSystem::MainLoop() {
    eventManager_ -> InstallMappings();
    return eventManager_ -> MainLoop();
}

void GPSDLSystem::Boot(int argc,char **argv) {
    // Install System
    System::Install(new GPSDLSystem());

    // Install FileSystem
    FileSystem::Install(new UnixFileSystem());
    Path::SetAlias("bin", ".");
    Path::SetAlias("root", ".");

    Config::GetInstance() -> ProcessArguments(argc,argv);

#ifdef _DEBUG
    Trace::GetInstance() -> SetLogger(*(new StdOutLogger()));
#else
    Path logPath("bin:lgpt.log");
    FileLogger *fileLogger = new FileLogger(logPath);
    if (fileLogger -> Init().Succeeded()) {
        Trace::GetInstance()->SetLogger(*fileLogger);
    }
#endif

    // Install GUI Factory
    I_GUIWindowFactory::Install(new GUIFactory());

    // Install Timers
    TimerService::GetInstance() -> Install(new SDLTimerService());

    // Install Sound
    AudioSettings hint;
    hint.bufferSize_ = 1024;
    hint.preBufferCount_ = 8;
    Audio::Install(new SDLAudio(hint));

    // Install Midi
    MidiService::Install(new DummyMidi());

    // Install Threads
    SysProcessFactory::Install(new SDLProcessFactory());
    if (SDL_Init(SDL_INIT_EVENTTHREAD | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) < 0) {
        return;
    }
    SDL_EnableUNICODE(1);
    atexit(SDL_Quit);

    eventManager_ = I_GUIWindowFactory::GetInstance() -> GetEventManager();
    eventManager_ -> Init();

    bool invert = false;
    Config *config=Config::GetInstance();
    const char *s=config -> GetValue("INVERT");

    if ((s) && (!strcmp(s, "YES"))) {
        invert=true;
    }

    if (!invert) {
        eventManager_->MapAppButton("left ctrl", APP_BUTTON_A);
        eventManager_->MapAppButton("left alt", APP_BUTTON_B);
    } else {
        eventManager_->MapAppButton("left alt", APP_BUTTON_A);
        eventManager_->MapAppButton("left ctrl", APP_BUTTON_B);
    }
    eventManager_->MapAppButton("return", APP_BUTTON_START);
    eventManager_->MapAppButton("tab", APP_BUTTON_L);
    eventManager_->MapAppButton("backspace", APP_BUTTON_R);
    eventManager_->MapAppButton("right", APP_BUTTON_RIGHT);
    eventManager_->MapAppButton("left", APP_BUTTON_LEFT);
    eventManager_->MapAppButton("down", APP_BUTTON_DOWN);
    eventManager_->MapAppButton("up", APP_BUTTON_UP);

    n = 4;
    t = sizeof(char);
    strval = (char*)malloc((size_t)5*sizeof(char));
}

void GPSDLSystem::Shutdown() {
    delete Audio::GetInstance();
}

unsigned long GPSDLSystem::GetClock() {
    struct timeval tp;

    gettimeofday(&tp, NULL);
    if (!secbase) {
        secbase = tp.tv_sec;
        return long(tp.tv_usec/1000.0);
    }
    return long((tp.tv_sec - secbase) * 1000 + tp.tv_usec / 1000.0);
}

int GPSDLSystem::GetBatteryLevel() {
    return -1;
}

void GPSDLSystem::Sleep(int millisec) {
    if (millisec>0)
        assert(0);
}

void *GPSDLSystem::Malloc(unsigned size) {
    void *ptr=malloc(size);
    return ptr;
}

void GPSDLSystem::Free(void *ptr) {
    free(ptr);
}

void GPSDLSystem::Memset(void *addr,char val,int size) {
    memset(addr,val,size);
}

void *GPSDLSystem::Memcpy(void *s1, const void *s2, int n) {
    return memcpy(s1,s2,n);
}

void GPSDLSystem::PostQuitMessage() {
    SDLEventManager::GetInstance()->PostQuitMessage();
}

unsigned int GPSDLSystem::GetMemoryUsage() {
    struct mallinfo mi;
    mi = mallinfo();
    return mi.uordblks;
}
