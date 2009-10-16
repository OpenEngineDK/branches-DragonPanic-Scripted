// Main setup for the OpenEngine DragonPanic project.
// -------------------------------------------------------------------
// Copyright (C) 2008 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// OpenEngine stuff
#include <Meta/Config.h>

// Core structures
#include <Core/Engine.h>

// Display structures
#include <Display/IFrame.h>
#include <Display/FollowCamera.h>
#include <Display/Frustum.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/ViewingVolume.h>
// SDL implementation
#include <Display/SDLEnvironment.h>
#include <Display/HUD.h>

// Rendering structures
#include <Scene/RenderNode.h>
// OpenGL rendering implementation
#include <Renderers/OpenGL/LightRenderer.h>
#include <Renderers/OpenGL/FBOBufferedRenderer.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Renderers/TextureLoader.h>

// Resources
#include <Resources/IModelResource.h>
#include <Resources/File.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>
// OBJ and TGA plugins
#include <Resources/RAWResource.h>
#include <Resources/ITextureResource.h>
#include <Resources/SDLImage.h>
#include <Resources/OBJResource.h>

// Scene structures
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/VertexArrayTransformer.h>
#include <Scene/DisplayListTransformer.h>
#include <Scene/PointLightNode.h>

// Utilities and logger
#include <Logging/Logger.h>
#include <Logging/StreamLogger.h>
#include <Utils/EventProfiler.h>
#include <Utils/FPSSurface.h>
#include <Scene/DotVisitor.h>

// OERacer utility files
#include <Utils/QuitHandler.h>

// sound 
#include <Sound/OpenALSoundSystem.h>
#include <Sound/MusicPlayer.h>
#include <Resources/VorbisResource.h>

#include <Meta/GLUT.h>

// NEW OEPARTICLESYSTEM TEST
#include <ParticleSystem/ParticleSystem.h>
#include <Effects/FireEffect.h>

// from project
#include "LightFader.h"
#include "KeyHandler.h"
#include "TransparentcyNode.h"
#include "GLSettingsNode.h"
#include "TimeModifier.h"

#include "Modules/Island/Island.h"
#include "Modules/Island/HeightMap.h"
#include "Modules/Target/Target.h"
#include "Modules/OscSurface/OscSurface.h"
#include "Modules/Dragon/Dragon.h"
#include "Modules/Boid/BoidsSystem.h"

#include "HUD/DragonHUD.h"

#include "scripts/AIscripts/support/MixedStuff.h"

//scriptsystem
#include <ScriptSystem.h>
#include <TestStuff/ScriptKeyHandler.h>

//test includes
#include <Math/Vector.h>
//#include <SubSystems/StoryProgression/StoryP.h>
//#include <SubSystems/AISubSystem/StateMashine.h>

// Additional namespaces
using namespace OpenEngine::Core;
using namespace OpenEngine::Logging;
using namespace OpenEngine::Devices;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Sound;

// Configuration structure to pass around to the setup methods
struct Config {
    IEngine&              engine;
    EventProfiler         prof;
    IEnvironment*         env;
    IFrame*               frame;
    Viewport*             viewport;
    IViewingVolume*       viewingvolume;
    FollowCamera*         camera;
    Frustum*              frustum;
    IRenderer*            renderer;
    IMouse*               mouse;
    IKeyboard*            keyboard;
    IJoystick*            joystick;
    ISceneNode*           scene;
    ISoundSystem*         soundsystem;
    MusicPlayer*          musicplayer;
    TimeModifier*         timeModifier;
    Target*               target;
    BoidsSystem*          boids;
    OscSurface*           oscs;
    Dragon*               dragon;
    bool                  resourcesLoaded;
    HUD*                  hud;
    OpenEngine::Renderers::TextureLoader* textureLoader;
    OpenEngine::ParticleSystem::ParticleSystem* particlesystem;
    OpenEngine::ParticleSystem::ParticleSystemTimer* pstimer;

