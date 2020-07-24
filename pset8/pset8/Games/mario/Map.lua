--[[
    Contains tile data and necessary code for rendering a tile map to the
    screen.
]]

require 'Util'

Map = Class{}

TILE_BRICK = 1
TILE_EMPTY = -1

-- cloud tiles
CLOUD_LEFT = 6
CLOUD_RIGHT = 7

-- bush tiles
BUSH_LEFT = 2
BUSH_RIGHT = 3

-- mushroom tiles
MUSHROOM_TOP = 10
MUSHROOM_BOTTOM = 11

-- jump block
JUMP_BLOCK = 5
JUMP_BLOCK_HIT = 9

FLAG_BOTTOM = 16
FLAG_POLE = 12
FLAG_TOP = 8
FLAG_UP = 14
FLAG_DOWN = 13
FLAG_FALLING = 15

-- a speed to multiply delta time to scroll map; smooth value
local SCROLL_SPEED = 62

local timer = love.timer.getTime()
flagTimer = 0
flagLoc = 10

-- constructor for our map object
function Map:init()

    self.spritesheet = love.graphics.newImage('graphics/spritesheet.png')
    self.sprites = generateQuads(self.spritesheet, 16, 16)
    self.music = love.audio.newSource('sounds/music.wav', 'static')

    self.tileWidth = 16
    self.tileHeight = 16
    self.mapWidth = 200
    self.mapHeight = 28
    self.tiles = {}

    -- applies positive Y influence on anything affected
    self.gravity = 15

    -- associate player with map
    self.player = Player(self)

    -- camera offsets
    self.camX = 0
    self.camY = -3

    -- cache width and height of map in pixels
    self.mapWidthPixels = self.mapWidth * self.tileWidth
    self.mapHeightPixels = self.mapHeight * self.tileHeight

    -- first, fill map with empty tiles
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            
            -- support for multiple sheets per tile; storing tiles as tables 
            self:setTile(x, y, TILE_EMPTY)
        end
    end

    -- begin generating the terrain using vertical scan lines
    local x = 1
    while x < self.mapWidth do
        
        -- 2% chance to generate a cloud
        -- make sure we're 2 tiles from edge at least
        if x < self.mapWidth - 2 then
            if math.random(20) == 1 then
                
                -- choose a random vertical spot above where blocks/pipes generate
                local cloudStart = math.random(self.mapHeight / 2 - 6)

                self:setTile(x, cloudStart, CLOUD_LEFT)
                self:setTile(x + 1, cloudStart, CLOUD_RIGHT)
            end
        end

        -- 5% chance to generate a mushroom
        if math.random(20) == 1 and x < self.mapWidth - 20 then
            -- left side of pipe
            self:setTile(x, self.mapHeight / 2 - 2, MUSHROOM_TOP)
            self:setTile(x, self.mapHeight / 2 - 1, MUSHROOM_BOTTOM)

            -- creates column of tiles going to bottom of map
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

            -- next vertical scan line
            x = x + 1

        -- 10% chance to generate bush, being sure to generate away from edge
        elseif math.random(10) == 1 and x < self.mapWidth - 3 then
            local bushLevel = self.mapHeight / 2 - 1

            -- place bush component and then column of bricks
            self:setTile(x, bushLevel, BUSH_LEFT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            x = x + 1

            self:setTile(x, bushLevel, BUSH_RIGHT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            x = x + 1

        -- 10% chance to not generate anything, creating a gap
        elseif math.random(10) ~= 1 then
            
            -- creates column of tiles going to bottom of map
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

            -- chance to create a block for Mario to hit
            if math.random(15) == 1 and x < self.mapWidth - 20 then
                self:setTile(x, self.mapHeight / 2 - 4, JUMP_BLOCK)
            end


            -- next vertical scan line
            x = x + 1

        elseif x >= self.mapWidth - 20 then
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

        else
            -- increment X so we skip two scanlines, creating a 2-tile gap
            x = x + 2
        end

        -- make pyramid
        for i = 1, 10 do
            for j = i, 10 do
                self:setTile(self.mapWidth - 10 - i, self.mapHeight / 2 - (10 - j), TILE_BRICK)
            end
        end

        -- make flagpole
        self:setTile(self.mapWidth - 5, self.mapHeight / 2 - 1, FLAG_BOTTOM)
        for j = 2, 9 do
            self:setTile(self.mapWidth - 5, self.mapHeight / 2 - j, FLAG_POLE)
        end
        self:setTile(self.mapWidth - 5, self.mapHeight / 2 - 10, FLAG_TOP)
        self:setTile(self.mapWidth - 4, self.mapHeight / 2 - 10, FLAG_DOWN) 

        -- check for half-bushes near flagpole
        if self:getTile(self.mapWidth - 5, self.mapHeight / 2 - 11) == BUSH_LEFT then
            self:setTile(self.mapWidth - 5, self.mapHeight / 2 - 11, TILE_EMPTY)
        end

        if self:getTile(self.mapWidth - 5, self.mapHeight / 2 - 9) == BUSH_RIGHT then
            self:setTile(self.mapWidth - 5, self.mapHeight / 2 - 9, TILE_EMPTY)
        end
    end

    -- start the background music
    self.music:setLooping(true)
    self.music:play()
end

-- return whether a given tile is collidable
function Map:collides(tile)
    -- define our collidable tiles
    local collidables = {
        TILE_BRICK, JUMP_BLOCK, JUMP_BLOCK_HIT,
        MUSHROOM_TOP, MUSHROOM_BOTTOM
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(collidables) do
        if tile.id == v then
            return true
        end
    end

    return false
end

function Map:winner(tile)
    local winBlocks = {
        FLAG_BOTTOM, FLAG_POLE, FLAG_TOP
    }

    for _, v in ipairs(winBlocks) do
        if tile.id == v then
            return true
        end
    end

    return false

end

-- function to update camera offset with delta time
function Map:update(dt)
    self.player:update(dt)
    time = love.timer.getTime()
    if time - timer >= 0.5 and self.player.state ~= 'winner' then
        timer = time
        flag_tile = self:getTile(self.mapWidth - 4, self.mapHeight / 2 - 10)
        if flag_tile == FLAG_DOWN then
            self:setTile(self.mapWidth - 4, self.mapHeight / 2 - 10, FLAG_UP)
        else
            self:setTile(self.mapWidth - 4, self.mapHeight / 2 - 10, FLAG_DOWN)
        end
    end

    if self.player.state == 'winner' and time - flagTimer > 0.5 and flagLoc > 1 then
        self:setTile(self.mapWidth - 4, self.mapHeight / 2 - flagLoc, TILE_EMPTY)
        flagLoc = flagLoc - 1
        flagTimer = time
        self:setTile(self.mapWidth - 4, self.mapHeight / 2 - flagLoc, FLAG_FALLING)
    end

    -- keep camera's X coordinate following the player, preventing camera from
    -- scrolling past 0 to the left and the map's width
    self.camX = math.max(0, math.min(self.player.x - VIRTUAL_WIDTH / 2,
        math.min(self.mapWidthPixels - VIRTUAL_WIDTH, self.player.x)))
end

-- gets the tile type at a given pixel coordinate
function Map:tileAt(x, y)
    return {
        x = math.floor(x / self.tileWidth) + 1,
        y = math.floor(y / self.tileHeight) + 1,
        id = self:getTile(math.floor(x / self.tileWidth) + 1, math.floor(y / self.tileHeight) + 1)
    }
end

-- returns an integer value for the tile at a given x-y coordinate
function Map:getTile(x, y)
    return self.tiles[(y - 1) * self.mapWidth + x]
end

-- sets a tile at a given x-y coordinate to an integer value
function Map:setTile(x, y, id)
    self.tiles[(y - 1) * self.mapWidth + x] = id
end

-- renders our map to the screen, to be called by main's render
function Map:render()
    love.graphics.setDefaultFilter('nearest', 'nearest')

    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            local tile = self:getTile(x, y)
            if tile ~= TILE_EMPTY then
                love.graphics.draw(self.spritesheet, self.sprites[tile],
                    (x - 1) * self.tileWidth, (y - 1) * self.tileHeight)
            end
        end
    end

    if self.player.state == 'lose' then
        love.graphics.setFont(love.graphics.newFont('fonts/font.ttf', 20))
        love.graphics.printf("You lose!", 0, 10, self.player.x, "right")

        love.graphics.setFont(love.graphics.newFont('fonts/font.ttf', 10))
        love.graphics.printf("Press enter to play again!", 0, 40, self.player.x + 15, "right")
    end

    if self.player.state == 'winner' then 
        love.graphics.setColor(1, 211/255, 0, 1)
        love.graphics.setFont(love.graphics.newFont('fonts/font.ttf', 30))
        love.graphics.printf("You win!", 0, 10, self.player.x, "right")

        love.graphics.setColor(1, 1, 1, 1)
        love.graphics.setFont(love.graphics.newFont('fonts/font.ttf', 10))
        love.graphics.printf("Press enter to play again!", 0, 40, self.player.x + 5, "right")
    end

    self.player:render()
end
