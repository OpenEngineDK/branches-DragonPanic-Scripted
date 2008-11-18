#ifndef _BOIDS_SYSTEM_H_
#define _BOIDS_SYSTEM_H_

//inherits from
#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Core/IEvent.h>
#include <Core/Event.h>
#include <Scene/RenderNode.h>

//templated classes
#include <Math/Vector.h>

// non reference or pointer member variables
#include <Utils/Timer.h>
#include <Math/RandomGenerator.h>
#include <vector>

#include <Sound/ISoundSystem.h>

// @todo: needs to be here for windows to compile
#include "BoidSystemEvents.h"
#include "../Particle/ParticleSystemEvents.h"

//forward reference
class Boid;
class HeightMap;

class OscSurface;

namespace OpenEngine {
    namespace Renderers {
        class TextureLoader;
    }
    namespace ParticleSystem {
        class ParticleSystem;
    }
    namespace Renderers {
        class IRenderingView;
    }
    namespace Scene {
        class ISceneNode;
    }
    namespace Sound {
        class IMonoSound;
    }
}

using namespace OpenEngine;

using OpenEngine::Core::IListener;
using OpenEngine::Core::InitializeEventArg;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Core::IEvent;
using OpenEngine::Core::Event;
using OpenEngine::Math::Vector;
using OpenEngine::Scene::RenderNode;
using OpenEngine::Renderers::IRenderingView;
using OpenEngine::Utils::Timer;
using OpenEngine::Sound::ISoundSystem;
using OpenEngine::Math::RandomGenerator;
using OpenEngine::Scene::ISceneNode;

#define numberOfBoids 49 //must be a square number, 9, 16, 25, 36...

class BoidsSystem : public IListener<InitializeEventArg>,
  public IListener<ProcessEventArg>, public RenderNode, 
public IListener<ParticleSystemEventArg> {
public:
  unsigned int aliveBoids;
  bool enabled;
    BoidsSystem(HeightMap* heightMap, OscSurface* oscsurface, ISoundSystem& soundsystem,
                OpenEngine::ParticleSystem::ParticleSystem& oeparticlesystem,
                OpenEngine::Renderers::TextureLoader& texloader,
                ISceneNode* particleRoot);
    ~BoidsSystem();
    void toggleRenderState();

    void Handle(InitializeEventArg arg);
    void Handle(ProcessEventArg arg);

    void ResetBoids();

    virtual void Apply(IRenderingView* rv);
  
    void Handle(ParticleSystemEventArg arg);
    void HandleFire(Vector<3,float> position, float strength);
    void IncNumberOfShownBoids();
    void DecNumberOfShownBoids();
    void IncAlignment();
    void DecAlignment();
    float getAlignment();

    void BoidDied(Boid& boid);
    virtual IEvent<BoidSystemEventArg>& BoidSystemEvent();

private:
    Event<BoidSystemEventArg> boidEvents;

    HeightMap* heightMap;
    OscSurface* oscsurface;

    ISceneNode* particleRoot;
    ISoundSystem& soundsystem;
    OpenEngine::ParticleSystem::ParticleSystem& oeparticlesystem;
    OpenEngine::Renderers::TextureLoader& texloader;
    std::vector<Sound::IMonoSound*> screams;
    RandomGenerator randGen;

    Timer timer;

    unsigned int numberOfShownBoids;
    float alignment;
    Boid* boids[numberOfBoids];

    bool disableLogic;
    int renderState, numberOfRenderStates;

    void AddSoundToList(std::string soundfile);
};

#endif
