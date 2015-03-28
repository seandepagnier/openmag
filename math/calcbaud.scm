(define (print-val-and-error F_OSC BAUD)
  (let ((ubbrf (- (/ F_OSC (* 16 BAUD)) 1)))
    (let ((ubbr (round ubbrf)))
      (let ((error% (exact->inexact (/ (round (/ (* (abs (- ubbrf ubbr)) 10000) (+ ubbr 2)))
                                       100))))
        (cond ((>= ubbr 0)
               (display ubbr) (display "\t") (display error%) (display "%\t")))))))

(define (show-bauds F_OSC BAUDS)
  (for-each
   (lambda (BAUD)
     (display BAUD) (display "\t")
     (print-val-and-error F_OSC BAUD)
     (print-val-and-error (* 2 F_OSC) BAUD)
     (newline))
   BAUDS))

(define common-bauds '(300 600 1200 2400 4800 9600 19200 38400 57600 115200))

(define (show-common-bauds F_OSC)
  (show-bauds F_OSC common-bauds))

;; try this
(show-common-bauds 12e6)
