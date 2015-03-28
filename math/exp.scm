(define (show-exp exps coeffs)
  (define (show-term exp coeff)
    (cond ((null? exp)
           (display coeff))
          (else
           (cond ((= -1 coeff)
                  (display "-"))
                 ((not (= 1 coeff))
                  (display coeff)
                  (display "*")))
             (display exp))))

  (let ((less-zeros
         (let cull-zeros ((exps exps) (coeffs coeffs))
           (if (null? exps) '()
               (let ((rest (cull-zeros (cdr exps) (cdr coeffs))))
                 (if (not (zero? (car coeffs)))
                     (cons (list (car exps) (car coeffs))
                           rest)
                     rest))))))

    (let loop ((terms less-zeros) (first #t))
      (if (null? terms)
          (if first
              (display "0"))
          (begin
            (if (not first)
                (display " + "))
            (show-term (caar terms) (cadar terms))
            (loop (cdr terms) #f))))))


