
ActivateStoryProgressionSubSystem()

--make milestones
m1 = MileStone()
m2 = MileStone()

--make storyp and add
sp = StoryP()
sp:AddMileStone("TalkToMarketNPC", m1:GetMileStone())
sp:AddMileStone("KilledMorgan", m2:GetMileStone())

--check script side
if (sp:IsReached("KilledMorgan")) then
	io.write("Morgan is dead:))")
else
	io.write("Morgan is still alive:(((")
end 

function GetStory() 

	return sp:GetStoryP()

end
