pad:
;sprite pad = {
	DB 0x80
	DB 0x80
	DB 0x80
	DB 0x80
;    0x80, 0x80, 0x80, 0x80, 0x80
	DB 0x80
;};
;
ball:
	DB 0x80
;sprite ball = { 0x80 };
;
main:
;void main()
;{
	LD V0,0x20
END1:
	LD V1,0x08
END2:
	LD I,pad
	DRW V0,V1,0x5
	LD V0,VF
END0:
;    draw(32, 8, pad);
;
;    // Ball.
	LD V0,0x03
END3:
;    byte x = 3;
	LD V1,0x0A
END4:
;    byte y = 10;
	LD V2,V0
END6:
	LD V3,V1
END7:
	LD I,ball
	DRW V2,V3,0x1
	LD V2,VF
END5:
;    draw(x, y, ball);
;
;    // Ball moves until it gets a collision.
	LD V2,0x00
END8:
;    byte collide = 0;
WHILE9:
	LD V3,V2
	LD V4,0x00
	SE V3,V4
	JP ELS11
	LD V3,0x01
	JP END11
ELS11:
	LD V3,0x00
END11:
END10:
	SNE V3,0x00
	JP END9
;    while(collide == 0) // (!) does not work.
;    {
	LD V3,V0
END13:
	LD V4,V1
END14:
	LD I,ball
	DRW V3,V4,0x1
	LD V3,VF
END12:
;        draw(x, y, ball);
	LD V3,V2
	ADD V0,0x01
	LD V4,V0
END16:
	LD V5,V1
END17:
	LD I,ball
	DRW V4,V5,0x1
	LD V4,VF
	LD V2,V4
END15:
;        collide = draw(++x, y, ball);
;    }
	JP WHILE9
END9:
;
;    // Done.
	CLS
	LD V3,VF
END18:
;    clear();
WHILE19:
	LD V3,0x01
END20:
	SNE V3,0x00
	JP END19
;    while(1)
;    {
;    }
	JP WHILE19
END19:
;}
	LD VF,0x03
	SUB VE,VF
	LD F,VE
	LD VE,[I]
	LD VF,V0
	RET
