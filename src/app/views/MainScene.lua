
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

MainScene.RESOURCE_FILENAME = "MainScene.csb"

function MainScene:onCreate()
    print("MainScene:onCreate")
    MainScene.m_root = self:getResourceNode()
    MainScene.m_root_table = self
end

function MainScene:onEnter()
    print("MainScene:onEnter ")
    MainScene:initUI()
end

function MainScene:onEnterTransitionDidFinish()
    print("MainScene:onEnterTransitionDidFinish")
end

function MainScene:handEvent(tag)
    print("MainScene:handEvent = ",tag)
    if tag == exit then
        MainScene:uninit()
    end
end

function MainScene:initUI()
    print("MainScene:init")
    self:init()
    
    local openglView = cc.Director:getInstance():getOpenGLView()
    local frameSize = openglView:getFrameSize()
    self.m_nframeSizeWidth = frameSize.width
    self.m_nframeSizeHeight = frameSize.height
    self.m_nopenglScaleX = openglView:getScaleX()
    self.m_nopenglScaleY = openglView:getScaleY()
    self.m_nscale= nil
    if self.m_nopenglScaleX > self.m_nopenglScaleY then
        self.m_nscale = self.m_nopenglScaleY / self.m_nopenglScaleX
    else
        self.m_nscale = self.m_nopenglScaleX / self.m_nopenglScaleY
    end

    local contentLayout = self.m_root:getChildByName("contentLayout_mainScene")

    
    local layoutBottom = contentLayout:getChildByName("layoutBottom_mainscene")

    local loadbIntensity = layoutBottom:getChildByName("loadbIntensity_mainScene")
    loadbIntensity = tolua.cast(loadbIntensity,"ccui.LoadingBar")
    loadbIntensity:setPercent(0)
    loadbIntensity:setScaleX(1/self.m_nopenglScaleX)
    loadbIntensity:setScaleY(1/self.m_nopenglScaleY)

    local imageBg = layoutBottom:getChildByName("imageBg_mainscene")
    imageBg:setScaleX(1/self.m_nopenglScaleX)
    imageBg:setScaleY(1/self.m_nopenglScaleY)

    local Text_1 = layoutBottom:getChildByName("Text_1")
    Text_1:setScaleX(1/self.m_nopenglScaleX)
    Text_1:setScaleY(1/self.m_nopenglScaleY)

    local btnAddAccel = layoutBottom:getChildByName("btnAddAccel_mainScene")
    btnAddAccel = tolua.cast(btnAddAccel,"ccui.Button")
    btnAddAccel:setScaleX(1/self.m_nopenglScaleX)
    btnAddAccel:setScaleY(1/self.m_nopenglScaleY)

    local btnMinusAccel = layoutBottom:getChildByName("btnMinusAccel_mainScene")
    btnMinusAccel = tolua.cast(btnMinusAccel,"ccui.Button")
    btnMinusAccel:setScaleX(1/self.m_nopenglScaleX)
    btnMinusAccel:setScaleY(1/self.m_nopenglScaleY)

    local btnAddUp = layoutBottom:getChildByName("btnAddUp_mainScene")
    btnAddUp = tolua.cast(btnAddUp,"ccui.Button")
    btnAddUp:setScaleX(1/self.m_nopenglScaleX)
    btnAddUp:setScaleY(1/self.m_nopenglScaleY)

    local btnMinusUp = layoutBottom:getChildByName("btnMinusUp_mainScene")
    btnMinusUp = tolua.cast(btnMinusUp,"ccui.Button")
    btnMinusUp:setScaleX(1/self.m_nopenglScaleX)
    btnMinusUp:setScaleY(1/self.m_nopenglScaleY)

    local txtAcceleration = layoutBottom:getChildByName("txtAcceleration_mainScene")
    txtAcceleration = tolua.cast(txtAcceleration,"ccui.Text")
    txtAcceleration:setScaleX(1/self.m_nopenglScaleX)
    txtAcceleration:setScaleY(1/self.m_nopenglScaleY)

    local txtUpValue = layoutBottom:getChildByName("txtUpValue_mainScene")
    txtUpValue = tolua.cast(txtUpValue,"ccui.Text")
    txtUpValue:setScaleX(1/self.m_nopenglScaleX)
    txtUpValue:setScaleY(1/self.m_nopenglScaleY)

    local function onClickChangeValue(sender)  --增加加速度
        if sender == btnAddAccel then
            local curAcceleration = tonumber(txtAcceleration:getString())
            curAcceleration = curAcceleration + 2
            self:setAcceleration(curAcceleration)
            txtAcceleration:setText(curAcceleration)
        elseif sender == btnMinusAccel then
            local curAcceleration = tonumber(txtAcceleration:getString())
            curAcceleration = curAcceleration - 2
            self:setAcceleration(curAcceleration)
            txtAcceleration:setString(curAcceleration)
        elseif sender == btnAddUp then
            local curUpValue = tonumber(txtUpValue:getString())
            curUpValue = curUpValue + 2
            self:setUpValue(curUpValue)
            txtUpValue:setString(curUpValue)
        elseif sender == btnMinusUp then
            local curUpValue = tonumber(txtUpValue:getString())
            curUpValue = curUpValue - 2
            self:setUpValue(curUpValue)
            txtUpValue:setString(curUpValue)
        end
    end
    
    btnAddAccel:addClickEventListener(onClickChangeValue)
    btnMinusAccel:addClickEventListener(onClickChangeValue)
    btnAddUp:addClickEventListener(onClickChangeValue)
    btnMinusUp:addClickEventListener(onClickChangeValue)

    self.m_nodeLoadBar = loadbIntensity

    local function topTouchHandle(sender,eventType)
        self:handTopTouchEvent(sender,eventType)
    end

    local topLayout = contentLayout:getChildByName("topLayout" .. CURRENT .. "_mainScene")
    topLayout:setVisible(true)
    topLayout:addTouchEventListener(topTouchHandle)
    topLayout:setScaleX(1/self.m_nopenglScaleX)
    topLayout:setScaleY(1/self.m_nopenglScaleY)
    self.m_oPillarContainer = topLayout
    self.m_nMaxX = -(self.m_oPillarContainer:getContentSize().width - 640)

    local imgHero = topLayout:getChildByName("imgHero_mainScene")
    imgHero = tolua.cast(imgHero,"ccui.ImageView")
    self.m_tHeroSize = imgHero:getContentSize()

    local btnReset = layoutBottom:getChildByName("btnReset_mainScene")
    btnReset =tolua.cast(btnReset, "ccui.Button")
    btnReset:setScaleX(1/self.m_nopenglScaleX)
    btnReset:setScaleY(1/self.m_nopenglScaleY)
    btnReset:addClickEventListener(function (sender)
        self.m_oPillarContainer:setPositionX(0)
        self:resetAcceleration()
        self:resetValue()
        self.m_nCurStandIndex = 1
        self.m_imageHero:setPosition(cc.p(self.m_nOrigintPSX,self.m_nOrigintPSY))
    end)

    self.m_imageHero = imgHero

   

    self.m_tAllPillarObject = {}
    local stairs,tempPSX,tempPSY,tempSize,tempWidth,tempHeight
    local count = 0
    for i=1,50 do
       stairs = topLayout:getChildByName("stairs" .. i .. "_mainScene")
       if stairs == nil then
           break
       end
        if i == 1 then
            local tempxxx ,tempyyy = stairs:getPosition()
            local dddd = stairs:getContentSize()
            local sizesss = stairs:getContentSize()
            local tempppx = tempxxx+sizesss.width/2 -self.m_tHeroSize.width/2
            local tempppy = tempyyy+dddd.height
            self.m_imageHero:setPosition(cc.p(tempppx,tempppy))
            self.m_nOrigintPSX  = tempX
            self.m_nOrigintPSY = tempY
        end
        tempPSX = stairs:getPositionX()
        tempPSY = stairs:getPositionY()
        tempSize = stairs:getContentSize()
        tempWidth = tempSize.width
        tempHeight = tempSize.height

       local pillarInfo = {}
       pillarInfo.node = stairs
       pillarInfo.psX = tempPSX
       pillarInfo.psY = tempPSY
       pillarInfo.sizeWidth = tempWidth
       pillarInfo.sizeHeight = tempHeight

       table.insert(self.m_tAllPillarObject,pillarInfo)
       count = count + 1
    end

    local setCount = math.ceil(count/4)  --计算把柱子分成几个集合
    local tempCount = 1
    
    for i=1,setCount do
        local tempT = {}
        local minX = nil
        local maxX = nil
        for a=1,4 do
            local tempTT = {}
            if tempCount > count then break end
            local tPillarInfo =self.m_tAllPillarObject[tempCount]
            if a==1 then
                minX = tPillarInfo.psX
            end
            
            if a == 4 or tempCount == count then
                maxX = tPillarInfo.psX + tPillarInfo.sizeWidth
            end
            tempTT.node = tPillarInfo.node
            local pointA,pointB,pointC,pointD = self:findPoint(tempTT.node)
            tempTT.psx = tPillarInfo.psX
            tempTT.psy = tPillarInfo.psY
            tempTT.width = tPillarInfo.sizeWidth
            tempTT.height = tPillarInfo.sizeHeight
            tempTT.pointA = pointA
            tempTT.pointB = pointB
            tempTT.pointC = pointC
            tempTT.pointD = pointD

            tempTT.name = tPillarInfo.node:getName()
            tempCount = tempCount + 1
            if minX and maxX then
                tempTT.minX = minX
                tempTT.maxX = maxX
            end
            --获取柱子的两个投影轴的单位向量
            local edge1Normalize,edge2Normalize,edge3Normalize,edge4Normalize = self:findPrependicular(tempTT.node)
            tempTT.edge1Normalize = edge1Normalize
            tempTT.edge2Normalize = edge2Normalize
            tempTT.edge3Normalize = edge3Normalize
            tempTT.edge4Normalize = edge4Normalize
            table.insert(tempT,tempTT)
        end
        table.insert(self.m_tBigSet,tempT)
    end

    --dump(self.m_tBigSet, "AppBase aaaa")
    local scheduler = cc.Director:getInstance():getScheduler()
    local function tempUpdate(interval)
        self:update(interval)
    end
    self.schedulerTag = scheduler:scheduleScriptFunc(tempUpdate, 0, false)
