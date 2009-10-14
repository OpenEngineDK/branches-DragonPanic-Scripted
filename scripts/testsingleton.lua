
sys = GetScriptSystem()
e = GetEngineProcessEvent(sys)
ListenOnEvent(e, "testsingleton.lua", "HandleProcessEvent2")

function HandleProcessEvent2 (e)
	RunScript("processrunwhole.lua")
end

