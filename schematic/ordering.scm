; Copyright (C) 2009 Sean D'Epagnier <sean@depagnier.com>
;
; This Program is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public
; License as published by the Free Software Foundation; either
; version 3 of the License, or (at your option) any later version.

; These routines find optimal instructions (for a human) to minimize
; cost when ordering electronic components from various suppliers


; Parts
; Start out with routines to define various parts and their use
; in various devices.

; parse-constant: take a string with suffix
; return (power value)
(define (parse-constant str places negallowed)
  (let split-loop ((split
		    (let ((n (- (string-length str) 1)))
		      (let ((c (substring str n))
			    (rest (substring str 0 n)))
			(let const-loop
			    ((constants '((G 9) (M 6) (k 3)
					  (m -3) (u -6) (n -9) (p -12))))
			  (cond ((null? constants)
				 (let ((n (string->number str)))
				   (if n
				       (list 0 n)
				       (error "unrecognized suffix" str))))
				((eq? (string->symbol c) (caar constants))
				 (list (cadar constants)
				       (let ((n (string->number rest)))
					 (if n n (error "invalid number" rest)))))
				(else (const-loop (cdr constants)))))))))
    (let ((exp (car split))
	  (num (cadr split)))
      (cond  ((= num 0) (error "parse-constant does not support 0"))
	     ((and (or negallowed (> exp 0))
		   (< num (expt 10 (- places 1))))
	      (split-loop (list (- exp 1) (* num 10))))
	     ((>= num (expt 10 places))
	      (split-loop (list (+ exp 1) (/ num 10))))
	     (else (list exp (inexact->exact (floor num))))))))

