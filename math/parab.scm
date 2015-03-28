(load "matrix.scm")
(load "exp.scm")

(define (fit-parab vars)
  (define (parts exp1 exp2 exp3)
    (map (lambda (var)
           (list (exp1 var) (exp2 var) (exp3 var)))
         vars))

  (define x^2 (lambda (x) (square (car x))))
  (define x car)
  (define y cadr)
  (define one (lambda (x) 1))

  (list
   (determinant (parts x y one))
   (- (determinant (parts x^2 y one)))
   (determinant (parts x^2 x one))
   (- (determinant (parts x^2 x y)))
)
)

(define (each-det vars exps)

(define (parab-to-y-form result)
    (let ((y (caddr result)))
      (let ((a (- (/ (car result) y)))
            (b (- (/ (cadr result) y)))
            (c (- (/ (cadddr result) y))))
        (list a b c))))

(define parab-vars '(x^2 x ()))

(define (nice-display-parab vars)
  (display "y = ")
  (show-exp
   parab-vars
   (parab-to-y-form (fit-parab vars))))
