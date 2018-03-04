-- load standard vis module, providing parts of the Lua API
require('vis')

vis.events.subscribe(vis.events.INIT, function()
  -- Your global configuration options
end)

vis.events.subscribe(vis.events.WIN_OPEN, function(win)
  -- Basic Vis options (theme, line numbers, etc)
   vis:command('set theme solarized-black')
   vis:command('set number')
   vis:command('set relativenumber')

  -- Formatting (tabs, spaces,etc)
   vis:command('set ai on')
   vis:command('set expandtab')
   vis:command('ai on')
   vis:command('set tw 2')

   -- Status line config
   --Window:status(thing[, thing]) 
   
end)