; generate the part number needed for a capacitor of a given size
(define (capacitor str)
    (let ((lst (string->list str)))
      (if (equal? (substring str (- (string-length str) 1)) "F")
	  (capacitor (substring str 0 (- (string-length str) 1)))
	  (let ((parsed (parse-constant str 2 #t)))
	    (let ((exp (car parsed))
		  (num (cadr parsed)))
	      (string->symbol (string-append "C0603C" (number->string num)
					     (number->string (+ 12 exp)))))))))

; generate part number for a resistor, eg: (resistor "100k")
(define (resistor str)
  (let ((parsed (parse-constant str 3 #f)))
    (let ((exp (car parsed))
	  (num (cadr parsed)))
      (string->symbol (string-append "MCR03EZPFX" (number->string num)
				     (if (and (= 0 exp)
					      (< num 100))
					 "R0"
					 (number->string exp)))))))

; generate the part number for a potentiometer
(define (variable-resistor str)
  (let ((parsed (parse-constant str 2 #f)))
    (let ((exp (car parsed))
	  (num (cadr parsed)))
      (string->symbol (string-append "CT6EP" (number->string num) (number->string exp))))))

; parts in all units
(define base-parts
  `((,(resistor "200") 1)
    (,(resistor "10k") 1)
    (,(resistor "100k") 1)
    (,(capacitor ".1uF") 1)
    (,(capacitor "1uF") 3)
    (ad7799 1)
    (hmc1052 1)
    (hmc1051Z 1)
    (sca3000-D01 1)
    (irf7509 2)
    (wt21 1)
    (laser 1)
))

(define baro-parts
  `((,(capacitor "1uF") 3)
    (scp1000-D01 1)))

(define hot-wire-parts
  `((,(capacitor "1uF") 2)
    (TPS73730 1)
    (,(variable-resistor "1k") 1)
    (,(variable-resistor "100k") 1)
    (STN790A 1)))

; current parts required for the dusi
(define dusi-base-parts
  (append base-parts
	  `((pcb-dusi 1)
	    (atmega2561V 1)
	    (25LC512-I/SN 1)
	    (1101M2S3ABE2 1)
	    (XF2L-3035-1A 1)
	    (ft232rl 1)
	    (690-004-221-023 2)
	    (PPTC05LFBN-RC 1)
	    (fan2503S30X 1)
	    (dusi-box 1)
	    (large-lcd 1)
	    (keypad 1)
	    (,(resistor "1M") 2)
	    (,(capacitor "1uF") 8))))

(define pointer-base-parts
  (append base-parts
	  `((pcb-pointer 1)
	    (at90usb1287 1)
	    (small-lcd 1)
	    (UX60-MB-5S8 1))))

(define (parts-multiply parts num)
  (map (lambda (cell) (list (car cell) (* (cadr cell) num))) parts))

(define (parts-unduplicate parts)
  (if (null? parts)
      '()
      (let loop ((part (car parts))
		 (parts (parts-unduplicate (cdr parts))))
	(cond ((null? parts)
	       (if (= 0 (cadr part))
		   '()
		   (list part)))
	      ((eq? (car part) (caar parts))
	       (loop (list (car part) (+ (cadr part) (cadar parts))) (cdr parts)))
	      ((= 0 (cadar parts)) (loop part (cdr parts)))
	      (else (cons (car parts) (loop part (cdr parts))))))))

; Suppliers
; Contains information on price breaks and overhead for each supplier
(define (make-supplier info parts overhead)
  (lambda (op)
    (case op
      ((info) info)
      ((quote) (lambda (part quantity)
		 (let find-part ((parts parts))
		   (cond ((null? parts) #f)
			 ((eq? (caar parts) part)	
			  (let min-price ((min #f)
					  (costs (cdar parts)))
			    (if (null? costs)
				min
				(let ((cqua (caar costs))
				      (ccos (cadar costs)))
				  (let ((qua (if (< quantity cqua) cqua quantity)))
				    (let ((cost (* qua ccos)))
				      (min-price (if (or (not min) (< cost (cadr min)))
						     (list qua cost)
						     min)
						 (cdr costs))))))))
			 (else (find-part (cdr parts)))))))
      ((overhead) overhead))))

; typical $25 overhead for bank charge, and $25 for shipping
(define (make-standard-chinese-supplier email parts)
  (make-supplier email parts 70))

(define suppliers
  (list 
   (make-supplier
    "digikey.com"
    '((ad7799 (1 8.63) (25 7.77) (100 6.8894))
      (hmc1051Z (1 17) (25 6) (100 5))
      (at90usb1287 (1 15.05) (25 10.5) (100 9.3333))
      (atmega2561V (1 15.91) (25 11.1) (100 9.8666))
      (irf7509 (1 .76) (10 .444) (100 .296))
      (C0603C220 (10 .036) (100 .0215) (500 .01238))
      (C0603C104 (10 .023) (100 .0137) (500 .00788))
      (C0603C105 (10 .198) (100 .117) (500 .0675) (1000 .0468))
      (MCR03EZPFX1004 (10 .076) (50 .0408) (200 .0234) (1000 .01595))
      (MCR03EZPFX1003 (10 .076) (50 .0408) (200 .0234) (1000 .01595))
      (MCR03EZPFX1002 (10 .076) (50 .0408) (200 .0234) (1000 .01595))
      (MCR03EZPFX2000 (10 .076) (50 .0408) (200 .0234) (1000 .01595))
      (XF2L-3035-1A (1 1.25) (25 1.12) (50 .9964) (100 .8969) (250 .84696) (500 .74730))
      (ECJ-2FB0J106K (1 .46) (10 .41) (100 .2465) (500 .16416))
      (ft232rl (1 4.5) (10 4.05) (50 3.85) (100 3.65) (250 3.25) (500 2.85))
      (25LC512-I/SN (1 3.25) (25 1.79) (100 1.72))
      (1101M2S3ABE2 (1 4.92) (25 3.9336) (100 3.1141))
      (CT6EP102 (1 .68) (10 .63) (100 .57) (500 .486))
      (CT6EP104 (1 .68) (10 .63) (100 .57) (500 .486))
      (TL1105AF250Q (1 .29) (10 .255) (100 .22) (250 .176) (500 .154))
      (UX60-MB-5S8 (1 1.18) (25 .85 24) (100 .6975) (250 .589) (500 .496))
      (PPTC05LFBN-RC (1 .46) (10 .383) (100 .2442) (500 .18876))
      (690-004-221-023 (1 .93) (10 .825) (100 .66) (500 .6) (1000 .51))
      (CA-8 (1 8.03) (25 6.3752) (50 5.4826) (100 4.845) (250 4.335) (500 3.825))
      (sca3000-D01 (1 29.73) (10 28.318) (25 36.9676) (50 25.6824))
      (scp1000-D01 (1 29.73) (10 28.318) (25 36.9676) (50 25.6824))
      (STN790A (1 .62) (10 .49) (100 .3382) (250 .27592) (500 .25810))
      (TPS73701DCQ (1 1.5) (25 1.2) (100 .9) (250 .84) (500 .81) (1000 .78))
      (CS208.000MABJ (1 1.1) (10 .9) (50 .8) (100 .7051)))
    5)
   (make-supplier
    "speedypcb, boxenclosures.com, bobzhangxu@yahoo.com.cn, semiconductorstore.com"
    '((pcb-dusi (1 10) (10 6) (30 2) (100 1))
      (pcb-pointer (1 5) (10 3) (30 1) (100 .5))
      (dusi-box (1 30) (10 25) (100 20))
      (keypad (1 7) (10 6) (30 4.5) (100 4))
      (laser (1 12) (10 8) (50 5) (100 4))
      (small-lcd (10 2))
      (large-lcd (10 8))
      (wt21 (1 24)))
    100)
   (make-supplier
    "mouser.com"
    '((at90usb1287 (1 15.05) (10 12.95) (25 10.5) (100 9.33))
      (atmega2561V (1 15.91) (10 13.69) (25 11.1) (100 9.86))
      (C0603C220 (1 .2) (100 .13) (500 .092))
      (C0603C104 (1 .05) (100 .01) (500 .009) (1000 .007))
      (C0603C105 (1 .17) (100 .1) (500 .087) (1000 .07))
      (ECJ-2FB0J106K (1 .39) (50 .32) (100 .191) (500 .11) (1000 .076))
      (ft232rl (1 4.5) (10 4.05) (50 3.85) (100 3.65) (500 2.85) (1000 2.65))
      (25LC512-I/SN (1 2.24) (10 1.86) (25 1.79) (100 1.72))
      (1101M2S3ABE2 (1 2.66) (25 2.51) (50 2.36) (100 2.2))
      (TL1105AF250Q (1 .15) (10 .14) (50 .12) (100 .09) (500 .08))
      (CA-8 (1 8.64) (10 7.59) (25 7.01) (50 4.67) (100 4.10))
      (STN790A (1 .47) (10 .40) (100 .287) (250 .236) (500 .18))
      (TPS73701DCQ (1 1.5) (25 1.2) (100 .9) (250 .84) (500 .81) (1000 .78))
      (fan2503S30X (1 .81) (25 .53) (100 .45) (250 .377) (500 .326) (1000 .28))
      (fan2503S33X (1 .81) (25 .53) (100 .45) (250 .377) (500 .326) (1000 .28))
      (CS208.000MABJ (1 1.1) (10 .9) (50 .8) (100 .7)))
    5)
   (make-supplier
    "vti.com"
    '((sca3000 (100 14) (1000 12))
      (scp1000 (100 16) (1000 12)))
    50)
   (make-standard-chinese-supplier
    "joyce@huatai-hk.com"
    '((hmc1052 (30 2))
      (hmc1051Z (30 7.94))
      (max1811 (30 1.24))
      (ad7799 (30 2.8))
      (irf7509 (30 .18))
      (FAN2503S30X (30 .35))
      (ft232rl (30 2.36))
      (atmega2561V (30 6.36))))
   (make-standard-chinese-supplier
    "ekkelect@gmail.com"
    '((hmc1051Z (100 2))
      (ad7799 (100 1.5))))
   (make-supplier
    "sparkfun.com"
    '((scp1000-D01 (1 24.95) (10 22.46) (100 19.96)))
    2)
   ))

(define (permutate-suppliers suppliers)
  (if (null? suppliers)
      '()
      (cons
       suppliers
       (let loop ((a suppliers)
		  (b '()))
	 (if (null? a)
	     '()
	     (append
	      (permutate-suppliers (append (cdr a) b))
	      (loop (cdr a) (cons (car a) b))))))))

; first subtract out stock
(define (remove-stock-for-order parts stock)
  (let stock-loop ((nstock stock)
		   (estock '()))
    (cond ((null? parts) (list parts stock))
	  ((null? nstock)
	   (let ((rest (remove-stock-for-order (cdr parts) estock)))
	     (list (cons (car parts) (car rest))
		   stock)))
	  ((eq? (caar nstock) (caar parts))
	   (let ((stocknum (cadar nstock))
		 (partsnum (cadar parts))
		 (rest (remove-stock-for-order (cdr parts) (append estock (cdr nstock)))))
	     (if (> stocknum partsnum)
		 (list (cons (list (caar parts) 0) (car rest))
		       (cons (list (caar nstock) (- stocknum partsnum)) (cadr rest)))
		 (list (cons (list (caar parts) (- partsnum stocknum)) (car rest))
		       (cons (list (caar nstock) 0) (cadr rest))))))
	  (else (stock-loop (cdr nstock)
			    (cons (car nstock) estock))))))

; using only the parts and all of the given suppliers,
; return (totalcost ((part supplier parts-cost extra-stock) ...))
(define (make-order-with-suppliers parts suppliers fail)
  (if (null? parts)
      (list (let supplier-overhead
		((suppliers suppliers))
		 (if (null? suppliers)
		     0
		     (+ (supplier-overhead (cdr suppliers))
			((car suppliers) 'overhead))))
	    '())
      (let supplier-loop ((csuppliers suppliers)
			  (min #f))
	(if (null? csuppliers)
	    (let ((rest (make-order-with-suppliers (cdr parts) suppliers fail)))
	      (if (and fail (not min)) (fail (car parts)))
	      (if (and min rest)
		  (list (+ (cadadr min) (car rest))
			(cons (cons (caar parts) min) (cadr rest)))
		  #f))
	    (let ((quote (apply ((car csuppliers) 'quote) (car parts))))
	      (supplier-loop (cdr csuppliers)
			     (if (and quote (or (not min) (< (cadr quote) (caddr min))))
				 (list (car csuppliers) quote (- (car quote) (cadar parts)))
				 min)))))))

; same as above, except it does it for all supplier permutations
; to get the lowest cost
(display "permutating suppliers... ") (flush-output)
(define permutated-suppliers (permutate-suppliers suppliers))
(display "done") (newline)

(define (make-order parts)
  (let each-supplier ((suppliers permutated-suppliers)
		      (min #f)
		      (supplier-num 0))
    (cond ((= 0 (remainder supplier-num 1000))
	   (display "\r") (display supplier-num) (display "/")
	   (display (length permutated-suppliers)) (flush-output)))
    (if (null? suppliers)
	(if min
	    min
	    (begin (newline)
		   (make-order-with-suppliers
		    parts (car permutated-suppliers)
		    (lambda (part) (display "cannot supply: ") (display part) (newline)))))
	(let ((cur (make-order-with-suppliers parts (car suppliers) #f)))
	  (each-supplier (cdr suppliers)
			 (if (and cur (or (not min)
					  (< (car cur) (car min)))) cur min)
			 (+ supplier-num 1))))))

; ((part1 supplier1 quote1 excess1) (part2 supplier1 quote2 excess2) ...)
; =>
; ((supplier1 (part1 quote1 excess1) (part2 quote2 excess2)) ...)
(define (sort-order-by-supplier order)
  (let order-loop ((order order)
		   (suppliers '()))
    (if (null? order)
	suppliers
	(order-loop (cdr order)
		    (let ((o (car order)))
		      (let suppliers-combine ((suppliers suppliers))
			(cond ((null? suppliers) (list (list (cadr o) (cons (car o) (cddr o)))))
			      ((eq? (cadr o) (caar suppliers))
			       (cons (append (car suppliers) (list (cons (car o) (cddr o))))
				     (cdr suppliers)))
			      (else (cons (car suppliers)
					  (suppliers-combine (cdr suppliers)))))))))))

; warning.. I didn't actually count this stock to be accurate yet
(define stock '((ad7799 30)
		(max1811 30)
		(at90usb1287 30)
		(keypad 60)
		(dusi-box 30)
		(laser-module 32)
		(1101M2S3ABE2 30)
		(PPTC05LFBN-RC 30)
		(690-004-221-023 60)))

; given certain parts, print out the instructions for obtaining
; needed parts from default suppliers, the cost, and the resulting
; excess parts if certain parts cannot be obtained and error occurs
(define (print-human-instructions parts)
  (let ((parts-stock (remove-stock-for-order (parts-unduplicate parts) stock)))
    (let ((parts (parts-unduplicate (car parts-stock))))
      (let ((order (make-order parts)))
	(newline)
	(if order
	    (let ((total-cost (car order))
		  (sorted (sort-order-by-supplier (cadr order))))
	      (let supplier-loop ((supplier sorted))
		(if (not (null? supplier))
		    (begin (display "Supplier: ")
			   (display ((caar supplier) 'info))
			   (newline)
			   (let parts-loop ((parts (cdar supplier)))
			     (if (null? parts)
				 (supplier-loop (cdr supplier))
				 (begin (display "\t")
					(display (caar parts))
					(display " order ")
					(display (caadar parts))
					(display " @ $")
					(display (car (cdadar parts)))
					(display "   excess: ")
					(display (caddar parts))
					(newline)
					(parts-loop (cdr parts))))))))
	      (display "total cost: $")
	      (display total-cost)
	      (newline)))))))

(print-human-instructions (parts-multiply (append pointer-base-parts) 50))
(print-human-instructions (parts-multiply (append pointer-base-parts) 80))
(print-human-instructions (parts-multiply (append pointer-base-parts) 100))
