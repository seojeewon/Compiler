/*tn.h - token�� ���� ����, errorTypes�� ���� ����
* programmer - �輭��, ��ȿ��, ������, ��ȿ��
* date - 
*/

enum tokentypes {
	TIDENT, TPLUS, TMUL, TMOD, TMINUS, TDIV, TCONST, TELSE, TIF, TINT, TRETURN, TVOID, TWHILE, TASSIGN,
	TADDASSIGN, TSUBASSIGN, TMULASSIGN, TDIVASSIGN, TMODASSIGN, TNOT, TAND, TOR, TEQUAL, TNOTEQU, TLESS, TGREAT,
	TLESSE, TGREATE, TINC, TDEC, TLPAREN, TRPAREN, TCOMMA, TSEMI, TLBRACE, TRBRACE, TLBRACKET, TRBRACKET, TNUMBER, TREALNUM,
	TCOMMENT_LINE, TCOMMENT_TEXT, TLINE, TSEPARATOR, TBQUOTE, TSQUOTE, TERROR, TEOF
};

enum errorTypes {
	noerror, illid, overst, overfl
};	//illid: identifier ����, overst: 12�� �̻� identifier, overfl: ST����
