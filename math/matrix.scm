;;
;; for these procedures, the matrix form is in a list.
;; all operations are for arbitrary sized matrix
;;
;; '((1 2) (3 4)) is the same as |1 2|
;;                               |3 4|
;;

;; transpose a matrix
(define (transpose x)
  (if (null? x)
      x
      (let loop ((n 0))
        (if (= n (length (car x)))
            '()
            (cons
             (map (lambda (a)
                    (list-ref a n))
                  x)
             (loop (+ n 1)))))))

;; multiply two matrixes
(define (mult-matrix x y)
  (if (not (= (length (car x)) (length y)))
      (begin (display "Warning, invalid matrix multiply") (newline)))
  (map (lambda (a)
         (map (lambda (b)
                (apply + (map * a b)))
              (transpose y)))
       x))

;; multiply each element by a constant
(define (mult-matrix-constant c x)
  (map (lambda (a)
         (map (lambda (b)
                (* c b))
              a))
       x))

;; pull out row and col from matrix m
(define (remove-rc row col m)
  (let ((rc (length m))
        (cc (length (car m))))
    (let rloop ((r 0))
      (cond ((= r rc) '())
            ((= r row) (rloop (+ r 1)))
            (else (cons
                   (let cloop ((c 0))
                     (cond ((= c cc) '())
                           ((= c col) (cloop (+ c 1)))
                           (else (cons
                                  (list-ref (list-ref m r) c)
                                  (cloop (+ c 1))))))
                   (rloop (+ r 1))))))))

;; compute determinant of a matrix
(define (determinant x)
  (if (null? x) 1
      (let loop ((pos 0) (sign 1))
        (if (= pos (length x)) 0
            (let ((c (remove-rc 0 pos x)))
              (+ (* sign (list-ref (car x) pos)
                    (if (null? c) 1 (determinant c)))
                 (loop (+ 1 pos) (- sign))))))))

; elimination style determinant
(define (determinant-fast x)
  (if (null? x) 1
      (let ((tr (car x))
            (rr (cdr x)))
        (let ((pos (- (length tr) (length x))))
          (for-each (lambda (row)
                     (let ((val (list-ref tr pos)))
                       (if (= val 0)
                           (set! tr (map + tr row)))))
                       rr)
          (let ((val (list-ref tr pos)))
            (if (= val 0) 0
                (let ((nr (map (lambda (row)
                                 (let ((y (/ (list-ref row pos) val)))
                                   (map (lambda (p q)
                                          (- p (* y q)))
                                        row tr)))
                               rr)))
                  (* val (determinant-fast nr)))))))))

;; compute the cofactor matrix
(define (cofactor m)
  (let ((rc (length m))
        (cc (length (car m))))
    (let rloop ((r 0) (rsign 1))
      (if (= r rc) '()
          (cons
           (let cloop ((c 0) (csign 1))
             (if (= c cc) '()
                 (cons (* rsign csign 
                          (determinant-fast (transpose (remove-rc r c m))))
                       (cloop (+ c 1) (- csign)))))
           (rloop (+ r 1) (- rsign)))))))

;; compute adjugate of m
(define (adjugate m)
  (transpose (cofactor m)))

;; compute the matrix inverse, return 'none or the inverse
(define (invert x)
  (let ((det (determinant-fast x)))
    (if (= 0 det)
        'none
        (mult-matrix-constant (/ det) (adjugate x)))))
