/*tn.h - token�� ���� ����, errorTypes�� ���� ����
* programmer - �輭��, ��ȿ��, ������, ��ȿ��
* date - 
*/

enum tokentypes {
	TIDENT, TPLUS, TSTAR, TMOD, TSLASH, TMINUS, TCONST, TELSE, TIF, TINT, TRETURN, TVOID, TWHILE, TMOD, TASSIGN,
	TADDASSIGN, TSUBASSIGN, TMULASSIGN, TDIVASSIGN, TMODASSIGN, TNOT, TAND, TOR, TEQUAL, TNOTEQU, TLESS, TGREAT,
	TLESSE, TGREATE, TINC, TDEC, TLPAREN, TRPAREN, TCOMMA, TSEMI, TLBRACE, TRBRACE, TLBRACKET, TRBRACKET, TNUMBER, TREALNUM,
	TCOMMENT_LINE, TCOMMENT_TEXT
};

enum errorTypes {
	noerror, illsp, illid, overst, overfl, illch
};
