--CreateRenderable("Created", "Cube", "T-Rex")
--SetLocation("Created", 10.0, 0.0, 0.0)
--[[
function CreateCubes(times)
    for i=0, times, 1
    do
        CreateRenderable("Created" .. tostring(i), "Cube", "Default")
        SetLocation("Created" .. tostring(i), i * 2.0, 3.0, 0.0)
    end
end
--]]