    Config(IEngine& engine)
        : engine(engine)
        , env(NULL)
        , frame(NULL)
        , viewport(NULL)
        , viewingvolume(NULL)
        , camera(NULL)
        , frustum(NULL)
        , renderer(NULL)
        , mouse(NULL)
        , keyboard(NULL)
        , joystick(NULL)
        , scene(NULL)
        , soundsystem(NULL)
        , musicplayer(NULL)
        , timeModifier(NULL)
        , target(NULL)
        , boids(NULL)
        , oscs(NULL)
        , dragon(NULL)
        , resourcesLoaded(false)
        , hud(NULL)
        , textureLoader(NULL)
        , particlesystem(NULL)
        , pstimer(NULL)
    {}
};

// Forward declaration of the setup methods
void SetupResources(Config&);
void SetupDevices(Config&);
void SetupDisplay(Config&);
void SetupRendering(Config&);
void SetupParticleSystem(Config&);
void SetupSound(Config&);
void SetupScene(Config&);
void SetupDebugging(Config&);

int main(int argc, char** argv) {
    InitializeGlut(&argc, argv);

    // Setup logging facilities.
    Logger::AddLogger(new StreamLogger(&std::cout));

    // Print usage info.
    logger.info << "========= ";
    logger.info << "Running The OpenEngine DragonPanic Project";
    logger.info << " =========" << logger.end;

    logger.info << "This project is a simple testing project for OpenEngine.";
    logger.info << logger.end;

    logger.info << "  for control information see: ";
    logger.info << "KeyboardLayout.txt in the project repository" << logger.end;
    logger.info << logger.end;

    // Create an engine and config object
    Engine* engine = new Engine();
    Config config(*engine);

   //test scriptsystem
    ScriptSystem::InitScriptSystem("projects/DragonPanic-Scripted/scripts/", engine);
/*
   //test get global func
   int ip = 50;
   ScriptSystem::RunScriptFunc("defs.lua", "SetP", "", "p", "", &ip);

   int i;
   bool b;
   double d;
   string s;
   int* ip2;

   ScriptSystem::GetGlobal("defs.lua", "count", 'i', &i);
   ScriptSystem::GetGlobal("defs.lua", "version", 's', &s);
   ScriptSystem::GetGlobal("defs.lua", "checked", 'b', &b);
   ScriptSystem::GetGlobal("defs.lua", "time", 'd', &d);
   ScriptSystem::GetGlobal("defs.lua", "p", 'p', &ip2);

   logger.info << "values returned are: " << i << ", " << s << ", " << b << ", " << d << ", " << *ip2 << logger.end;
*/
    //register the project script activatefunc
//    ScriptSystem::RegisterNonClassFunc("ActivateAISupport", ActivateAISupportFuncs);

    //test scriptvector
//    Vector<3, float> testvec = Vector<3, float>(4,4,4);
//    ScriptSystem::RunScriptFunc("/home/jakob/Skrivebord/openengine/projects/DragonPanic/scripts/AIscripts/VectorTest.lua", "test", "", "p", "", &testvec);

 //   ScriptSystem::RunScript("usage.lua");
 //   ssystem->RunScript("testsingleton.lua");
 //   ssystem->RunScript("setup.lua");

    //userdata test
  /*  Point* p = new Point(12, 34);
    ssystem->RunScriptFunc("testscript.lua", "TestUserData", "u", "u", p, sizeof(*p), p, sizeof(*p));
    logger.info << "point returned with x: " << p->x << " and y: " << p->y << logger.end;    */

    //string test
 /*   string s = "kan du appended???";
    ssystem->RunScriptFunc("testscript.lua", "TestString", "s", "s", s.c_str(), &s);
    logger.info << "resultat af test: " << s << logger.end;*/

    //bool test
/*    bool b = true;
    ssystem->RunScriptFunc("testscript.lua", "TestBool", "b", "b", b, &b);
    if (b)
	logger.info << "true" << logger.end;
    else
    	logger.info << "false" << logger.end;*/

    //int test
/*    int i = 42;
    ssystem->RunScriptFunc("testscript.lua", "TestInt", "i", "i", i, &i);
    logger.info << "resultat af test: " << i << logger.end;

    //float test
    double d = 25.96;
    ssystem->RunScriptFunc("testscript.lua", "TestFloat", "d", "d", d, &d);
    logger.info << "resultat af test: " << d << logger.end;*/

    //test of story progression
 /*   ScriptSystem::RunScript("TestStoryP.lua");

    StoryP* sp;
    ScriptSystem::RunScriptFunc("TestStoryP.lua", "GetStory", "", "p", &sp);
    sp->Mark("KilledMorgan");
    logger.info << "we are on the CPP side and have killed Morgan: " 
		<< sp->IsReached("KilledMorgan") << logger.end;	*/

    //test statemashines 
/*    ScriptSystem::RunScript("teststatemashine.lua");

    StateMashine* sm;
    ScriptSystem::RunScriptFunc("teststatemashine.lua", "GetMashine", "", "p", &sm);
    ScriptSystem::RunScriptFunc("teststatemashine.lua", "PrintState", "", "");
    sm->GotoState("gaurding");
    ScriptSystem::RunScriptFunc("teststatemashine.lua", "PrintState", "", "");*/

    // Setup the engine
    SetupResources(config);
    SetupDisplay(config);
    SetupDevices(config);
    SetupSound(config);
    SetupParticleSystem(config);
    SetupRendering(config);
    SetupScene(config);
    
    //these test are here cause at this point the default scripts have all been run
//    ScriptSystem::RunScript("define.lua", "teststack");
 //   ScriptSystem::RunScript("usage.lua", "teststack");
//    ScriptSystem::RunScript("testscript.lua", "teststack");

    // Possibly add some debugging stuff
    SetupDebugging(config);

    // Start up the engine.
    engine->Start();

// #if OE_DEBUG
    // Print out any profiling info
    config.prof.DumpInfo();
// #endif

    ScriptSystem::DeinitScriptSystem();

    // release event system
    // post condition: scene and modules are not processed
    delete engine;

    // destroy all modules
    // post condition: no one has refs to the scene
    //delete config.boids; //delete sounds
    delete config.soundsystem; // close soundsystem

    delete config.scene;

    // Return when the engine stops.
    return EXIT_SUCCESS;
}


