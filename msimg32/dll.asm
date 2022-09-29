
;ml64 /Fo $(IntDir)%(fileName).obj /c /Cp %(fileName).asm
;$(IntDir)%(fileName).obj;%(Outputs)



.DATA
EXTERN pfnAheadLib_vSetDdrawflag:dq;
EXTERN pfnAheadLib_AlphaBlend:dq;
EXTERN pfnAheadLib_DllInitialize:dq;
EXTERN pfnAheadLib_GradientFill:dq;
EXTERN pfnAheadLib_TransparentBlt:dq;

.CODE
AheadLib_vSetDdrawflag PROC
	jmp pfnAheadLib_vSetDdrawflag
AheadLib_vSetDdrawflag ENDP

AheadLib_AlphaBlend PROC
	jmp pfnAheadLib_AlphaBlend
AheadLib_AlphaBlend ENDP

AheadLib_DllInitialize PROC
	jmp pfnAheadLib_DllInitialize
AheadLib_DllInitialize ENDP

AheadLib_GradientFill PROC
	jmp pfnAheadLib_GradientFill
AheadLib_GradientFill ENDP

AheadLib_TransparentBlt PROC
	jmp pfnAheadLib_TransparentBlt
AheadLib_TransparentBlt ENDP


END
