/* See LICENSE file for copyright and license details. */

/* Patches */
#include <X11/XF86keysym.h>

/* Define the terminal of your choice here */
#define TERM "st"

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 6;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 0;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh           = 32;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Adobe Courier Regular:size=14", "Symbols Nerd Font:size=12" };
static const char dmenufont[]       = "Adobe Courier Regular:size=14";
static const char col_gray1[]       = "#121212";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#dfdfdf";
static const char col_gray4[]       = "#ffdfdf";
static const char col_cyan[]        = "#e60053";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* Multimedia Keys */
static const char *upvol[]   = { "/usr/bin/pamixer", "--sink", "0", "-i", "5",     NULL };
static const char *downvol[] = { "/usr/bin/pamixer", "--sink", "0", "-d", "5",     NULL };
static const char *mutevol[] = { "/usr/bin/pamixer", "--sink",   "0", "-t",  NULL };

/* tagging */
static const char *tags[] = { "", "", "", "切", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     					instance  		title       		tags mask  		isfloating  	isterminal  noswallow  monitor */
	{ "St",    						NULL,     		NULL,       		0,         		0,          	1,						0,        -1	},
	{ "Alacritty",    		NULL,     		NULL,       		0,         		0,          	1,						0,        -1	},
	/* Web browsers are opened at tag	5 */
	{ NULL,  							"Navigator",	NULL,      			1 << 4,       0,           	0, 						1,				-1	},
	{ "Luakit",  					NULL,					NULL,      			1 << 4,       0,           	0, 						1,				-1	},
	{ NULL,  							"Toolkit", 		NULL,      			0,       			1,           	0, 						1,				-1	},
	/* xev */
	{ NULL,  							NULL, 				"Event Tester", 0,       			0,           	0, 						0,				-1	},
	{ "TelegramDesktop",  NULL,       	NULL,       		1 << 3,       1,           	0, 						0,				-1	},
	{ "lightcord",  			NULL,       	NULL,       		1 << 3,       0,      		 	0, 						0,				-1	},
	{ "Steam",  					NULL,       	NULL,       		1 << 2,       1,      		 	0, 						0,				-1	},
	{ "obs",  						NULL,       	NULL,       		1 << 2,       1,      		 	0, 						1,				-1	},
	{ "Sxiv",  						NULL,       	NULL,       		0,       			1,      		 	0, 						0,				-1	},
	{ "mpv",  						NULL,       	NULL,       		0,       			1,      		 	0, 						0,				-1	},
	{ "Galculator",  			NULL,       	NULL,       		0,       			1,      		 	0, 						0,				-1	},
	{ NULL, 							NULL, 				"hidden", 			SP_MASK, 			1, 						0,						0,				-1  },
	{ "flterm", 					NULL, 				NULL, 					0, 						1, 						1,						0,				-1  },
	{ "tabbed", 					NULL, 				NULL, 					0, 						0, 						0,						0,				-1  },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
/* Tecla Meta = Mod4*/
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "6", NULL };

