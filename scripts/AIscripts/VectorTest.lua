
ActivateAISupport()

tofunctions = false
printthem = false

function test (vector)

	if tofunctions and printthem then
		testvec = FloatVector()
		printvec(testvec)
		io.write("adding constant 13\n")
		testvec:AddTo(13.0)
		printvec(testvec)
		io.write("adding argument vector\n")
		testvec:SetToZero()
		testvec:AddTo(vector)
		printvec(testvec)
		io.write("adding another floatvector\n")
		testvec:SetToZero()
		other = FloatVector(3, 4, 5)
		testvec:AddTo(other)
		printvec(testvec)
	else 
		if printthem then
			testvec = FloatVector()
			io.write("contant addition returned vector\n")
			printvec(testvec:Add(12))
			io.write("original vector\n")
			printvec(testvec)
			io.write("vector addition returned vector\n")
			printvec(testvec:Add(vector))
			io.write("original vector\n")
			printvec(testvec)
			other = FloatVector(5,1,7)
			io.write("own vector addition returned vector\n")
			printvec(testvec:Add(other))
			io.write("original vector\n")
			printvec(testvec)		
		end
	end

--	test2vec = FloatVector(9,7,5)
--	printvec(test2vec:GetNormalize())
--	printvec(test2vec:Mul(2))
	io.write("Result is: " .. math.sqrt(100) .. "\n")

end 

function printvec (vec)

	--io.write(vec .. "\n")
	io.write("From script x is: " .. vec:GetX() .. " and y is: " .. vec:GetY() .. " and z is: " .. vec:GetZ() .. "\n")

end
