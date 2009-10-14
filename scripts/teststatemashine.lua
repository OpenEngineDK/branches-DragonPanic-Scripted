
ActivateAISubSystem()

st = StateMashine()
st:AddState("patrolling")
st:AddState("gaurding")

st:GotoState("patrolling")

function PrintState() 
	io.write("we are in state: " .. st:CurrentState() .. "\n")
end

function GetMashine() 
	return st:GetStateMashine()
end
