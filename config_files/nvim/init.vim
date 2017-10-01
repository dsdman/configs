"vim compatibility
set runtimepath+=~/.vim,~/.vim/after
set packpath+=~/.vim
source ~/.vimrc 
set encoding=utf-8

"plugins
call plug#begin('~/.local/share/nvim/plugged')
Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
Plug 'zchee/deoplete-clang', { 'do': ':UpdateRemotePlugins' }
Plug 'Shougo/denite.nvim', { 'do': ':UpdateRemotePlugins' }
Plug 'vim-airline/vim-airline', { 'do': ':UpdateRemotePlugins' }
Plug 'majutsushi/tagbar', { 'do': ':UpdateRemotePlugins' }
Plug 'vim-airline/vim-airline-themes', { 'do': ':UpdateRemotePlugins' }
call plug#end()

"enable deocomplete
call deoplete#enable()

"make deocomplete-clang work right
let g:deoplete#sources#clang#libclang_path = "/usr/lib/libclang.so"
let g:deoplete#sources#clang#clang_header = "/usr/lib/clang"

"airline
set guifont=Monaco\ for\ Powerline "make sure to escape spacess
let g:airline_powerline_fonts = 1
"let g:airline_symbols_ascii = 1
let g:airline#extensions#tabline#enabled = 1 "smarter tabs with airline
let g:airline#extensions#tabline#show_tab_nr = 1
let g:airline#extensions#tabline#tab_nr_type = 1
let g:airline#extensions#tabline#tabs_label = 't'
let g:airline_theme='minimalist'
