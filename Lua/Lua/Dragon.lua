function plustwo (x)
local a
a = 2
return a + x
end

function addnum_lua(a, b)
return addnum_c(a, b)
end

rows = 7
cols = addnum_lua(rows, 2)