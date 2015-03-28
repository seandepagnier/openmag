; exponents with arbitrary parameters, we only support 2 for derivatives
(define (^ . vals)
  (if (null? vals) 1
      (expt (car vals) (apply ^ (cdr vals)))))


; lets find (f(x)^g(x))'
; y = f(x)^g(x)
; ln(y) = ln(f(x)^g(x)) = g(x)*ln(f(x))
; ln(y)' = y'/y = g'(x)*ln(f(x)) + g(x)*f'(x)/f(x)
; y' = f(x)^g(x)*(g'(x)*ln(f(x)) + g(x)*f'(x)/f(x)) 

; lets find (f(x)/g(x))'
; y = f(x)/g(x)
; y = f(x)*g(x)^-1
; y' = f(x)'/g(x) - f(x)*g'(x)/g(x)^2

(define (derivative exp var)
  (define funcs
    `((log ,(lambda (x) `(/ ,x)))
      (sin ,(lambda (x) `(cos ,x)))
      (cos ,(lambda (x) `(- (sin ,x))))
      (exp ,(lambda (x) `(exp x)))
      ))
  (let d ((exp exp))
    (cond
     ((eq? exp var) 1)
     ((list? exp)
      (let ((op (car exp)))
        (case op
          ((+ -)
           (cons op (map d (cdr exp))))
          ((*)
           (if (null? (cdr exp)) 0
               `(+ (* ,(cadr exp) ,(d (cons '* (cddr exp))))
                   (* ,(d (cadr exp)) ,(cons '* (cddr exp))))))
          ((/)
           (if (null? (cddr exp))
               `(- (/ ,(d (cadr exp)) (* ,(cadr exp) ,(cadr exp))))
               (d `(* ,(cons (cadr exp) (map (lambda (x) `(/ ,x)) (cddr exp)))))))
          ((^)
           (if (not (= (length exp) 3))
               (error "^ operator must have exactly 2 args")
               `(* ,exp (+ (* ,(d (caddr exp)) (log ,(cadr exp)))
                           (* ,(caddr exp) ,(d (cadr exp)) (/ ,(cadr exp)))))))
          (else
           (let loop ((func funcs))
             (cond ((null? func) (error "function not found: " op))
                   ((eq? op (caar func)) (list '* ((cadar func) (cadr exp)) (d (cadr exp))))
                   (else (loop (cdr func)))))))))
     (else 0))))


(define (simplify-weight exp)
  (cond ((null? exp) 0)
        ((pair? exp) (+ 1
                        (simplify-weight (car exp))
                        (simplify-weight (cdr exp))))
        (else 1)))

(define integration-rules
  '(((int a dx) -> (* a x))
    ((int (^ x a) dx) -> (* a (^ x (+ a 1))))
    ((int (+ x y)) -> (+ (int x) (int y)))
    ((* x y) -> (+ (int x dy) (int y dx)))
    ))

; x, y, z is any number or variable
; a, b, c must be numbers
; ... is multiple items or or nothing, can be specified more than once for the match,
;     which means they can be concatenated
; .. means there must be at least 1 item
(define simplification-rules
  '(((+) -> 0)
    ((+ x) -> x)
    ((+ 0 ...) -> (+ ...))
    ((+ a b) -> ,(+ a b))
    ((+ x x) -> (* 2 x))
    ((+ x y) -> (+ y x))
    ((+ ... (+ ...) ...) -> (+ ...))
    ((- (- x)) -> x)
    ((- x y) -> (- (- y x)))
    ((- a b) -> ,(- a b))
    ((- x ..) -> (+ x ,(map - ..)))
    ((*) -> 1)
    ((* x) -> x)
    ((* ... 0 ...) -> 0)
    ((* ... 1 ...) -> (* ...))
    ((* ... (/ x) ... x ...) -> (* ...))
    ((* ... (* ...) ...) -> (* ...))
    ((* (- x) (- y)) -> (* x y))
    ((/ (/ x)) -> x)
    ((/ x ..) -> (* x ,(map / ..)))
    ))

(define (convert-simplification-rule rule)
  (let ((from (car rule))
        (to (caddr rule)))
    (let ((op (car from)))
      (lambda (exp)
        (if (not (and (pair? exp) (eq? op (car exp)))) '()
            (
    

(define (simplify exp)

 
(define (expand exp)
  (define (self-expanding? exp)
    (or (null? exp)
          (number? exp)
          (symbol? exp)))
  (cond ((self-expanding? exp) exp)
        (else
         (let ((op (car exp)))
           (cond ((eq? op '*)
                  (let ((expanded (map expand (cdr exp))))
                    (cond ((null? expanded) 1)
                          ((null? (cdr expanded)) (cons '* expanded))
                          (else
                           (let ((rest (expand (cons '* (cdr expanded)))))
                             (cond 
                              ((self-expanding? (car expanded))
                               (expand (list '* rest (car expanded))))
                              ((eq? (caar expanded) '+)
                                    (cons '+
                                          (map (lambda (val)
                                                 (expand (cons '* (cons val (cdr expanded)))))
                                               (cdar expanded))))
                                   (else
                                    (cons '* expanded))))))))
                 (else exp))))))