void SetupParticleSystem(Config& config) {

    //get particle system from setupscript
    OpenEngine::ParticleSystem::ParticleSystem* ps;	
    ScriptSystem::RunScriptFunc("setup.lua", "GetSystem", "", "", "p", &ps);
    config.particlesystem = ps;

}

void SetupSound(Config& config) {

    ScriptSystem::RunScriptFunc("sound.lua", "Setup", "", "pp", "", config.scene, config.camera);

    ISoundSystem* ss;
    ScriptSystem::RunScriptFunc("sound.lua", "GetSoundSystem", "", "", "p", &ss);
    config.soundsystem = ss;	

    MusicPlayer* mp;
    ScriptSystem::RunScriptFunc("sound.lua", "GetMusicPlayer", "", "", "p", &mp);
    config.musicplayer = mp;
}

void SetupResources(Config& config) {
    // set the resources directory
    // @todo we should check that this path exists
    // set the resources directory
    string resources = "projects/DragonPanic/data/";
    DirectoryManager::AppendPath(resources);

    // load resource plug-ins
    ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
    ResourceManager<ITextureResource>::AddPlugin(new SDLImagePlugin());
    ResourceManager<ISoundResource>::AddPlugin(new VorbisResourcePlugin());

    config.resourcesLoaded = true;
}

