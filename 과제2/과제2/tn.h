/*tn.h - token에 관한 정의, errorTypes에 관한 정의
* programmer - 김서영, 김효진, 서지원, 손효원
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
};	//illid: identifier 오류, overst: 12자 이상 identifier, overfl: ST꽉참
