
ActivateMusicSubSystem()

--sream sounds
sreamtable = {} 

function Setup (scene, camera) 

	--setup openalsoundsystem
	sounds = OpenALSoundSystem(scene, camera)
	sounds:SetMasterGain(1.0);
	Attach(proat, sounds:GetOpenALSoundSystem()) 

	--setup musicplayer
--	musicp = MusicPlayer(camera)
	musicp = MusicPlayer(camera, sounds:GetOpenALSoundSystem())
	musicp:AddSound("Music/defibrilation.ogg") --sang "Music/beak.ogg"
	musicp:AddSound("Music/beak.ogg")
	musicp:SetGain(0.3) --val 0.3
        musicp:Shuffle(true)
        musicp:Next()
        musicp:Play()
        Attach(proat, musicp:GetMusicPlayer())

	--setup sreams
	sreamtable[0] = IMonoSound(sounds:CreateSound("SoundFX/vester-aargh.ogg"))
	sreamtable[1] = IMonoSound(sounds:CreateSound("SoundFX/cpvc-AARGH.ogg"))
	sreamtable[2] = IMonoSound(sounds:CreateSound("SoundFX/ian-aargh.ogg"))
	sreamtable[3] = IMonoSound(sounds:CreateSound("SoundFX/jakob-aargh.ogg"))
	sreamtable[4] = IMonoSound(sounds:CreateSound("SoundFX/salomon-aargh.ogg"))
	sreamtable[5] = IMonoSound(sounds:CreateSound("SoundFX/ptx-jargh.ogg"))

end 

function GetSoundSystem ()
	return sounds:GetOpenALSoundSystem()
end 

function GetMusicPlayer()
	return musicp:GetMusicPlayer()
end 

function Scream (pos) 
	index = GetRandomInt(# sreamtable)
	sound = sreamtable[index]
	sound:SetPosition(pos)
	sound:Play()
end

function Next ()
	musicp:Next()
end 

function TestScream ()
	index = RunScriptFunc("sound2.lua", "GetScreamIndex", 1)	
	--io.write(index .. " is")
	sound = sreamtable[index]
	sound:Play()
end

--mulige sange
--"Music/defibrilation.ogg"
--"Music/glow.ogg"
--"Music/trouble.ogg"