void SetupDisplay(Config& config) {
    if (config.frame         != NULL ||
        config.viewingvolume != NULL ||
        config.camera        != NULL ||
        config.frustum       != NULL ||
        config.viewport      != NULL)
        throw Exception("Setup display dependencies are not satisfied.");

    config.env = new SDLEnvironment(800, 600);
    //config.frame         = new SDLFrame(1024, 768, 32, FRAME_FULLSCREEN);    
    config.frame         = &config.env->GetFrame();
    config.viewingvolume = new InterpolatedViewingVolume(*(new ViewingVolume()));

    FollowCamera* cam;
    ScriptSystem::RunScriptFunc("setup.lua", "GetCamera", "", "p", "p", config.viewingvolume, &cam);
    config.camera = cam;

    //config.frustum       = new Frustum(*config.camera, 20, 3000);
    config.viewport      = new Viewport(*config.frame);
    config.viewport->SetViewingVolume(config.camera);

    config.engine.InitializeEvent().Attach(*config.frame);
    config.engine.ProcessEvent().Attach(*config.frame);
    config.engine.DeinitializeEvent().Attach(*config.frame);
}

void SetupDevices(Config& config) {
    if (config.keyboard != NULL ||
        config.mouse    != NULL ||
        config.env      == NULL)
        throw Exception("Setup devices dependencies are not satisfied.");
    // Create the mouse and keyboard input modules
    
    config.keyboard = config.env->GetKeyboard();
    config.mouse = config.env->GetMouse();
    config.joystick = config.env->GetJoystick();

    // Bind the quit handler
    QuitHandler* quit_h = new QuitHandler(config.engine);
    config.keyboard->KeyEvent().Attach(*quit_h);

    // Bind to the engine for processing time
    config.engine.InitializeEvent().Attach(*config.env);
    config.engine.ProcessEvent().Attach(*config.env);
    config.engine.DeinitializeEvent().Attach(*config.env);
}

void SetupRendering(Config& config) {
    if (config.viewport == NULL ||
        config.renderer != NULL ||
        config.camera == NULL ||
        config.soundsystem == NULL )
        throw Exception("Setup renderer dependencies are not satisfied.");

    // Create a renderer
    config.renderer = new OpenGL::Renderer(config.viewport);
    //config.renderer = new FBOBufferedRenderer(config.viewport);

    // Setup a rendering view
    IRenderingView* rv = new OpenGL::RenderingView(*config.viewport);
    config.renderer->ProcessEvent().Attach(*rv);

    // Add rendering initialization tasks
    config.textureLoader = new Renderers::TextureLoader(*config.renderer);
    config.renderer->PreProcessEvent().Attach(*config.textureLoader);

    DisplayListTransformer* dlt = new DisplayListTransformer(rv);
    config.renderer->InitializeEvent().Attach(*dlt);

    config.renderer->PreProcessEvent()
        .Attach( *(new Renderers::OpenGL::LightRenderer(*config.camera)) );

    config.engine.InitializeEvent().Attach(*config.renderer);
    config.engine.ProcessEvent().Attach(*config.renderer);
    config.engine.DeinitializeEvent().Attach(*config.renderer);

    config.hud = new HUD();
    config.renderer->PostProcessEvent().Attach( *config.hud );
}

