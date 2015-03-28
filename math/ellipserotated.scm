(load "matrix.scm")
(load "linalg.scm")

;; ellipse   a0*(x0-xi)^2 + b0*(y0-yi)^2 + c0*(x0-xi)*(y0-yi) = 1
(define (test a0 b0 x0 c0 y0 x y)
  (+ (* a0 (square (- x0 x)))
     (* b0 (square (- y0 y)))
     (* c0 (- x0 x) (- y0 y))))

(define (ellipse-jacobian a0 b0 x0 c0 y0 points)
  (map (lambda (point)
         (let ((xi (car point))
               (yi (cadr point)))
           (let ((dx (- x0 xi))
                 (dy (- y0 yi)))
               (list (square dx) (square dy) (* dx dy)
                     (+ (* 2 a0 dx) (* c0 dy)) (+ (* 2 b0 dy) (* c0 dx))))))
       points))

(define (ellipse-residuals a0 b0 c0 x0 y0 points)
   (list
    (map (lambda (point)
           (let ((xi (car point))
                 (yi (cadr point)))
             (let ((dx (- x0 xi))
                   (dy (- y0 yi)))
               (- 1 (* a0 (square dx)) (* b0 (square dy)) (* c0 dx dy)))))
         points)))

;; compute update values
(define (ellipse-diff a0 b0 c0 x0 y0 points)
  (let ((j (ellipse-jacobian a0 b0 c0 x0 y0 points))
        (r (ellipse-residuals a0 b0 c0 x0 y0 points)))
    (let ((inverse (invert (mult-matrix (transpose j) j))))
      (if (equal? 'none inverse)
          (error "not enough data")
          (mult-matrix (mult-matrix inverse (transpose j)) (transpose r))))))

;; iteratively apply update values
(define (ellipse-approx iters a0 b0 c0 x0 y0 points)
  (if (= iters 0) (list a0 b0 c0 x0 y0)
      (let ((diff (ellipse-diff a0 b0 c0 x0 y0 points)))
        (ellipse-approx
               (- iters 1)
               (+ a0 (caar diff))
               (+ b0 (caadr diff))
               0;(+ c0 (caaddr diff))
               (+ x0 (car (cadddr diff)))
               (+ y0 (caar (cddddr diff)))
               points))))