end

function MainScene:init()
    self.m_fPercent = 0
    self.m_nAcceleration = 104
    self.m_nTempAcceleration = 104
    self.m_nTempUp = 700
    self.m_nSpeedUp = 700   --向上的加速度
    self.m_nGravity = 8   --重力
    self.m_bUp = false
    self.m_tHeroSize = nil
    self.m_nCurrentPSX = 0
    self.m_nCurrentPSY = 0
    self.m_bCollision = false
    self.m_nCurStandIndex = 1
    self.m_bCollisionPillar = false --与柱子碰撞的状态
    self.m_bCheckCollision = true  --是否检测与柱子的碰撞检测
    self.m_tAllSquareAxisVecotr = {}  --存放每个长方形的投影轴
    self.m_tHeroPrependAxis = {}  --当前位置的英雄各条边的投影轴
    self.m_oPillarContainer = nil
    self.m_tCheckPillar = nil  --存放需要做碰撞检测的柱子
    self.m_tBigSet = {}     --大的集合
    self.m_nMaxX = 0
    self.m_nTouchCount = 0
    self.m_oCurCheckPillar = nil
end

function MainScene:uninit()
    self.m_root = nil
    self.m_root_table = nil
    self.m_fPercent = nil
    self.m_nTempAcceleration = nil
    self.m_nTempUp = nil
    self.m_tHeroSize = nil
    self.m_bCollision = nil
    self.m_bCheckCollision = nil
    self.m_bCollisionPillar = nil
    self.m_tAllSquareAxisVecotr = nil
    self.m_tHeroPrependAxis = nil
    self.m_oPillarContainer = nil
    self.m_tCheckPillar = nil
    self.m_tBigSet = nil
    self.m_nMaxX = nil
    self.m_oCurCheckPillar = nil
    self.m_nTouchCount = nil
    self:unSchedule()
