/* modifier 0 means no modifier */
static int surfuseragent    = 0;  /* Append Surf version to default WebKit user agent */
static char *fulluseragent  = ""; /* Or override the whole user agent string */
static char *scriptfile     = "~/.surf/script.js";
static char *styledir       = "~/.surf/styles/";
static char *certdir        = "~/.surf/certificates/";
static char *cachedir       = "~/.surf/cache/";
static char *cookiefile     = "~/.surf/cookies.txt";

/* Webkit default features */
/* Highest priority value will be used.
 * Default parameters are priority 0
 * Per-uri parameters are priority 1
 * Command parameters are priority 2
 */
static Parameter defconfig[ParameterLast] = {
	/* parameter                    Arg value       priority */
	[AcceleratedCanvas]   =       { { .i = 1 },     },
	[AccessMicrophone]    =       { { .i = 0 },     },
	[AccessWebcam]        =       { { .i = 0 },     },
	[Certificate]         =       { { .i = 0 },     },
	[CaretBrowsing]       =       { { .i = 0 },     },
	[CookiePolicies]      =       { { .v = "@Aa" }, },
	[DefaultCharset]      =       { { .v = "UTF-8" }, },
	[DiskCache]           =       { { .i = 1 },     },
	[DNSPrefetch]         =       { { .i = 0 },     },
	[FileURLsCrossAccess] =       { { .i = 0 },     },
	[FontSize]            =       { { .i = 16 },    },
	[FrameFlattening]     =       { { .i = 0 },     },
	[Geolocation]         =       { { .i = 0 },     },
	[HideBackground]      =       { { .i = 0 },     },
	[Inspector]           =       { { .i = 1 },     },
	[Java]                =       { { .i = 1 },     },
	[JavaScript]          =       { { .i = 1 },     },
	[KioskMode]           =       { { .i = 0 },     },
	[LoadImages]          =       { { .i = 1 },     },
	[MediaManualPlay]     =       { { .i = 1 },     },
	[Plugins]             =       { { .i = 1 },     },
	[PreferredLanguages]  =       { { .v = (char *[]){ NULL } }, },
	[RunInFullscreen]     =       { { .i = 1 },     },
	[ScrollBars]          =       { { .i = 1 },     },
	[ShowIndicators]      =       { { .i = 1 },     },
	[SiteQuirks]          =       { { .i = 1 },     },
	[SmoothScrolling]     =       { { .i = 1 },     },
	[SpellChecking]       =       { { .i = 1 },     },
	[SpellLanguages]      =       { { .v = ((char *[]){ "en_US", NULL }) }, },
	[StrictTLS]           =       { { .i = 1 },     },
	[Style]               =       { { .i = 1 },     },
	[WebGL]               =       { { .i = 1 },     },
	[ZoomLevel]           =       { { .f = 1.0 },   },
};

static UriParameters uriparams[] = {
	{ "", {
	  [JavaScript] = { { .i = 1 }, 1 },
	  [Plugins]    = { { .i = 1 }, 1 },
	}, },
};

/* default window size: width, height */
static int winsize[] = { 800, 600 };

static WebKitFindOptions findopts = WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                    WEBKIT_FIND_OPTIONS_WRAP_AROUND;

#define PROMPT_GO   "Go:"
#define PROMPT_FIND "Find:"

/* SETPROP(readprop, setprop, prompt)*/
#define SETPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "prop=\"$(printf '%b' \"$(xprop -id $1 $2 " \
             "| sed \"s/^$2(STRING) = //;s/^\\\"\\(.*\\)\\\"$/\\1/\")\" " \
             "| dmenu -p \"$4\" -w $1)\" && xprop -id $1 -f $3 8s -set $3 \"$prop\"", \
             "surf-setprop", winid, r, s, p, NULL \
        } \
}

/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(u, r) { \
        .v = (const char *[]){ "st", "-e", "/bin/sh", "-c",\
             "curl -g -L -J -O -A \"$1\" -b \"$2\" -c \"$2\"" \
             " -e \"$3\" \"$4\"; read", \
             "surf-download", useragent, cookiefile, r, u, NULL \
        } \
}

/* PLUMB(URI) */
/* This called when some URI which does not begin with "about:",
 * "http://" or "https://" should be opened.
 */
#define PLUMB(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xdg-open \"$0\"", u, NULL \
        } \
}

/* VIDEOPLAY(URI) */
#define VIDEOPLAY(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mpv --really-quiet \"$0\"", u, NULL \
        } \
}

#define WATCH {.v = (char *[]){ "/bin/sh", "-c", \
	"st -e \
	surf_audio.sh $(xprop -id $0 _SURF_URI | cut -d \\\" -f 2)", \
	winid, NULL } \ 
}

/* styles */
/*
 * The iteration will stop at the first match, beginning at the beginning of
 * the list.
 */
static SiteSpecific styles[] = {
	/* regexp                            file in $styledir */
	{ ".*",                              "default.css" }
};

/* certificates */
/*
 * Provide custom certificate for urls
 */
static SiteSpecific certs[] = {
	/* regexp               file in $certdir */
	{ "://suckless\\.org/", "suckless.org.crt" },
};

