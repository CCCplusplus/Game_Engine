local simpledraw = require "SimpleDraw"

print("hola mundo desde main.lua")

--print("hola mundo desde main.lua")

function DrawX(dt)
	print("draw desde lua" .. dt)
	SimpleDraw.Clear(20,20,20)
	simpledraw.DrawCircle(100,50,28, 255, 29, 141, 0)
end