end

function MainScene:unSchedule()
    if self.schedulerTag ~= nil then
        local scheduler = cc.Director:getInstance():getScheduler()
        scheduler:unscheduleScriptEntry(self.schedulerTag)
        self.schedulerTag = nil
    end
end


function MainScene:handTopTouchEvent(sender,eventType)
    if self.m_nAcceleration == 0 or self.m_nTouchCount > 1 then
        return
    end
    if eventType == ccui.TouchEventType.began then
        self.m_bUpdateLoadingBax = true
         self.m_bUp = false
    elseif eventType == ccui.TouchEventType.canceled then
        self.m_bUpdateLoadingBax = false
        self.m_nodeLoadBar:setPercent(0)
        self.m_bUp = false
    elseif eventType == ccui.TouchEventType.ended then
        self.m_bUpdateLoadingBax = false
        self.m_fPercent = self.m_nodeLoadBar:getPercent()
        
        self.m_tCheckPillar = nil
        if self.m_nSpeedUp == self.m_nTempUp or self.m_bCollision then
            self.m_nCurrentPSX = self.m_imageHero:getPositionX()
            self.m_nCurrentPSY = self.m_imageHero:getPositionY()
        end
        self.m_nSpeedUp = self.m_nTempUp
        self.m_nSpeedUp = self.m_nSpeedUp * (self.m_fPercent/100)
        self.m_bUp = true
        self.m_nTouchCount = self.m_nTouchCount + 1
        self.m_nodeLoadBar:setPercent(0)
    end
end

function MainScene:update(interval)
    if self.m_root == nil then
        local scheduler = cc.Director:getInstance():getScheduler()
        scheduler:unscheduleScriptEntry(self.schedulerTag)
        self.schedulerTag = nil
        return
    end
    if self.m_bUpdateLoadingBax then
        local percent = self.m_nodeLoadBar:getPercent()
        if percent == 0 then
            self.m_bFront = true
            self.m_bBack = false
        elseif percent == 100 then
            self.m_bBack = true
            self.m_bFront = false
        end

        if self.m_bFront then
            self.m_nodeLoadBar:setPercent(percent + 1)
        elseif self.m_bBack then
            self.m_nodeLoadBar:setPercent(percent - 1)
        end
    else
        if self.m_bUp then  --是否可以执行跳跃飞行动作
            self:runJump(interval)
        end
    end
