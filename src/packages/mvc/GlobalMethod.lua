
--两个向量求边
function VectorSubtract(vector1,vector2)
	local vector = {}
	local x = vector1.x - vector2.x
	local y = vector1.y - vector2.y
	vector.x = x
	vector.y = y
	return vector
end

--向量的垂直向量
function VectorPrependicular(vector)
	local temp = {}
	local x = vector.y
	local y = -vector.x
	if vector.x == 0 then
		y = 0
	end
	temp.x = x
	temp.y = y
	return temp
end

--求向量长度
function VectorMagnitude(vector)
	local x = vector.x
	local y = vector.y
	local length = math.sqrt(math.pow(x,2)+math.pow(y,2))
	return length
end

--求单位向量
function VectorNormalize(vector)
	local temp = {}
	local m = VectorMagnitude(vector)
	if length ~= 0 then
		temp.x = vector.x / m
		temp.y = vector.y / m
	end
	return temp
end

--求矢量点乘(投射点)
--vector1为投影轴
--vector2为某个点
function VecotDotProduct(vector1,vector2)
	local dotProductResult = vector1.x*vector2.x+vector1.y*vector2.y
    return dotProductResult
end

--查找最小的值
function FindMinValue(tValue)
	local tempMin = tValue[1]
	for i,v in ipairs(tValue) do
		if i < #tValue then
			if tValue[i+1] < tempMin then
				tempMin = tValue[i+1]
		    end
		end
	end
	return tempMin
end

--查找最小的值
function FindMaxValue(tValue)
	local tempMax = tValue[1]
	local count = #tValue
	for i,v in ipairs(tValue) do
		if i < count then
			if tValue[i+1] > tempMax then
				tempMax = tValue[i+1]
		    end
		end
	end
	return tempMax
end



