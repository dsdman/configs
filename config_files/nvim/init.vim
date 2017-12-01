" basic features of vim
colorscheme desert "set colorscheme
set shortmess=I "disable welcome screen
syntax on " enable syntax highlighting
set showmatch "highlight matching () and {} and []
set number "line numbers
highlight LineNr ctermfg=215
set ruler "ruler
set showmode "show mode you are in
set mouse=a "mouse 
set noswapfile "disable swap file
filetype plugin indent on "enable vim's file browser
set showcmd "show keystrokes in normal mode

" searching
set hlsearch "highlight searches
set incsearch "enable increment search

"highlight the line number
hi clear CursorLine
augroup CLClear
    autocmd! ColorScheme * hi clear CursorLine
augroup END
hi CursorLineNR cterm=bold
augroup CLNRSet
    autocmd! ColorScheme * hi CursorLineNR cterm=bold
augroup END

"Function for highlighting column
function! ToggleColorColumn()
if &colorcolumn != '' | set colorcolumn&
else                  | set colorcolumn=80
endif
endfunction

"Press 'L' to toggle highlight column
:nnoremap ; :call ToggleColorColumn()!<CR>
set cursorline
map <C-_> I//<esc>

"map ctrl + s to save
:nnoremap <C-s> :w! <return>

"tabline configuration
set showtabline=2  " 0, 1 or 2; when to use a tab pages line
set tabline=%!MyTabLine()  " custom tab pages line
function MyTabLine()
  let s = '' " complete tabline goes here
  " loop through each tab page
  for t in range(tabpagenr('$'))
    " set highlight for tab number and &modified
    let s .= '%#TabLineSel#'
    " set the tab page number (for mouse clicks)
    let s .= '%' . (t + 1) . 'T'
    " set page number string
    let s .= t + 1 . '.'
    " get buffer names and statuses
    let n = ''  "temp string for buffer names while we loop and check buftype
    let m = 0  " &modified counter
    let bc = len(tabpagebuflist(t + 1))  "counter to avoid last ' '
    " loop through each buffer in a tab
    for b in tabpagebuflist(t + 1)
      " buffer types: quickfix gets a [Q], help gets [H]{base fname}
      " others get 1dir/2dir/3dir/fname shortened to 1/2/3/fname
      if getbufvar( b, "&buftype" ) == 'help'
        let n .= '[H]' . fnamemodify( bufname(b), ':t:s/.txt$//' )
      elseif getbufvar( b, "&buftype" ) == 'quickfix'
        let n .= '[Q]'
      else
        let n .= pathshorten(bufname(b))
      endif
      " check and ++ tab's &modified count
      if getbufvar( b, "&modified" )
        let m += 1
      endif
      " no final ' ' added...formatting looks better done later
      if bc > 1
        let n .= ' '
      endif
      let bc -= 1
    endfor
    " add modified label [n+] where n pages in tab are modified
    if m > 0
      let s .= '[' . m . '+]'
    endif
    " select the highlighting for the buffer names
    " my default highlighting only underlines the active tab
    " buffer names.
    if t + 1 == tabpagenr()
      let s .= '%#TabLine#'
    else
      let s .= '%#TabLineSel#'
    endif
    " add buffer names
    let s .= n
    " switch to no underlining and add final space to buffer list
    let s .= '%#TabLineSel#' . ' '
  endfor
  " after the last tab fill with TabLineFill and reset tab page nr
  let s .= '%#TabLineFill#%T'
  " right-align the label to close the current tab page
  if tabpagenr('$') > 1
    let s .= '%=%#TabLineFill#%999Xclose'
  endif
  return s
endfunction"if exists("+showtabline")

" tabs and formatting
set tabstop=2 "make two columns the standard tab
set shiftwidth=2 "indent 2
set autoindent "Automatically indent
set softtabstop=2 "makes it work for backspace and delete
set expandtab "use spaces 
set backspace=indent,eol,start "make backspace complete

" tab completion
set complete=k "Global autocomplete
set completeopt+=longest "show complete tab completions
set path +=** " tab-completion for everything
set wildmenu "display all matches when completing

" menu customization
hi! Pmenu ctermfg=black ctermbg=darkgrey 
set completeopt-=preview "disable scratch preview window

"Neovim specific options
if has('nvim')
  "plugins
  call plug#begin('~/.local/share/nvim/plugged')
  Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
  Plug 'zchee/deoplete-clang', { 'do': ':UpdateRemotePlugins' }
  Plug 'plasticboy/vim-markdown'
  "Plug 'vim-airline/vim-airline' OVERKILL, DEFAULT RULER WORKS WITH TABLINE FIXES ABOVE
  "Plug 'majutsushi/tagbar'
  "Plug 'vim-airline/vim-airline-themes'
  call plug#end()

  "deocomplete
  call deoplete#enable()
  let g:deoplete#sources#clang#libclang_path = "/usr/lib/libclang.so"
  let g:deoplete#sources#clang#clang_header = "/usr/lib/clang"

  "airline
  "set guifont=Monaco\ for\ Powerline "make sure to escape spacess
  "let g:airline_powerline_fonts = 1
  "let g:airline_symbols_ascii = 1
  "let g:airline#extensions#tabline#enabled = 1 "smarter tabs with airline
  "let g:airline#extensions#tabline#show_tab_nr = 1
  "let g:airline#extensions#tabline#tab_nr_type = 1
  "let g:airline#extensions#tabline#tabs_label = 't'
  "let g:airline_theme='distinguished'

  "terminal shortcuts
  :tnoremap <C-t> <C-\><C-n> 
  :nnoremap <F5> :vsplit term://zsh <return>A
endif
