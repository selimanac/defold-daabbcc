local utils = {}

function utils.table_copy(tbl)
	local copy = {}
	for key, value in pairs(tbl) do
		if type(value) ~= 'table' then
			copy[key] = value
		else
			copy[key] = utils.table_copy(value)
		end
	end
	return copy
end

return utils
