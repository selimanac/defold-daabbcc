local M={}

M.particleGroup = {}


function M.addCircle(_name, _size, _position,_targetId)
 local _radius = _size.x/2
 local _id = daabbcc.insertCircle(_name, _radius, _position.x , _position.y)
 table.insert(M.particleGroup,_id, {_id,_targetId})
 return _id
end

function M.addRect(_name, _size, _position,_targetId)
 local _id = daabbcc.insertRect(_name, _position.x , _position.y, _size.x, _size.y)
 table.insert(M.particleGroup,_id, {_id,_targetId})
return _id
end

return M