end

--执行跳跃飞行动作
function MainScene:runJump(interval)
    local nextX = self.m_imageHero:getPositionX() + interval*self.m_nAcceleration
    local nextY = self.m_imageHero:getPositionY() + interval*self.m_nSpeedUp
    
    local containerNextX = math.abs(self.m_oPillarContainer:getPositionX()) + interval*100
    
    if self.m_nAcceleration ~= 0 then
        self.m_bCollision = self:checkCollision(nextX,nextY)
    end
    self.m_nSpeedUp = self.m_nSpeedUp - self.m_nGravity
    if self.m_bCollisionPillar then --碰撞到柱子了
        self.m_nAcceleration = 0
        self.m_oPillarContainer:setTouchEnabled(false)
        if nextX > self.m_oCurCheckPillar:getPositionX() -  self.m_tHeroSize.width then
            nextX = self.m_oCurCheckPillar:getPositionX() -self.m_tHeroSize.width
        end
    else
        if -containerNextX > self.m_nMaxX then
            self.m_oPillarContainer:setPositionX(-containerNextX)
        end
    end

    self.m_imageHero:setPosition(cc.p(nextX,nextY))
    if self.m_bCollision then
        if self.m_oCurCheckPillar ~= nil then
            local size = self.m_oCurCheckPillar:getContentSize()
            local psy = self.m_oCurCheckPillar:getPositionY()
            self.m_imageHero:setPosition(cc.p(nextX,size.height+psy))
        end

        local maxCount = #self.m_tBigSet
        local maxSet = self.m_tBigSet[maxCount]
        maxCount = #maxSet
        local tempNode = maxSet[maxCount].node
        
        if self.m_oCurCheckPillar ~= nil and self.m_oCurCheckPillar == tempNode then
            self:unSchedule()
            GAME_OVER = true
            require("app.MyApp"):create():run("StartScene")
            return
        end
        self:resetValue()
    end
    
    if nextY < -50 then
        self:resetValue()
        self:resetAcceleration()
        self.m_imageHero:setPosition(cc.p(self.m_nCurrentPSX,self.m_nCurrentPSY))
        if -(self.m_nCurrentPSX-60) > self.m_nMaxX then
            self.m_oPillarContainer:setPositionX(-(self.m_nCurrentPSX-60))
        else
            self.m_oPillarContainer:setPositionX(self.m_nMaxX)
        end
    end
    
    self.m_tCheckPillar = nil
end

function MainScene:resetValue()
    self.m_bCollisionPillar = false
    self.m_nSpeedUp = self.m_nTempUp
    self.m_bUp = false
    self.m_nTouchCount = 0
    self.m_oCurCheckPillar = nil
end

function MainScene:resetAcceleration()
    self.m_oPillarContainer:setTouchEnabled(true)
    self.m_nAcceleration = self.m_nTempAcceleration
end

function MainScene:setAcceleration(accel)
    self.m_nAcceleration =  accel
    self.m_nTempAcceleration = accel
end

function MainScene:setUpValue(upValue)
    self.m_nSpeedUp = upValue
    self.m_nTempUp = upValue
end

