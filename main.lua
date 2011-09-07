function lapis.charset_loader_load(name, x, y)
   local function cs_build (x, y)
      local
         CS_X_SIZE, CS_Y_SIZE, CS_X_GAP, CS_Y_GAP, CS_OFFSET_X, CS_OFFSET_Y = 22, 22, 20, 7, 34, 34
      
      return
         CS_OFFSET_X + (CS_X_SIZE + CS_X_GAP) * x,
         CS_OFFSET_Y + (CS_Y_SIZE + CS_Y_GAP) * y,
         CS_X_SIZE,
         CS_Y_SIZE     
   end

   lapis.image_loader_load(name, "charset.png", cs_build(x,y))
end

lapis.charset_loader_load("player", 2, 5)
lapis.charset_loader_load("wall", 3, 5)
lapis.charset_loader_load("zombie1", 1, 4)
lapis.charset_loader_load("zombie2", 2, 4)
lapis.charset_loader_load("zombie3", 3, 4)
lapis.charset_loader_load("zombie4", 4, 4)
lapis.charset_loader_load("zombie5", 5, 4)
lapis.charset_loader_load("bullet1", 6, 6)
lapis.charset_loader_load("round", 4, 1)
lapis.charset_loader_load("light", 11, 2)