static SearchEngine searchengines[] = {
	{ "d",   "https://duckduckgo.com/?q=%s" },
	{ "g",   "http://www.google.com/search?q=%s" },
	{ "gh",  "https://github.com/search?q=%s" },
	{ "u",   "https://www.urbandictionary.com/define.php?term=%s" },
	{ "w",   "https://en.wikipedia.org/w/index.php?search=%s" },
	{ "y",   "https://www.youtube.com/results?search_query=%s" }

};
static char *linkselect_curwin [] = { "/bin/sh", "-c",
	"surf_linkselect.sh $0 'Link' | xargs -r xprop -id $0 -f _SURF_GO 8s -set _SURF_GO",
	winid, NULL
};
static char *linkselect_newwin [] = { "/bin/sh", "-c",
	"surf_linkselect.sh $0 'Link (new window)' | xargs -r surf",
	winid, NULL
};
static char *editscreen[] = { "/bin/sh", "-c", "edit_screen.sh", NULL };
static char *test[] = { "/bin/sh", "-c", "surf_test.sh", NULL };

#define CLEAR { .v = (char *[]) { "/bin/sh", "-c", "surf_clear.sh", NULL } }
#define BM_PICK { .v = (char *[]){ "/bin/sh", "-c", \
"xprop -id $0 -f _SURF_GO 8s -set _SURF_GO \
`cat ~/.surf/bookmarks | dmenu || exit 0`", \
winid, NULL } }
#define BM_ADD { .v = (char *[]){ "/bin/sh", "-c", \
"(echo `xprop -id $0 _SURF_URI | cut -d '\"' -f 2` && \
cat ~/.surf/bookmarks) | sort -u > ~/.surf/bookmarks_new && \
mv ~/.surf/bookmarks_new ~/.surf/bookmarks", \
winid, NULL } }

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
	/* modifier              keyval          function    arg */
	{ 0,                     GDK_KEY_g,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ 0,                     GDK_KEY_slash,  spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ 0,                     GDK_KEY_i,      insert,     { .i = 1 } },
	{ 0,                     GDK_KEY_Escape, insert,     { .i = 0 } },

	{ MODKEY,                GDK_KEY_r,      reload,     { .i = 1 } },
	{ 0,                     GDK_KEY_r,      reload,     { .i = 0 } },

	{ 0,                     GDK_KEY_l,      navigate,   { .i = +1 } },
	{ 0,                     GDK_KEY_h,      navigate,   { .i = -1 } },

	/* vertical and horizontal scrolling, in viewport percentage */
	{ 0,                     GDK_KEY_j,      scrollv,    { .i = +10 } },
	{ 0,                     GDK_KEY_k,      scrollv,    { .i = -10 } },
	{ 0,                     GDK_KEY_space,  scrollv,    { .i = +50 } },
	{ 0,                     GDK_KEY_b,      scrollv,    { .i = -50 } },
	{ 0,                     GDK_KEY_i,      scrollh,    { .i = +10 } },
	{ 0,                     GDK_KEY_u,      scrollh,    { .i = -10 } },


	{ 0,                     GDK_KEY_0,      zoom,       { .i = 0  } },
	{ 0,                     GDK_KEY_minus,  zoom,       { .i = -1 } },
	{ 0,                     GDK_KEY_equal,  zoom,       { .i = +1 } },

	{ 0,                     GDK_KEY_p,      clipboard,  { .i = 1 } },
	{ 0,                     GDK_KEY_y,      clipboard,  { .i = 0 } },

	{ 0,                     GDK_KEY_n,      find,       { .i = +1 } },
	{ 0|GDK_SHIFT_MASK,      GDK_KEY_n,      find,       { .i = -1 } },

	{ MODKEY,                GDK_KEY_p,      print,      { 0 } },
	{ MODKEY,                GDK_KEY_m,      showcert,   { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_a,      togglecookiepolicy, { 0 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_i,      toggleinspector, { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_c,      toggle,     { .i = CaretBrowsing } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_g,      toggle,     { .i = Geolocation } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_s,      toggle,     { .i = JavaScript } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_semicolon,      toggle,     { .i = LoadImages } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_v,      toggle,     { .i = Plugins } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_b,      toggle,     { .i = ScrollBars } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_t,      toggle,     { .i = StrictTLS } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_m,      toggle,     { .i = Style } },
  { 0,                     GDK_KEY_z,      loaduri,    { .v = "duckduckgo.com" } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_Delete, spawn,      CLEAR },
	{ 0,                     GDK_KEY_q,      spawn,      CLEAR },
  { MODKEY,                GDK_KEY_d,      spawn,      BM_ADD },
  { 0,                     GDK_KEY_d,      spawn,      BM_PICK },
	{ 0,                     GDK_KEY_v,      playexternal, { 0 } },
	{ 0,                     GDK_KEY_a,      spawn,      WATCH },
  { 0,                     GDK_KEY_f,      externalpipe, { .v = linkselect_curwin } },
	{ 0|GDK_SHIFT_MASK,      GDK_KEY_f,      externalpipe, { .v = linkselect_newwin } },
	{ 0,                     GDK_KEY_o,      externalpipe, { .v = editscreen        } },
};

/* button definitions */
/* target can be OnDoc, OnLink, OnImg, OnMedia, OnEdit, OnBar, OnSel, OnAny */
static Button buttons[] = {
	/* target       event mask      button  function        argument        stop event */
	{ OnDoc,        MODKEY,         4,      zoom,           { .i = +1 },    1 },
	{ OnDoc,        MODKEY,         5,      zoom,           { .i = -1 },    1 },
	{ OnLink,       0,              2,      clicknewwindow, { .i = 0 },     1 },
	{ OnLink,       MODKEY,         2,      clicknewwindow, { .i = 1 },     1 },
	{ OnLink,       MODKEY,         1,      clicknewwindow, { .i = 1 },     1 },
	{ OnAny,        0,              8,      clicknavigate,  { .i = -1 },    1 },
	{ OnAny,        0,              9,      clicknavigate,  { .i = +1 },    1 },
	{ OnMedia,      MODKEY,         1,      clickexternplayer, { 0 },       1 },
};
