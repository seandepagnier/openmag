;;;;;;; ORIGINAL ellipsoid fit routines, now obsolete

(define (dist . vals)
  (sqrt (apply + (map square vals))))

(define (avg-pos a . b)
  (map (lambda (val)
         (/ val (+ (length b) 1)))
       (fold (lambda (x y)
               (map + x y))
             a b)))

(define (super-map proc a)
  (cond ((null? a) '())
        ((pair? a) (cons (super-map proc (car a))
                         (super-map proc (cdr a))))
        (else (proc a))))

(define (rnd . x) (super-map exact->inexact x))

(define (slope-2d a b)
  (let ((diffx (- (car b) (car a)))
        (diffy (- (cadr b) (cadr a))))
    (/ diffy diffx)))

(define (perp-slope-2d slope)
  (- (/ slope)))

(define (circle a b c)
  (let ((ab-center (avg-pos a b))
        (bc-center (avg-pos b c))
        (ab-bisect-slope (perp-slope-2d (slope-2d a b)))
        (bc-bisect-slope (perp-slope-2d (slope-2d b c))))
    ab-center))

    
;a(x-x')^2 + b(y-y')^2 + c(z-z')^2 = 1
;a(x^2 - x - x' + x'^2) + b(y^2 - y - y' + y'^2) + c(z^2 - z - z' + z'^2) = 1

(load "matrix.scm")
(load "linalg.scm")

;;;;;;;;;;;;;;;; CURVE FITTING ;;;;;;;;;;;;;;;;;;;;;;;

(require-extension (srfi 1))

(define (square x) (* x x))
(define (cube x) (* x x x))

(define (parabola . points)
  (let ((n (length points)))
    (define (accum fun)
      (fold (lambda (a b) (+ (fun a) b)) 0 points))
    (let ((sum (accum (lambda (x) 1)))
          (xisum (accum car))
          (xi2sum (accum (lambda (x) (square (car x)))))
          (xi3sum (accum (lambda (x) (cube (car x)))))
          (xi4sum (accum (lambda (x) (square (square (car x))))))
          (yisum (accum cadr))
          (xiyisum (accum (lambda (x) (* (car x) (cadr x)))))
          (xi2yisum (accum (lambda (x) (* (square (car x)) (cadr x))))))
      (let ((A
             (list (list sum    xisum  xi2sum)
                   (list xisum  xi2sum xi3sum)
                   (list xi2sum xi3sum xi4sum)))
            (b (list yisum xiyisum xi2yisum)))
        (let ((inv (invert A)))
          (if (eq? inv 'none)
              (begin (display "Unable to represent") (newline))
              (let ((res (car (mult-matrix (list b) inv))))
                (display "a = ") (display (caddr res)) (newline)
                (display "b = ") (display (cadr res)) (newline)
                (display "c = ") (display (car res)) (newline))))))))

(define A '((3 2 2) (2 2 2) (2 2 2)))
(define b '(3 3 3))

;;;;;;;;;;;;;;; CALCULUS OPERATIONS ;;;;;;;;;;;;;;;;;;;     

;; compute the jacobian matrix f
(define (jacobian x0 y0 r0 . points)
  (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi)))
             (let ((ri (sqrt (+ (square dx) (square dy)))))
               (list (/ dx ri) (/ dy ri) -1)))))
       points))

;; compute residules for a circle
(define (residuals x0 y0 r0 . points)
   (list
    (map (lambda (x)
           (- r0 (sqrt (+ (square (- (car x) x0)) (square (- (cadr x) y0))))))
         points)))

;; compute update values
(define (circle-diff x0 y0 r0 . points)
  (mult-matrix
   (mult-matrix (invert
                 (mult-matrix (transpose (apply jacobian x0 y0 r0 points))
                              (apply jacobian x0 y0 r0 points)))
                (transpose (apply jacobian x0 y0 r0 points)))
   (transpose (apply residuals x0 y0 r0 points))))

;; iteratively apply update values
(define (circle-approx iters x0 y0 r0 . points)
  (if (= iters 0) (list x0 y0 r0)
      (let ((diff (apply circle-diff x0 y0 r0 points)))
        (apply circle-approx
               (- iters 1)
               (+ x0 (caar diff))
               (+ y0 (caadr diff))
               (+ r0 (caaddr diff))
               points))))

;;;;;; TEST (Success!) ;;;;;;;;;;
;; compute the jacobian matrix f
(define (jacobian x0 y0 r0 . points)
  (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi)))
               (list (* 2 dx) (* 2 dy) (- (* 2 r0))))))
       points))

;; compute residules for a circle
(define (residuals x0 y0 r0 . points)
   (list
    (map (lambda (x)
           (- (square r0) (+ (square (- x0 (car x))) (square (- y0 (cadr x))))))
         points)))

;;;;;; END TEST ;;;;;;;;;;;

;; ellipse   a0*(x0-xi)^2 + b0*(y0-yi)^2 = 1
(define (ellipse-jacobian a0 b0 x0 y0 points)
  (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi)))
               (list (square dx) (square dy)
                     (* 2 a0 dx) (* 2 b0 dy)))))
       points))

(define (ellipse-residuals a0 b0 x0 y0 points)
   (list
    (map (lambda (point)
           (let ((xi (car point))
                 (yi (cadr point)))
             (let ((dx (- x0 xi))
                   (dy (- y0 yi)))
               (- 1 (+ (* a0 (square dx)) (* b0 (square dy)))))))
         points)))

