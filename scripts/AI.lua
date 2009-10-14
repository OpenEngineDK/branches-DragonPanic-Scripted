
ActivateAISupport()

counter = 0

maxspeed = 5.0
maxforce = 10.0
mass = 1.0

accsteeringvectors = {}
accexternalforces = {}
accexternalimpluses = {}
upvecs = {}
leftvecs = {}
forvecs = {}
boidfires = {}
airborns = {}
boids = {}
isdead = {}
isburning = {}
isdrowning = {}
lifes = {}
hots = {}
runnings = {}
cycles = {}

function SetupAI (heightmap, bs, ocs)
	hm = HeightMap(heightmap)
	boidsys = bs
	oscsur = osc
end 

function AddBoid (boidfire, boid, forward) 
	accsteeringvectors[counter] = FloatVector()
	accexternalforces[counter] = FloatVector()
	accexternalimpluses[counter] = FloatVector()
	upvecs[counter] = FloatVector(0, 1, 0)
	forvecs[counter] = FloatVector(forward):GetNormalize()
	leftvecs[counter] = upvecs[counter]:Cross(forvecs[counter])
	airborns[counter] = false
	boidfires[counter] = boidfire
	boids[counter] = boid
	isdead[counter] = false
	isburning[counter] = false 
	isdrowning[counter] = false
	lifes[counter] = 1.0
	hots[counter] = 0.0
	runnings[counter] = 0.0
	cycles[counter] = 0.0
	counter = counter + 1
	return (counter - 1)
end 

--function AddSteering (id, steering, weight)
function AddSteering ()
--	steeringvec = FloatVector(steering)
--	calvec = steeringvec:Mul(weight)
--	accsteering = accsteeringvectors[id]
--	accsteering:AddTo(calvec)	
end	

function AddExternalForce (id, force) 	
	accexternalforce = accexternalforces[id]
   	accexternalforce:AddTo(force)
end

function AddExternalImpulse (id, impulse) 
	accexternalimpulse = accexternalimpulses[id]
    	accexternalimpulse:AddTo(impulse)
end

function AddVelocity (id, steering, weight, velocity)
	steeringvec = FloatVector(steering) 
	vect = steeringvec:GetNormalize()
	vect:MulTo(maxspeed)
	vect:SubtractFrom(velocity)
	vect:MulTo(weight)

	accsteering = accsteeringvectors[id]
	accsteering:AddTo(vect)
end

function GetUpVector (id)
	return upvecs[id]:GetVector()
end

function GetLeftVector (id)
	return leftvecs[id]:GetVector()
end

function GetForwardVector (id)
	return forvecs[id]:GetVector()
end

function GetRunning (id)
	return runnings[id]
end

function GetCycle (id)
	return cycles[id]
end

function UpdateSteering (id, time)
	accsteering = accsteeringvectors[id] 
  	steeringforce = Truncate(accsteering, maxforce) 
    	accsteering:SetToZero();
end

function UpdatePhysics (id, time, position, velocity, upright) 

	posvec = FloatVector(position)
	forvec = forvecs[id]
	leftvec = leftvecs[id]
	upvec = upvecs[id]
	velvec = FloatVector(velocity)
	uprightvec = FloatVector(upright)
	airborn = airborns[id]
	dead = isdead[id]
	burning = isburning[id]
	life = lifes[id]

	gravity = FloatVector(0, -20, 0)
	AddExternalForce(id, gravity)
	calvec = posvec:Add(up)
	if (calvec:GetY() < 0.0) then
		calvec = FloatVector(-(velvec:GetX()), -(velvec:GetY()) + 22, -(velvec:GetZ()))
		AddExternalForce(id, calvec)
	end  

	if (not airborn) and (dead or (upvec:Dot(upright) < 0.8)) then 
		AddExternalForce(id, velvec:Mul(-5))
	end

	if not airborn then
		stepf = hm:NormalAt(position)
		stepf:SetY(0.0)

		if (stepf:GetLength() > 0.2) then
			calvec = stepf:GetNormalize()	
			calvec:MulTo(math.pow(stepf:GetLength(), 3))
			AddExternalForce(id, calvec:MulTo(4))
		end
	end 

	calvec = forvec:Mul(7)
	calvec:AddTo(position)
	cal2vec = hm:HeightAt(calvec)
	aheadstraight = cal2vec:GetY()

	cal3vec = leftvec:Mul(7)
	calvec:AddTo(cal3vec)
	cal2vec = hm:HeightAt(calvec)
	aheadleft = cal2vec:GetY()

	cal3vec:MulTo(2)
	calvec:SubtractFrom(cal3vec)
	cal2vec = hm:HeightAt(calvec)
	aheadright = cal2vec:GetY()
	
	if aheadright < 0.0 then
		AddSteering(id, leftvec:Mul(5), 1.0) 
	end 
	
	if aheadleft < 0.0 then
		AddSteering(id, leftvec:Mul(-5), 1.0) 
	end 

	if aheadstraight < 0.0 then
		AddSteering(id, forvec:Mul(-5), 2.0) 
	end 

	if (Abs(aheadright - posvec:GetY())) > 5 then
		weight = math.pow(Abs(posvec:GetY() - aheadright), 3)
		AddSteering(id, leftvec:Mul(0.005), weight)
	end

	if (Abs(aheadleft - posvec:GetY())) > 5 then
		weight = math.pow(Abs(posvec:GetY() - aheadleft), 3)
		AddSteering(id, leftvec:Mul(-0.005), weight)
	end	
	
	if (Abs(aheadstraight - posvec:GetY())) > 5 then
		weight = math.pow(Abs(posvec:GetY() - aheadstraight), 3)
		AddSteering(id, forvec:Mul(-0.005), weight)
	end

	if burning then
		AddSteering(id, velvec:Mul(3), life)
	end

	if (not airborn) and (not dead) and (upvec:DotProduct(upright) > 0.8) then 
		calvec = steeringforce:Mul((1/mass)*time)
		calvec:AddTo(velocity)
		velvec = Truncate(calvec, maxspeed)	
	end

	calvec = accexternalforces[id]:Mul((1/mass)*time)
	accexternalforces[id]:SetToZero()
	velvec:AddTo(calvec)

	prevpos = FloatVector(posvec:GetVector())
	posvec:AddTo(velvec:Mul(time))

	calvec = hm:HeightAt(posvec)
	if posvec:GetY() < (calvec:GetY() + 10.0*time) then
		airborn = false
	else
		airborn = true
	end
	
	calvec = hm:HeightAt(posvec)
	if posvec:GetY() < calvec:GetY() then
		posvec = calvec
	end

	if (time > 0) and (not airborn) then
		calvec = posvec:Subtract(prevpos)
		calvec:DevideBy(time)
		velvec = calvec
	end

	calvec = accexternalimpulses[id]:Mul(1/mass)
	if (calvec:GetLength() > 0.0) then
		accexternalimpluses[id]:SetToZero()
		velvec:AddTo(calvec)
		calvec:MulTo(time)
		posvec:AddTo(calvec)
		airborn = true
	end

	airborns[id] = airborn

	return posvec:GetVector(), velocity:GetVector()
