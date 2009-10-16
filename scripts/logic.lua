
ActivateUtilSubSystem()

function Init (dur)	
	score = 0
	duration = dur
	timer = Timer()
	timer:Start()
end

function BoidDied ()
	score = score + 1
end

function GetTime ()
	return timer:GetElapsedIntervals(1000000)
end

function GetTimeLeft ()	
	if (duration <= GetTime()) then 
		return 0;
	end
  	return duration - GetTime();		
end

function GetScore ()
	return score
end

function Reset ()
	timer:Reset()
	timer:Start()
	score = 0
end

function Pause ()
	timer:Stop()
end

function Unpause ()
	timer:Start()
end

