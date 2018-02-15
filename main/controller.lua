local M={}

M.particleGroup = {}


function M.addCircle(_name, _size, _position,_targetId,rndSprite,rndType)
  local _radius = _size.x/2
  local _id = daabbcc.insertCircle(_name, _radius, _position.x , _position.y)
  table.insert(M.particleGroup,#M.particleGroup+1, {_id,_targetId,rndSprite,_position,rndType,_size,false})
  return _id
end

function M.addRect(_name, _size, _position,_targetId,rndSprite,rndType)
  local _id = daabbcc.insertRect(_name, _position.x , _position.y, _size.x, _size.y)
  table.insert(M.particleGroup,#M.particleGroup+1, {_id,_targetId,rndSprite,_position,rndType,_size,false})
  return _id
end


function M.updateCircle(_name, _size, _position, _id)
  local _radius = _size.x/2

  daabbcc.updateCircle(_name,_id, _radius, _position.x , _position.y)
end

function M.updateRect(_name, _size, _position,_id)
  daabbcc.updateRect(_name,_id, _position.x , _position.y, _size.x, _size.y)
end

function M.removeFromTree(_name,_id)
  table.remove(M.particleGroup, _id)
--  M.particleGroup[_id] = nil
  daabbcc.removeAABB("particles",_id)
end

return M
