
--io.write("We managed to call a script\n")

--proat = GetProcessAttacher()
initat = GetInitAttacher()
--deinitat = GetDeinitAttacher()
--ListenOnEvent(proat, "processrunwhole.lua")
--ListenOnEvent(proat, "testscript.lua", "HandleProcessEvent")
--"testscript.lua","HandleInitEvent"
ListenOnEvent(initat, "testscript.lua", "HandleInitEvent", "teststack")
--ListenOnEvent(deinitat, "testscript.lua", "HandleDeinitEvent")

function HandleProcessEvent (e)
	io.write("we called the handleprocessevent jubiiiiii\n")
end

function HandleInitEvent (e)
	io.write(teststring)
end

function HandleDeinitEvent (e)
	io.write("we can Deinit with ease :)\n")
end

--p = CreatePoint();
--ChangePoint(p, 7, 12);
--PrintPoint(p);

--a = Account{balance=100}
--a:deposit(50.00)
--a:withdraw(25.00)
--print("Account balance = $"..a:balance())

function PrintVersion ()
	io.write("Version is succesful :)))))))))))))))\n")
end

function TestRegister ()
	p = CreatePoint();
	PrintPoint(p);
end 

function TestUserData (p)
	PrintPoint(p);
	ChangePoint(p, 90, 3);
	return p;
end 

function TestBool (b)
	if (b) then
		io.write("arg was true\n")
		return false;
	else 
		io.write("arg was false\n")
		return true;
	end
end

function TestString (s)
	io.write(s .. "\n")
	return (s .. " script appended")
end

function TestInt (i)
	io.write(i .. "\n")
	return 56;
end

function TestFloat (f)
	io.write(f .. "\n")
	return 768.003
end

function TestCombi (b, s, i, f)
	if (b) then
		io.write("ybi true  " .. s .. "  " .. i .. "  " .. f .."\n")
	else 
		io.write("buhhh false  " .. s .. "  " .. i .. "  " .. f .. "\n")
	end
end
