local lapisext = {}

function lapisext.charset_loader_load(name, x, y)
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

return lapisext