;; compute update values
(define (ellipse-diff a0 b0 x0 y0 points)
  (mult-matrix
   (mult-matrix (invert
                 (mult-matrix (transpose (ellipse-jacobian a0 b0 x0 y0 points))
                              (ellipse-jacobian a0 b0 x0 y0 points)))
                (transpose (ellipse-jacobian a0 b0 x0 y0 points)))
   (transpose (ellipse-residuals a0 b0 x0 y0 points))))

;; iteratively apply update values
(define (ellipse-approx iters a0 b0 x0 y0 points)
  (if (= iters 0) (list a0 b0 x0 y0)
      (let ((diff (ellipse-diff a0 b0 x0 y0 points)))
        (ellipse-approx
               (- iters 1)
               (+ a0 (caar diff))
               (+ b0 (caadr diff))
               (+ x0 (caaddr diff))
               (+ y0 (car (cadddr diff)))
               points))))

;;;;;;;;;; 3D ellipse ;;;;;;;;;;;;
(define (ellipse-3d-jacobian a0 b0 c0 x0 y0 z0 . points)
  (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point))
               (zi (caddr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi))
                 (dz (- z0 zi)))
               (list (square dx) (square dy) (square dz)
                     (* 2 a0 dx) (* 2 b0 dy) (* 2 c0 dz)))))
       points))

(define (ellipse-3d-residuals a0 b0 c0 x0 y0 z0 . points)
   (list
    (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point))
               (zi (caddr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi))
                 (dz (- z0 zi)))
               (- 1 (+ (* a0 (square dx))
                       (* b0 (square dy))
                       (* c0 (square dz)))))))
         points)))

;; compute update values
(define (ellipse-3d-diff a0 b0 c0 x0 y0 z0 . points)
  (let ((res (apply ellipse-3d-residuals a0 b0 c0 x0 y0 z0 points))
        (jac (apply ellipse-3d-jacobian a0 b0 c0 x0 y0 z0 points)))
    (mult-matrix
     (mult-matrix 
      (invert
       (mult-matrix
        (transpose jac)
        jac))
      (transpose jac))
     (transpose res))))


;; iteratively apply update values
(define (ellipse-3d-approx iters a0 b0 c0 x0 y0 z0 . points)
  (if (= iters 0) (list a0 b0 c0 x0 y0 z0)
      (let ((diff (apply ellipse-3d-diff a0 b0 c0 x0 y0 z0 points)))
        (apply ellipse-3d-approx
               (- iters 1)
               (+ a0 (caar diff))
               (+ b0 (caadr diff))
               (+ c0 (caaddr diff))
               (+ x0 (caaddr (cdr diff)))
               (+ y0 (caaddr (cddr diff)))
               (+ z0 (caaddr (cdddr diff)))
               points))))

(define (convert x)
  (cons (sqrt (car x))
        (cons (sqrt (cadr x))
              (cons (sqrt (caddr x))
                    (cdddr x)))))


;;
(define data5 '((803 587 587)
                (616 350 590)
                (336 481 600)
                (325 681 617)
                (560 591 343)
                (800 588 593)
                (740 424 589)))

(define bias6 '(618.2222222222 637.11111111 610.66666666))

(define data6 '((800 600 600)
                (327 668 618)
                (510 613 844)
                (804 594 580)
                (550 342 607)
                (612 783 432)
                (607 783 427)
                (554 751 788)
                (802 593 592)))

(define data7 '((25600 19200 19200)
                (17004 27157 19107)
                (11276 23680 19714)
                (17973 19167 10983)
                (17602 11062 18588)
                (17690 18603 27231)
                (10686 15889 19508)
                (20419 24923 14064)
                (25664 18765 18634)))

(define magdata '((-587.000000 -679.000000 2811.000000)
                  (3043.000000 694.000000 -882.000000)
                  (3005.000000 -99.000000 758.000000)
                  (2835.000000 2311.000000 -59.000000)
                  (1964.000000 -1777.000000 582.000000)
                  (1547.000000 -102.000000 2662.000000)
                  (2006.000000 -1472.000000 -1545.000000)
                  (928.000000 1300.000000 -3036.000000)
                  (-257.000000 -814.000000 2838.000000)))

 
;;;;;;;;;;;;;;;; linear for rotation ;;;;;;;;;;;;;;;;
;;;;   ax+by+cz = 0
;;;;  P = (ax + by + cz)^2
;;;;  dP/da = 0, dP/db = 0, dP/dc = 0
;;;;  ax^2 + bxy + cxz = 0, axy + by^2 + cyz = 0, axz + byz + cz^2 = 0

(define points '((0 1 0) (0 -1 0) (0 0 1)))
(define (pointtotal points)
  (map (lambda (x) (apply + x)) (transpose points)))

(define (find-plane points)
  (define (pt f1 f2)
    (apply + (map (lambda (x) (* (f1 x) (f2 x))) points)))
  (let ((xx (pt car car))
        (xy (pt car cadr))
        (xz (pt car caddr))
        (yy (pt cadr cadr))
        (yz (pt cadr caddr))
        (zz (pt caddr caddr)))
      (let ((A (list
                (list xx xy xz)
                (list xy yy yz)
                (list xz yz zz)))
            (R (list (list 1 1 1))))
        (let ((vec (mult-matrix (invert A) (transpose R))))
          (normalize (car (transpose vec)))))))

(define data '((0.026703 -0.082700 0.996217)
               (-0.012077 -0.045070 -0.998911)
               (0.005511 -0.996351 -0.085174)
               (-0.011383 0.848217 -0.529526)))