end  

function UpdateLocomotion (id, time, position, velocity, upright, burned)

	posvec = FloatVector(position)
	upvec = upvecs[id]
	leftvec = leftvecs[id]
	uprightvec = FloatVector(upright)
	forvec = forvecs[id]
	velvec = FloatVector(velocity)
	boidfire = boidfires[id]
	airborn = airborns[id]
	dead = isdead[id]
	burning = isburning[id]
	life = lifes[id]
	hot = hots[id]
	drowning = isdrowning[id]
	running = runnings[id]
	walkcycle = cycles[id]

	SetTransformationNode(boidfire, position)

	normalvec = hm:NormalAt(position)
	if (airborn and drowning) then
		normalvec = FloatVector(0.0, 1.0, 0.0)
	end 

	if (airborn and (not drowning)) then
		calvec = velvec:Mul(time*1.0)
		upvec:AddTo(calvec)
		upvec = Turncate(upvec, maxspeed)
		upvec = (FloatVector(upvec:GetX(), math.max(0.0, upvec:GetY()), upvec:GetZ())):GetNormalize()
		leftvec = (upvec:Cross(forward)):GetNormalize()
		forvec = (leftvec:Cross(upvec)):GetNormalize()
	elseif dead then
		calvec = leftvec:Cross(normalvec)
		calvec:MulTo(time*6.0)
		upvec = (upvec:AddTo(calvec)):GetNormalize()
		calvec = normalvec:Mul(-1*time*6.0)
		forvec = (forvec:AddTo(calvec)):GetNormalize()
		leftvec = (upvec:Cross(forward)):GetNormalize()
		forvec = (leftvec:Cross(upvec)):GetNormalize()	
	else
		calvec = uprightvec:Mul(time*2.0)
		upvec = (upvec:AddTo(calvec)):GetNormalize()
		calvec = velvec:Mul(time*3.0)
		forvec = forvec:AddTo(Truncate(calvec, maxspeed):GetNormalize())
		left = (upvec:Cross(forvec)):GetNormalize()
		forvec = (leftvec:Cross(upvec)):GetNormalize()
	end

	running = math.min(1.0, (velvec:GetLength())/maxspeed)
	walkcycle = walkcycle + (time*math.sqrt(running))

	if dead then
		walkcycle = 0.0
	end

	if (posvec:GetY() < 0.0) and ((posvec:Subtract(velvec:Mul(time))):GetY() > 0.0) then
		MakeRipple(oscsur, posvec:GetX(), posvec:GetZ(), 1.2, (velvec:GetLength())*0.3);
	end

	if (posvec:Add(upvec)):GetY() < 0.0 then
		drowning = true
		burned = burned + 100.0
	else
		drowning = false
	end

	if burned > 0.0 then
		hot = hot + (burned*time*0.005)
		life = life - (burned*time*0.0003)
		hot = hot - (time/3.0)
		burned = 0.0
	end

	if (hot > 1) and (life > 0.0) then
		if not burning then
			Scream(posvec:GetVector());  
			SetActiveBoidfire(boidfire, true);
		end
		burning = true
	end

	if burning then
		life = life - (0.05*time)
	end

	if life <= 0.0 then
		if not dead then
			KillBoid(boids[id], boidsys)
		end
		dead = true
	end

	if (life < -0.4) then
		burning = false
		SetActiveBoidfire(boidfire, false)
		life = 0.0
	end

	upvecs[id] = upvec
	leftvecs[id] = leftvec
	forvecs[id] = forvec
	airborns[id] = airborn
	isdead[id] = dead
	isburning[id] = burning
	lifes[id] = life
	hots[id] = hot
	isdrowning[id] = drowning
	runnings[id] = running
	cycles[id] = walkcycle

	return burned
end 


