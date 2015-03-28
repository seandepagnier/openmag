	.text
	.global	testasm
	.type	testasm, @function
testasm:
	inc r24
	inc r24
        rjmp testasm
	ret
