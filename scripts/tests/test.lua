
--teststring = "sidste afgørende test for recompiling systemet\n"

RunScriptFunc("tests/test2.lua", "SetString", 1)
io.write(teststring)

--function Print ()
--	 io.write(teststring)
--end