void SetupScene(Config& config) {
    if (config.scene  != NULL ||
        config.mouse  == NULL ||
        config.keyboard == NULL ||
        config.particlesystem == NULL ||
        config.soundsystem == NULL ||
        config.resourcesLoaded == false)
        throw Exception("Setup scene dependencies are not satisfied.");

    // Create a root scene node

    RenderStateNode* renderStateNode = new RenderStateNode();
    renderStateNode->EnableOption(RenderStateNode::LIGHTING);
    config.scene = renderStateNode;

    float fadetime = 3000.0 * 3.5;
    GLSettingsNode* glNode = new GLSettingsNode(fadetime);
    config.engine.ProcessEvent().Attach(*glNode);
    renderStateNode->AddNode(glNode);

    // attach scene to soundsystem
    config.soundsystem->SetRoot(config.scene);
    
    // Set scene lighting
    float pFade = 1.4;

    PointLightNode* light1 = new PointLightNode();
    light1->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light1Position = new TransformationNode();
    light1Position->SetPosition(Vector<3,float>(0.8, 0.7, 0.4));
    light1Position->AddNode(light1);
    config.scene->AddNode(light1Position);
    PointLightNode* to1 = new PointLightNode();
    to1->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to1->diffuse = Vector<4,float>(0.4*pFade, 0.4*pFade, 0.7*pFade, 1.0);
    to1->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);

    LightFader* light1Fader = new LightFader(light1, to1, light1, fadetime);
    config.engine.ProcessEvent().Attach(*light1Fader);

    PointLightNode* light2 = new PointLightNode();
    light2->ambient = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->diffuse = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    light2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    TransformationNode* light2Position = new TransformationNode();
    light2Position->SetPosition(Vector<3,float>(-1.0, 1.0, 0.7));
    light2Position->AddNode(light2);
    config.scene->AddNode(light2Position);
    PointLightNode* to2 = new PointLightNode();
    to2->ambient = Vector<4,float>(0.1*pFade, 0.1*pFade, 0.1*pFade, 1.0);
    to2->diffuse = Vector<4,float>(0.8*pFade, 0.8*pFade, 0.7*pFade, 1.0);
    to2->specular = Vector<4,float>(0.0, 0.0, 0.0, 1.0);
    LightFader* light2Fader = new LightFader(light2, to2, light2, fadetime);
    config.engine.ProcessEvent().Attach(*light2Fader);

    TimeModifier* timeModifier = 
        new TimeModifier(config.engine.ProcessEvent(),1.00f);
    config.timeModifier = timeModifier;
    //timeModifier->SetFactor(1.23);

    timeModifier->ProcessEvent().Attach(*config.particlesystem);

    //init HeightMap
    string filename = DirectoryManager::FindFileInPath("Island/Terrain5.raw");
    ITextureResourcePtr hMap = 
      ITextureResourcePtr(new RAWResource(filename, 1024, 1024, 8));
    hMap->Load();
        
    ITextureResourcePtr texture =
      ResourceManager<ITextureResource>::Create("Island/ground.tga");

    HeightMap* heightMap = new HeightMap(hMap, texture, 300.0, 0.25, 16);

    Island* island = new Island(heightMap);
    config.scene->AddNode(island);
    hMap->Unload();

    Target* target = config.target = new Target(*heightMap);
    TransformationNode* targetNode = target->GetTargetNode();
    config.scene->AddNode(targetNode);
    config.engine.ProcessEvent().Attach(*target);

    TransparencyNode* tpNode = new TransparencyNode();
    config.scene->AddNode(tpNode);

    Vector<4,float> oscsColor(0.8f,0.25f,0.0f,0.7f); // lava
    //Vector<4,float> oscsColor(0.1f,0.25f,0.7f,0.7f); // water
    OscSurface* oscs = config.oscs = new OscSurface(heightMap,oscsColor);
    timeModifier->ProcessEvent().Attach(*oscs);
    tpNode->AddNode(oscs);

    //@todo: Boids have transparent shadows
    BoidsSystem* boids = config.boids = new BoidsSystem(heightMap, oscs,*config.soundsystem,
                                                        *config.particlesystem,
                                                        *config.textureLoader,
                                                        config.scene);
    config.engine.InitializeEvent().Attach(*boids);
    timeModifier->ProcessEvent().Attach(*boids);
    tpNode->AddNode(boids);


    Dragon* dragon = config.dragon = new Dragon(heightMap, *config.boids, target,
                                                *config.textureLoader,
                                                *config.particlesystem, config.scene);
    config.scene->AddNode(dragon);
    config.engine.InitializeEvent().Attach(*dragon);
    config.engine.ProcessEvent().Attach(*dragon);

    
    // game state logic
    ScriptSystem::RunScriptFunc("logic.lua", "Init", "", "i", "", 120);

    KeyHandler* key_h = new KeyHandler(*config.camera, *targetNode, *heightMap,
                                       *config.mouse, island, dragon, boids, *timeModifier, *config.musicplayer, *config.frame, renderStateNode);
    //    KeyHandler* key_h = new KeyHandler(*config.camera, *targetNode, *heightMap, island, dragon, boids, *timeModifier, *config.gamestate);

    config.engine.ProcessEvent().Attach(*key_h);
    config.keyboard->KeyEvent().Attach(*key_h);

    config.joystick->JoystickButtonEvent().Attach(*key_h);
    config.joystick->JoystickAxisEvent().Attach(*key_h);

    config.textureLoader->Load(*config.scene);

    // Transform the scene to use vertex arrays
    VertexArrayTransformer vaT;
    vaT.Transform(*config.scene);

    // Supply the scene to the renderer
    config.renderer->SetSceneRoot(config.scene);

    //HUD
    config.textureLoader->SetDefaultReloadPolicy(Renderers::TextureLoader::RELOAD_QUEUED);
    DragonHUD* hud = new DragonHUD(*config.frame, *config.hud, *config.textureLoader);
    config.textureLoader->SetDefaultReloadPolicy(Renderers::TextureLoader::RELOAD_NEVER);
    //config.scene->AddNode(hud->GetLayerNode());
    config.engine.ProcessEvent().Attach(*hud);

    //script test
    ScriptKeyHandler* skey = new ScriptKeyHandler();
    config.keyboard->KeyEvent().Attach(*skey);
}

