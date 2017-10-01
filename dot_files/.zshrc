# Basic options for history, corrections, completion, etc
HISTFILE=~/.histfile
HISTSIZE=1000
SAVEHIST=1000
setopt correct
setopt appendhistory autocd beep extendedglob nomatch
bindkey -e
zstyle :compinstall filename '/home/dylan/.zshrc'
source /usr/share/zsh/share/antigen.zsh
plugins=zsh-completions
fpath=(usr/share/zsh/site-functions/ $fpath)
autoload -U compinit && compinit
zstyle ':completion:*' menu select
compinit
zstyle ':completion:*' list-colors 'di=34:ln=35:so=32:pi=33:ex=31:bd=46;34:cd=43;34:su=41;30:sg=46;30:tw=42;30:ow=43;30'

# enable vi mode
set -o vi

#prompt
autoload -Uz colors && colors
#PROMPT="%n@%m %d%# "
PROMPT="%n@%m:%~%#"
#RPROMPT="[%[%n]%t]"
RPROMPT="%[%n]%t"

#aliases
alias ..='cd ..'           
alias ...='cd ../..'       
alias ....='cd ../../..'   
alias l='ls -la'
alias ls='ls --color=tty'
alias grep='grep --color=auto'
alias vi='nvim'
alias vim='nvim'
alias steam-wine='WINEDEBUG=-all wine /mnt/windows/games/PC/Steam/Steam.exe >/dev/null 2>&1 &'
alias pico='nano'
alias ssh-cse-usc='ssh -l ddesai -p 222 L-1D39-04.cse.sc.edu'
alias ipconfig='ifconfig'
alias psa='ps -auxf'
alias eclimd='/home/dylan/apps/eclipse/eclimd'
alias v='nvim'
alias blkren='\ls | vim -'

# Various variables to set
export CFLAGS="-march=native -O3 -pipe -fomit-frame-pointer"
export CXXFLAGS="${CFLAGS}"
export PREFIX=/usr
export EDITOR=/usr/bin/nvim
export WM=enlightenment