--人物与各条柱子的碰撞检测
function MainScene:checkCollision(psx,psy)
    self:findNearestpPillar(psx,psy)
    if  self.m_tCheckPillar == nil then
        self.m_tCheckPillar = self.m_tBigSet[#self.m_tBigSet]
    end
    local tempPoint1 , tempPoint2 ,tempPoint3,tempPoint4 = self:findPoint(self.m_imageHero,psx,psy)
    local heroEdge1Normalize,heroEdge2Normalize,heroEdge3Normalize,heroEdge4Normalize = self:findPrependicular(self.m_imageHero,psx,psy)
    for i,v in ipairs(self.m_tCheckPillar) do
        local node = v.node
        
        local height = v.psy + v.height
        local nodePointA = v.pointA
        local nodePointB = v.pointB
        local nodePointC = v.pointC
        local nodePointD = v.pointD
        local nodeEdge1Normaliz = v.edge1Normalize
        local nodeEdge2Normaliz = v.edge2Normalize
        local nodeEdge3Normaliz = v.edge3Normalize
        local nodeEdge4Normaliz = v.edge4Normalize
        
        local temp1 = nil
        local nodePointAPSByNodeEdge1 = nodePointA.x * nodeEdge1Normaliz.x + nodePointA.y * nodeEdge1Normaliz.y
        local nodePointBPSByNodeEdge1 = nodePointB.x * nodeEdge1Normaliz.x + nodePointB.y * nodeEdge1Normaliz.y
        local nodePointCPSByNodeEdge1 = nodePointC.x * nodeEdge1Normaliz.x + nodePointC.y * nodeEdge1Normaliz.y
        local nodePointDPSByNodeEdge1 = nodePointD.x * nodeEdge1Normaliz.x + nodePointD.y * nodeEdge1Normaliz.y
        local nodeNodeEdge1Max = math.max(nodePointAPSByNodeEdge1,nodePointBPSByNodeEdge1,nodePointCPSByNodeEdge1,nodePointDPSByNodeEdge1)
        local nodeNodeEdge1Min = math.min(nodePointAPSByNodeEdge1,nodePointBPSByNodeEdge1,nodePointCPSByNodeEdge1,nodePointDPSByNodeEdge1)

        local heroPointAPSByNodeEdge1 = tempPoint1.x * nodeEdge1Normaliz.x + tempPoint1.y * nodeEdge1Normaliz.y
        local heroPointBPSByNodeEdge1 = tempPoint2.x * nodeEdge1Normaliz.x + tempPoint2.y * nodeEdge1Normaliz.y
        local heroPointCPSByNodeEdge1 = tempPoint3.x * nodeEdge1Normaliz.x + tempPoint3.y * nodeEdge1Normaliz.y
        local heroPointDPSByNodeEdge1 = tempPoint4.x * nodeEdge1Normaliz.x + tempPoint4.y * nodeEdge1Normaliz.y
        local heroNodeEdge1Max = math.max(heroPointAPSByNodeEdge1,heroPointBPSByNodeEdge1,heroPointCPSByNodeEdge1,heroPointDPSByNodeEdge1)
        local heroNodeEdge1Min = math.min(heroPointAPSByNodeEdge1,heroPointBPSByNodeEdge1,heroPointCPSByNodeEdge1,heroPointDPSByNodeEdge1)

        if heroNodeEdge1Min <= nodeNodeEdge1Max and heroNodeEdge1Max >= nodeNodeEdge1Min then
            temp1 = true
        end

        local nodePointAPSByNodeEdge2 = nodePointA.x * nodeEdge2Normaliz.x + nodePointA.y * nodeEdge2Normaliz.y
        local nodePointBPSByNodeEdge2 = nodePointB.x * nodeEdge2Normaliz.x + nodePointB.y * nodeEdge2Normaliz.y
        local nodePointCPSByNodeEdge2 = nodePointC.x * nodeEdge2Normaliz.x + nodePointC.y * nodeEdge2Normaliz.y
        local nodePointDPSByNodeEdge2 = nodePointD.x * nodeEdge2Normaliz.x + nodePointD.y * nodeEdge2Normaliz.y
        local nodeNodeEdge2Max = math.max(nodePointAPSByNodeEdge2,nodePointBPSByNodeEdge2,nodePointCPSByNodeEdge2,nodePointDPSByNodeEdge2)
        local nodeNodeEdge2Min = math.min(nodePointAPSByNodeEdge2,nodePointBPSByNodeEdge2,nodePointCPSByNodeEdge2,nodePointDPSByNodeEdge2)

        local heroPointAPSByNodeEdge2 = tempPoint1.x * nodeEdge2Normaliz.x + tempPoint1.y * nodeEdge2Normaliz.y
        local heroPointBPSByNodeEdge2 = tempPoint2.x * nodeEdge2Normaliz.x + tempPoint2.y * nodeEdge2Normaliz.y
        local heroPointCPSByNodeEdge2 = tempPoint3.x * nodeEdge2Normaliz.x + tempPoint3.y * nodeEdge2Normaliz.y
        local heroPointDPSByNodeEdge2 = tempPoint4.x * nodeEdge2Normaliz.x + tempPoint4.y * nodeEdge2Normaliz.y
        local heroNodeEdge2Max = math.max(heroPointAPSByNodeEdge2,heroPointBPSByNodeEdge2,heroPointCPSByNodeEdge2,heroPointDPSByNodeEdge2)
        local heroNodeEdge2Min = math.min(heroPointAPSByNodeEdge2,heroPointBPSByNodeEdge2,heroPointCPSByNodeEdge2,heroPointDPSByNodeEdge2)
        
        local temp2 = nil
        if heroNodeEdge2Min <= nodeNodeEdge2Max and heroNodeEdge2Max >= nodeNodeEdge2Min then
            temp2 = true
        end



        local nodePointAPSByNodeEdge3 = nodePointA.x * nodeEdge3Normaliz.x + nodePointA.y * nodeEdge3Normaliz.y
        local nodePointBPSByNodeEdge3 = nodePointB.x * nodeEdge3Normaliz.x + nodePointB.y * nodeEdge3Normaliz.y
        local nodePointCPSByNodeEdge3 = nodePointC.x * nodeEdge3Normaliz.x + nodePointC.y * nodeEdge3Normaliz.y
        local nodePointDPSByNodeEdge3 = nodePointD.x * nodeEdge3Normaliz.x + nodePointD.y * nodeEdge3Normaliz.y
        local nodeNodeEdge3Max = math.max(nodePointAPSByNodeEdge3,nodePointBPSByNodeEdge3,nodePointCPSByNodeEdge3,nodePointDPSByNodeEdge3)
        local nodeNodeEdge3Min = math.min(nodePointAPSByNodeEdge3,nodePointBPSByNodeEdge3,nodePointCPSByNodeEdge3,nodePointDPSByNodeEdge3)

        local heroPointAPSByNodeEdge3 = tempPoint1.x * nodeEdge3Normaliz.x + tempPoint1.y * nodeEdge3Normaliz.y
        local heroPointBPSByNodeEdge3 = tempPoint2.x * nodeEdge3Normaliz.x + tempPoint2.y * nodeEdge3Normaliz.y
        local heroPointCPSByNodeEdge3 = tempPoint3.x * nodeEdge3Normaliz.x + tempPoint3.y * nodeEdge3Normaliz.y
        local heroPointDPSByNodeEdge3 = tempPoint4.x * nodeEdge3Normaliz.x + tempPoint4.y * nodeEdge3Normaliz.y
        local heroNodeEdge3Max = math.max(heroPointAPSByNodeEdge3,heroPointBPSByNodeEdge3,heroPointCPSByNodeEdge3,heroPointDPSByNodeEdge3)
        local heroNodeEdge3Min = math.min(heroPointAPSByNodeEdge3,heroPointBPSByNodeEdge3,heroPointCPSByNodeEdge3,heroPointDPSByNodeEdge3)

        local temp3 = nil
        if heroNodeEdge3Min <= nodeNodeEdge3Max and heroNodeEdge3Max >= nodeNodeEdge3Min then
            temp3 = true
        end


        local nodePointAPSByNodeEdge3 = nodePointA.x * nodeEdge4Normaliz.x + nodePointA.y * nodeEdge4Normaliz.y
        local nodePointBPSByNodeEdge3 = nodePointB.x * nodeEdge4Normaliz.x + nodePointB.y * nodeEdge4Normaliz.y
        local nodePointCPSByNodeEdge3 = nodePointC.x * nodeEdge4Normaliz.x + nodePointC.y * nodeEdge4Normaliz.y
        local nodePointDPSByNodeEdge3 = nodePointD.x * nodeEdge4Normaliz.x + nodePointD.y * nodeEdge4Normaliz.y
        local nodeNodeEdge4Max = math.max(nodePointAPSByNodeEdge3,nodePointBPSByNodeEdge3,nodePointCPSByNodeEdge3,nodePointDPSByNodeEdge3)
        local nodeNodeEdge4Min = math.min(nodePointAPSByNodeEdge3,nodePointBPSByNodeEdge3,nodePointCPSByNodeEdge3,nodePointDPSByNodeEdge3)

        local heroPointAPSByNodeEdge4 = tempPoint1.x * nodeEdge4Normaliz.x + tempPoint1.y * nodeEdge4Normaliz.y
        local heroPointBPSByNodeEdge4 = tempPoint2.x * nodeEdge4Normaliz.x + tempPoint2.y * nodeEdge4Normaliz.y
        local heroPointCPSByNodeEdge4 = tempPoint3.x * nodeEdge4Normaliz.x + tempPoint3.y * nodeEdge4Normaliz.y
        local heroPointDPSByNodeEdge4 = tempPoint4.x * nodeEdge4Normaliz.x + tempPoint4.y * nodeEdge4Normaliz.y
        local heroNodeEdge4Max = math.max(heroPointAPSByNodeEdge4,heroPointBPSByNodeEdge4,heroPointCPSByNodeEdge4,heroPointDPSByNodeEdge4)
        local heroNodeEdge4Min = math.min(heroPointAPSByNodeEdge4,heroPointBPSByNodeEdge4,heroPointCPSByNodeEdge4,heroPointDPSByNodeEdge4)
        
        local temp4 = nil
        if heroNodeEdge4Min <= nodeNodeEdge4Max and heroNodeEdge4Max >= nodeNodeEdge4Min then
            temp4 = true
        end
        
        local AAA1 = nodePointA.x * heroEdge1Normalize.x + nodePointA.y * heroEdge1Normalize.y
        local AAB1 = nodePointB.x * heroEdge1Normalize.x + nodePointB.y * heroEdge1Normalize.y
        local AAC1 = nodePointC.x * heroEdge1Normalize.x + nodePointC.y * heroEdge1Normalize.y
        local AAD1 = nodePointD.x * heroEdge1Normalize.x + nodePointD.y * heroEdge1Normalize.y
        local ACMax = math.max(AAA1,AAB1,AAC1,AAD1)
        local ACMin = math.min(AAA1,AAB1,AAC1,AAD1)

        local ABA1 = tempPoint1.x * heroEdge1Normalize.x + tempPoint1.y * heroEdge1Normalize.y
        local ABB1 = tempPoint2.x * heroEdge1Normalize.x + tempPoint2.y * heroEdge1Normalize.y
        local ABC1 = tempPoint3.x * heroEdge1Normalize.x + tempPoint3.y * heroEdge1Normalize.y
        local ABD1 = tempPoint4.x * heroEdge1Normalize.x + tempPoint4.y * heroEdge1Normalize.y
        local ADMax = math.max(ABA1,ABB1,ABC1,ABD1)
        local ADMin = math.min(ABA1,ABB1,ABC1,ABD1)


        local temp5 = nil
        if ADMin <= ACMax and ADMax >= ACMin then
            temp5 = true
        end



        local BAA1 = nodePointA.x * heroEdge2Normalize.x + nodePointA.y * heroEdge2Normalize.y
        local BAB1 = nodePointB.x * heroEdge2Normalize.x + nodePointB.y * heroEdge2Normalize.y
        local BAC1 = nodePointC.x * heroEdge2Normalize.x + nodePointC.y * heroEdge2Normalize.y
        local BAD1 = nodePointD.x * heroEdge2Normalize.x + nodePointD.y * heroEdge2Normalize.y
        local BCMax = math.max(BAA1,BAB1,BAC1,BAD1)
        local BCMin = math.min(BAA1,BAB1,BAC1,BAD1)

        local BBA1 = tempPoint1.x * heroEdge2Normalize.x + tempPoint1.y * heroEdge2Normalize.y
        local BBB1 = tempPoint2.x * heroEdge2Normalize.x + tempPoint2.y * heroEdge2Normalize.y
        local BBC1 = tempPoint3.x * heroEdge2Normalize.x + tempPoint3.y * heroEdge2Normalize.y
        local BBD1 = tempPoint4.x * heroEdge2Normalize.x + tempPoint4.y * heroEdge2Normalize.y
        local BDMax = math.max(BBA1,BBB1,BBC1,BBD1)
        local BDMin = math.min(BBA1,BBB1,BBC1,BBD1)

        local temp6 = nil
        if BDMin <= BCMax and BDMax >= BCMin then
            temp6 = true
        end



        local CAA1 = nodePointA.x * heroEdge3Normalize.x + nodePointA.y * heroEdge3Normalize.y
        local CAB1 = nodePointB.x * heroEdge3Normalize.x + nodePointB.y * heroEdge3Normalize.y
        local CAC1 = nodePointC.x * heroEdge3Normalize.x + nodePointC.y * heroEdge3Normalize.y
        local CAD1 = nodePointD.x * heroEdge3Normalize.x + nodePointD.y * heroEdge3Normalize.y
        local CCMax = math.max(CAA1,CAB1,CAC1,CAD1)
        local CCMin = math.min(CAA1,CAB1,CAC1,CAD1)

        local CBA1 = tempPoint1.x * heroEdge3Normalize.x + tempPoint1.y * heroEdge3Normalize.y
        local CBB1 = tempPoint2.x * heroEdge3Normalize.x + tempPoint2.y * heroEdge3Normalize.y
        local CBC1 = tempPoint3.x * heroEdge3Normalize.x + tempPoint3.y * heroEdge3Normalize.y
        local CBD1 = tempPoint4.x * heroEdge3Normalize.x + tempPoint4.y * heroEdge3Normalize.y
        local CDMax = math.max(CBA1,CBB1,CBC1,CBD1)
        local CDMin = math.min(CBA1,CBB1,CBC1,CBD1)

        local temp7 = nil
        if CDMin <= CCMax and CDMax >= CCMin then
            temp7 = true
        end


        local DAA1 = nodePointA.x * heroEdge4Normalize.x + nodePointA.y * heroEdge4Normalize.y
        local DAB1 = nodePointB.x * heroEdge4Normalize.x + nodePointB.y * heroEdge4Normalize.y
        local DAC1 = nodePointC.x * heroEdge4Normalize.x + nodePointC.y * heroEdge4Normalize.y
        local DAD1 = nodePointD.x * heroEdge4Normalize.x + nodePointD.y * heroEdge4Normalize.y
        local DCMax = math.max(DAA1,DAB1,DAC1,DAD1)
        local DCMin = math.min(DAA1,DAB1,DAC1,DAD1)

        local DBA1 = tempPoint1.x * heroEdge4Normalize.x + tempPoint1.y * heroEdge4Normalize.y
        local DBB1 = tempPoint2.x * heroEdge4Normalize.x + tempPoint2.y * heroEdge4Normalize.y
        local DBC1 = tempPoint3.x * heroEdge4Normalize.x + tempPoint3.y * heroEdge4Normalize.y
        local DBD1 = tempPoint4.x * heroEdge4Normalize.x + tempPoint4.y * heroEdge4Normalize.y
        local DDMax = math.max(DBA1,DBB1,DBC1,DBD1)
        local DDMin = math.min(DBA1,DBB1,DBC1,DBD1)

        local temp8 = nil
        if DDMin <= DCMax and DDMax >= DCMin then
            temp8 = true
        end
        if temp1 and temp2 and temp3 and temp4 and temp5 and temp6 and temp7 and temp8 then
            
            if self.m_nSpeedUp < 0 and psy+10 >= height then
                self.m_bCollisionPillar = false
                self.m_oCurCheckPillar = node
                return true
            else
                if psy < height then
                    self.m_oCurCheckPillar = node
                    self.m_bCollisionPillar = true
                end
                return false
            end
        end
    end
    self.m_bCollisionPillar = false
    return false
end


--查找最近的柱子进行碰撞检测
function MainScene:findNearestpPillar(psx,psy) 
    for i,v in ipairs(self.m_tBigSet) do
        local minX = nil
        local maxX = nil
        local count = #v
        local temp = v[count]
        if temp.minX  then
            minX = temp.minX
        end

        if temp.maxX then
            maxX = temp.maxX
        end

        if psx  <= maxX then -- 找到了需要进行碰撞检测的集合
            self.m_tCheckPillar = v
            return
        end
    end
end

function MainScene:findPoint(node,psx,psy)
    if psx == nil or psy == nil then
        psx = node:getPositionX()
        psy = node:getPositionY()
    end
    local tempPsx = psx 
    local tempPsy = psy 
    
    local nodeSize = node:getContentSize()
    local width = nodeSize.width
    local height = nodeSize.height
    local pointA = {x = tempPsx,y = tempPsy}
    local pointB = {x = tempPsx,y = tempPsy+height}

    local pointC = {x = tempPsx+width,y = tempPsy+height}
    local pointD = {x = tempPsx+width,y = tempPsy}

    return pointA,pointB,pointC,pointD
end

--求出物体自身的四个投影轴
function MainScene:findPrependicular(node,psx,psy)
    local pointA,pointB,pointC,pointD = self:findPoint(node,psx,psy)
    
    local edge1 = nil
    local edge2 = nil
    local edge3 = nil
    local edge4 = nil

    edge1 = {x = pointB.x - pointA.x, y = pointB.y - pointA.y}
    edge2 = {x = pointC.x - pointB.x, y = pointC.y- pointB.y}
    edge3 = {x = pointD.x- pointC.x, y = pointD.y - pointC.y}
    edge4 = {x = pointA.x- pointD.x,y = pointA.y - pointD.y}

    local prependicularA = {x =edge1.y,y = -edge1.x }
    local prependicularB = {x =edge2.y,y = -edge2.x }
    local prependicularC = {x =edge3.y,y = -edge3.x }
    local prependicularD = {x =edge4.y,y = -edge4.x }


    local edge1Magnitude = math.sqrt(prependicularA.x* prependicularA.x + prependicularA.y*prependicularA.y)
    local edge2Magnitude = math.sqrt(prependicularB.x* prependicularB.x + prependicularB.y*prependicularB.y)
    local edge3Magnitude = math.sqrt(prependicularC.x*prependicularC.x + prependicularC.y*prependicularC.y)
    local edge4Magnitude = math.sqrt(prependicularD.x*prependicularD.x + prependicularD.y*prependicularD.y)

    --print("777777 = ",edge1Magnitude,edge2Magnitude,edge3Magnitude,edge4Magnitude)
    
    --投影轴边向量的法向量
    local edge1AxisVector = {x = prependicularA.x / edge1Magnitude, y = prependicularA.y / edge1Magnitude }
    local edge2AxisVector = {x = prependicularB.x / edge2Magnitude, y = prependicularB.y / edge2Magnitude }
    local edge3AxisVector = {x = prependicularC.x / edge3Magnitude, y = prependicularC.y / edge3Magnitude }
    local edge4AxisVector = {x = prependicularD.x / edge4Magnitude, y = prependicularD.y / edge4Magnitude }
    
    return edge1AxisVector,edge2AxisVector,edge3AxisVector ,edge4AxisVector
end




return MainScene
