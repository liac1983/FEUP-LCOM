PROG=proj

.PATH: ${.CURDIR}/controllers/
.PATH: ${.CURDIR}/controllers/keyboard
.PATH: ${.CURDIR}/controllers/mouse
.PATH: ${.CURDIR}/controllers/timer
.PATH: ${.CURDIR}/controllers/graphics

SRCS = main.c graphics.c menu.c game.c timer.c keyboard.c mouse.c button.c player.c level.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
