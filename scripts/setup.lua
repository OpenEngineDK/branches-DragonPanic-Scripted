
ActivateParticleSubSystem()
ActivateCameraSubSystem()

--particle system setup
particlesystem = GetParticleSystem()
initat = GetInitAttacher()
proat = GetProcessAttacher()
deinitat = GetDeinitAttacher()

Attach(initat, particlesystem)
Attach(deinitat, particlesystem)

function GetSystem ()
	return particlesystem
end

--camera setup
function GetCamera(viewvol)
	cam = FollowCamera(viewvol)
	return cam:GetFollowCamera()	
end