void SetupDebugging(Config& config) {
// #if OE_DEBUG
    // main engine events
    config.prof.Profile<ProcessEventArg>
        ("Environment", config.engine.ProcessEvent(), *config.env);
    config.prof.Profile<ProcessEventArg>
        ("Sound System", config.engine.ProcessEvent(), *config.soundsystem);
    config.prof.Profile<ProcessEventArg>
        ("Music Player", config.engine.ProcessEvent(), *config.musicplayer);
    config.prof.Profile<ProcessEventArg>
        ("Target",       config.engine.ProcessEvent(), *config.target);
    config.prof.Profile<ProcessEventArg>
        ("Renderer",     config.engine.ProcessEvent(), *config.renderer);
    config.prof.Profile<ProcessEventArg>
        ("Dragon",       config.engine.ProcessEvent(), *config.dragon);
    // renderer events
    config.prof.Profile<RenderingEventArg>
        ("HUD", config.renderer->ProcessEvent(), *config.hud);
    // time modified events
    config.prof.Profile<ProcessEventArg>
        ("Boids System",    config.timeModifier->ProcessEvent(), *config.boids);
    config.prof.Profile<ProcessEventArg>
        ("Osc Surface",     config.timeModifier->ProcessEvent(), *config.oscs);
    config.prof.Profile<ProcessEventArg>
        ("OE Particle System", config.timeModifier->ProcessEvent(), *config.particlesystem);

    // Visualization of the frustum
    if (config.frustum != NULL) {
        config.frustum->VisualizeClipping(true);
        config.scene->AddNode(config.frustum->GetFrustumNode());
    }

    ofstream dotfile("scene.dot", ofstream::out);
    if (!dotfile.good()) {
        logger.error << "Can not open 'scene.dot' for output"
                     << logger.end;
    } else {
        DotVisitor dot;
        dot.Write(*config.scene, &dotfile);
        logger.info << "Saved scene graph to 'scene.dot'"
                    << logger.end
                    << "To create a SVG image run: "
                    << "dot -Tsvg scene.dot > scene.svg"
                    << logger.end;
    }
// #endif

    // FPS layer with cairo
    FPSSurfacePtr fps = FPSSurface::Create();
    config.textureLoader->Load(fps, TextureLoader::RELOAD_QUEUED);
    config.engine.ProcessEvent().Attach(*fps);
    HUD::Surface* fpshud = config.hud->CreateSurface(fps);
    fpshud->SetPosition(HUD::Surface::LEFT, HUD::Surface::TOP); 
}