static const char *termcmd[]  = { TERM, NULL };
static const char *fltermcmd[]  = { TERM, "-c", "flterm", "-g", "60x20", NULL };
static const char *tbtermcmd[]  = { "tabbed", "-c", "-r", "2", TERM, "-w", "\'\'", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier               chain key   key        function        argument */

	/* Dmenu-based applications */
	{ MODKEY,                 XK_d,       XK_1,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                 XK_d,       XK_2,      spawn,          SHCMD("passmenu2") },
	{ MODKEY,                 XK_d,       XK_3,      spawn,          SHCMD("connect_wlan") },
	{ MODKEY,                 XK_d,       XK_4,      spawn,          SHCMD("ytfzf -D") },

	{ MODKEY,             		-1,         XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,	      -1, 				XK_Return, spawn,          {.v = fltermcmd } }, 
	{ MODKEY|ControlMask,	    -1, 				XK_Return, spawn,          {.v = tbtermcmd } }, 

	{ MODKEY|ShiftMask,       -1,					XK_b,      togglebar,      {0} },
	{ MODKEY,                 -1,         XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                 -1,         XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,       -1,					XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,       -1,					XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                 -1,         XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                 -1,         XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                 -1,         XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                 -1,         XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,     -1,         XK_equal,  zoom,           {0} },
	{ MODKEY,                 -1,         XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,       -1,         XK_w,      killclient,     {0} },
	{ MODKEY,                 -1,         XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                 -1,         XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,       -1,         XK_space,  setlayout,      {0} },
	{ MODKEY,             		-1,         XK_space,  togglefloating, {0} },
	{ MODKEY,             		-1,					XK_f,      togglefullscr,  {0} },
	{ MODKEY,                 -1,					XK_s,      togglesticky,   {0} },
	{ MODKEY,                 -1,         XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                 -1,         XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,       -1,         XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,       -1,         XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,       -1,					XK_BackSpace,      quit,   {0} },
	{ ALTKEY|ShiftMask,       -1,       	XK_l, 		 spawn,          SHCMD("slock") },

	/* Tag control */
	TAGKEYS(                  -1,         XK_1,                      0)
	TAGKEYS(                  -1,         XK_2,                      1)
	TAGKEYS(                  -1,         XK_3,                      2)
	TAGKEYS(                  -1,         XK_4,                      3)
	TAGKEYS(                  -1,         XK_5,                      4)
	TAGKEYS(                  -1,         XK_6,                      5)
	TAGKEYS(                  -1,         XK_7,                      6)
	TAGKEYS(                  -1,         XK_8,                      7)
	TAGKEYS(                  -1,         XK_9,                      8)
	{ MODKEY,                 -1,         XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,       -1,         XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                 -1,					XK_bracketleft, shiftview, {.i = -1 } },
	{ MODKEY,                 -1,					XK_bracketright, shiftview,{.i = +1 } },

	/* Center a floating window using custom wmMove command */
	{ MODKEY,       					-1,       	XK_c, 		 spawn,          SHCMD("wmMove center") },

	/* Scratchpad control */
	{ MODKEY,             		-1,					XK_backslash,	 scratchpad_hide, 			 {0} },
	{ MODKEY|ShiftMask,       -1,					XK_backslash,	 scratchpad_show, 			 {0} },
	{ MODKEY|ControlMask,     -1,					XK_backslash,  scratchpad_remove,			 {0} },

	/* Audio control */
	{ ShiftMask,       				-1,         XK_F1,     spawn,          SHCMD("pulsemixer --toggle-mute; pkill -RTMIN+10 dwmblocks")},
	{ ShiftMask,       				-1,         XK_F2,     spawn,          SHCMD("pulsemixer --change-volume -5; pkill -RTMIN+10 dwmblocks")},
	{ ShiftMask,       				-1,         XK_F3,     spawn,          SHCMD("pulsemixer --change-volume +5; pkill -RTMIN+10 dwmblocks")},
	{ ShiftMask,       				-1,         XK_F4,     spawn,          SHCMD("mic_check") },

	/* MPC */
	{ ShiftMask,       				-1,         XK_F5,     spawn,          SHCMD("mpc stop") },
	{ ShiftMask,       				-1,         XK_F6,     spawn,          SHCMD("mpc prev") },
	{ ShiftMask,       				-1,         XK_F7,     spawn,          SHCMD("mpc toggle") },
	{ ShiftMask,       				-1,         XK_F8,     spawn,          SHCMD("mpc next") },

	/* Brightness control */
	{ ShiftMask,       				-1,         XK_F11,    spawn,          SHCMD("xbacklight -dec 10") },
	{ ShiftMask,       				-1,         XK_F12,    spawn,          SHCMD("xbacklight -inc 10") },

	/* Terminal Applications */
	{ MODKEY,             		-1,         XK_v, 		 spawn,          SHCMD(TERM " -e nvim") },
	{ MODKEY,             		XK_e,       XK_1, 		 spawn,          SHCMD(TERM " -e lf") },
	{ MODKEY,             		XK_e,       XK_2, 		 spawn,          SHCMD(TERM " -e ncmpcpp") },
	{ MODKEY,             		XK_e,       XK_3, 		 spawn,          SHCMD(TERM " -e ncspot") },
	{ MODKEY,             		XK_e,       XK_4, 		 spawn,          SHCMD(TERM " -e aerc") },
	{ MODKEY,             		XK_e,       XK_5, 		 spawn,          SHCMD(TERM " -e newsboat") },
	{ MODKEY,             		XK_e,       XK_6, 		 spawn,          SHCMD(TERM " -e pulsemixer") },
	{ MODKEY,             		XK_e,       XK_7, 		 spawn,          SHCMD(TERM " -e htop") },

	/* Graphical Applications */
	{ MODKEY,             		XK_g,       XK_1, 		 spawn,          SHCMD("firefox-developer-edition") },
	{ MODKEY,             		XK_g,       XK_2, 		 spawn,          SHCMD("librewolf") },
	{ MODKEY,             		XK_g,       XK_3, 		 spawn,          SHCMD("luakit") },

	/* Screenshots with scrot */
	{ 0,             					-1,       	XK_Print,  spawn,          SHCMD("sleep 0.2; scrot -s -f '%Y-%m-%d_$wx$h.png' -e 'mv $f /home/math/Imagens/Prints'") },
	{ ShiftMask,        			-1,       	XK_Print,  spawn,          SHCMD("sleep 0.2; scrot -u '%Y-%m-%d_$wx$h.png' -e 'mv $f /home/math/Imagens/Prints'") },
	{ ALTKEY,        					-1,       	XK_Print,  spawn,          SHCMD("sleep 0.2; \"%Y-%m-%d_$wx$h.png\" -e 'mv $f /home/math/Imagens/Prints'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

