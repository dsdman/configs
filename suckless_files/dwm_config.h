/* See LICENSE file for copyright and license details. */
/* including x11 keysym for volume/brightness keys */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_white[]       = "#ffffff";
static const char col_black[]       = "#000000";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_gray5[]       = "#383c4a";
static const char col_cyan[]        = "#005577";
static const char col_neon[]        = "#39ff14";
static const char col_yellow[]      = "#FFCC00";
static const char col_yellow2[]     = "#FFA726";
static const char col_auora[]       = "#8E97B4";
static const char col_tan[]         = "#D2B48C";
static const char col_crimson[]     = "#DC143C";
static const char col_mint[]        = "#3EB489";
static const char col_blue[]        = "#506991";
static const char col_navyblue[]    = "#000080";
static const char col_purple[]      = "#6600CC";
static const char col_purple2[]     = "#9e68a6";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
  /* Current theme */
  [SchemeNorm] = { col_gray3, col_black, col_gray2 },
	[SchemeSel]  = { col_mint, col_black, col_mint },

  /* Dark Theme 1 */
  /*
  [SchemeNorm] = { col_gray3, col_black, col_gray2 },
	[SchemeSel]  = { col_auora, col_black, col_navyblue },*/
  /* DARK THEME 2 */
 /* 
	[SchemeNorm] = { col_gray3, col_black, col_gray1 },
	[SchemeSel]  = { col_tan, col_black, col_purple2 }, */
  /* DARK THEME 3 */
 /* 
	[SchemeNorm] = { col_gray3, col_black, col_gray2 },
	[SchemeSel]  = { col_auora, col_black, col_mint }, */
  /* DARK THEME 4 */
 /* 
	[SchemeNorm] = { col_gray3, col_black, col_gray1 },
	[SchemeSel]  = { col_mint, col_black, col_mint }, */
  /* LIGHT THEME 1 */
/*	
  [SchemeNorm] = { col_black, col_white, col_gray2 },
	[SchemeSel]  = { col_black, col_crimson, col_orange }, */
  /* DEFAULT THEME */
  /*
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  }, */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,       NULL,       0,            1,           -1 },
	{ "SpeedCrunch",        NULL,       NULL,       0,            1,           -1 },
	{ "xterm-256color",     NULL,       "scratch",  0,            1,           -1 },
};

/* layout(s) */
#include "gaplessgrid.c"
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 2;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[G]",      gaplessgrid },
	{ "[B]",      bstack },
	{ "[H]",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_gray3, "-sb", col_black, "-sf", col_mint, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *sttmux[]  = { "st-tmux", NULL };
static const char *browser[]  = { "firefox", NULL };
static const char *browser3[]  = { "chromium", NULL };
static const char *editor[]  = { "st", "-e", "nvim", NULL };
static const char *sys_mon[]  = { "st", "-e", "htop", NULL };
static const char *display_setup[]  = {"st", "-e", "python", "/home/dylan/apps/src/dual-monitor-setup/dual-monitor-setup.py", NULL };
static const char *calc[]  = { "speedcrunch", NULL };
static const char *lockscreen[]  = { "slock", NULL };
static const char *filebrowser[]  = { "st", "-e", "ranger", NULL };
static const char *screenshot[]  = { "scrot",  NULL };
static const char *poweroff[] = { "systemctl", "poweroff", NULL};
static const char *suspend[] = { "systemctl", "suspend", NULL};
static const char *reboot[] = { "systemctl", "reboot", NULL};
static const char *upvol[]   = { "amixer", "-D", "pulse", "sset", "Master", "5%+", NULL };
static const char *downvol[] = { "amixer", "-D", "pulse", "sset", "Master", "5%-", NULL };
static const char *mutevol[] = { "amixer", "-D", "pulse", "sset", "Master", "mute", NULL };
static const char *upbacklight[] = { "xbacklight", "-inc", "10", NULL };
static const char *downbacklight[] = { "xbacklight", "-dec", "10", NULL };
static const char *scratch[] = { "st", "-g", "146x26+150+225", "-t", "scratch", NULL };
#include "push.c"
#include "moveresize.c"
#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 75y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_j,      moveresize,     {.v = "0x 75y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -75y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_k,      moveresize,     {.v = "0x -75y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "75x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_l,      moveresize,     {.v = "75x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-75x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_h,      moveresize,     {.v = "-75x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 75h" } },
	{ MODKEY|ControlMask,           XK_j,      moveresize,     {.v = "0x 0y 0w 75h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -75h" } },
	{ MODKEY|ControlMask,           XK_k,      moveresize,     {.v = "0x 0y 0w -75h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 75w 0h" } },
	{ MODKEY|ControlMask,           XK_l,      moveresize,     {.v = "0x 0y 75w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -75w 0h" } },
	{ MODKEY|ControlMask,           XK_h,      moveresize,     {.v = "0x 0y -75w 0h" } },
  { MODKEY,             XK_bracketleft,      pushup,         {.i = -1 } },
  { MODKEY,            XK_bracketright,      pushdown,       {.i = +1 } },
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_o,      winview,        {0} },
 	{ MODKEY,                       XK_y,      spawn,          {.v = editor } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filebrowser } },
	{ MODKEY,                       XK_w,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = browser3 } },
 	{ MODKEY,                       XK_c,      spawn,          {.v = calc } },
 	{ MODKEY,                       XK_p,      spawn,          {.v = display_setup } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
 	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = scratch } },
 	{ MODKEY|ControlMask|ShiftMask, XK_Return, spawn,          {.v = sttmux } },
	{ MODKEY,                       XK_Escape, spawn,          {.v = sys_mon } },
 	{ MODKEY|ControlMask|ShiftMask, XK_l,      spawn,          {.v = lockscreen } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = poweroff} },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = suspend} },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = reboot} },
 	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = downvol} },
 	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = upvol} },
 	{ 0,                            XK_F12,    spawn,          {.v = screenshot} },
 	{ 0,              XF86XK_AudioMute,        spawn,          {.v = mutevol} },
 	{ 0,              XF86XK_MonBrightnessUp,  spawn,          {.v = upbacklight} },
 	{ 0,            XF86XK_MonBrightnessDown,  spawn,          {.v = downbacklight} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_equal,  shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_minus,  shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_z,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { MODKEY|ControlMask,           XK_q,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,    MODKEY|ShiftMask,    Button1,        killclient,     {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
