local StartScene = class("StartScene", cc.load("mvc").ViewBase)

StartScene.RESOURCE_FILENAME = "StartScene.csb"


function StartScene:onCreate()
    print("StartScene:onCreate")
    
end

function StartScene:onEnter()
    print("StartScene:onEnter ")
    StartScene.m_root = self:getResourceNode()
    StartScene:initUI()
end

function StartScene:onEnterTransitionDidFinish()
    print("StartScene:onEnterTransitionDidFinish")
end


function StartScene:initUI()
	print("StartScene:initUI")
	local openglView = cc.Director:getInstance():getOpenGLView()
    local frameSize = openglView:getFrameSize()
    self.m_nframeSizeWidth = frameSize.width
    self.m_nframeSizeHeight = frameSize.height
    self.m_nopenglScaleX = openglView:getScaleX()
    self.m_nopenglScaleY = openglView:getScaleY()

    local mainContainer = self.m_root:getChildByName("MainContainer")
    local conStartGame = mainContainer:getChildByName("conStartGame_StartScene")
    conStartGame:setScaleX(1/self.m_nopenglScaleX)
    conStartGame:setScaleY(1/self.m_nopenglScaleY)

    local conGameOver = mainContainer:getChildByName("conGameOver_StartScene")
    conGameOver:setScaleX(1/self.m_nopenglScaleX)
    conGameOver:setScaleY(1/self.m_nopenglScaleY)

    local conLevelList = mainContainer:getChildByName("conLevelList_StartScene")

    if GAME_OVER then
    	conGameOver:setVisible(true)
    	conStartGame:setVisible(false)
    end

    local function onClickLevel(sender)
    	print("onClickLevel = ",sender:getTag())
    	local index = sender:getTag() - 100 +1
    	if index <= LEVEL_COUNT then
    		CURRENT = index
    		require("app.MyApp"):create():run("MainScene")
    	end
    end

    local function onClickSelectLevel(sender) --重新开始游戏
    	print("onClickSelectLevel")
    	conStartGame:setVisible(false)
    	conGameOver:setVisible(false)
    	conLevelList:setVisible(true)
    end

    local function onClickRestartGame(sender)
    	print("onClickRestartGame")
    	require("app.MyApp"):create():run("MainScene")
    	
    end

    local btnSelectLevel = conGameOver:getChildByName("btnSelectLevel_StartScene")
    btnSelectLevel =tolua.cast(btnSelectLevel, "ccui.Button")
    btnSelectLevel:addClickEventListener(onClickSelectLevel)

    local btnRestartGame = conGameOver:getChildByName("btnRestartGame_StartScene")
    btnRestartGame =tolua.cast(btnRestartGame, "ccui.Button")
    btnRestartGame:addClickEventListener(onClickRestartGame)
    
    for i=1,12 do
    	local btnLevel = conLevelList:getChildByName("btnLevel" .. i )
    	btnLevel:setScaleX(1/self.m_nopenglScaleX)
    	btnLevel:setScaleY(1/self.m_nopenglScaleY)
    	btnLevel:addClickEventListener(onClickLevel)
    end

    local btnStartGame = conStartGame:getChildByName("btnStartGame_StartScene")
    btnStartGame =tolua.cast(btnStartGame, "ccui.Button")
    btnStartGame:addClickEventListener(onClickSelectLevel)  --开始游戏

    GAME_OVER = false
end

return StartScene